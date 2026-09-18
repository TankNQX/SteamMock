#!/usr/bin/env python3
"""Turn gen/steam_api.idl.json into the stub's trampolines and export list.

The IDL is the single source of truth for what the stub exports. Adding a call
means adding one entry there and regenerating:

    python gen/generate.py            # rewrite the generated files
    python gen/generate.py --check    # fail if they are stale (used by CI)

Generated outputs:
    src/generated/api_stub.cpp             one trampoline per call
    src/generated/steam_api_exports.def    the export list, so names stay undecorated
    python/steambridge/surface.json        what the backend knows about each call
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

CPP_TYPE = {
    "bool": "bool",
    "int32": "std::int32_t",
    "uint32": "std::uint32_t",
    "int64": "std::int64_t",
    "uint64": "std::uint64_t",
    "float": "float",
    "double": "double",
    "cstring": "const char*",
    "opaque_ptr": "void*",
}

# Which helper in bridge/call.hpp builds this type's argument.
ARG_HELPER = {
    "bool": "arg_bool",
    "int32": "arg_int",
    "uint32": "arg_uint",
    "int64": "arg_int",
    "uint64": "arg_uint",
    "float": "arg_real",
    "double": "arg_real",
    "cstring": "arg_cstring",
    "opaque_ptr": "arg_pointer",
}

# What a call returns when nobody answers it: the same values a game sees when
# Steam is not running. Strings get a real empty string rather than nullptr, so a
# game that forgets to null check does not fall over.
RETURN_DEFAULT = {
    "bool": "false",
    "int32": "0",
    "uint32": "0",
    "int64": "0",
    "uint64": "0",
    "float": "0.0f",
    "double": "0.0",
    "cstring": "kEmptyString",
    "opaque_ptr": "nullptr",
}

# The expression that reads a reply's return value into `result`, cast to the
# declared type: the bridge carries everything as a JSON number, so the narrower
# integer and float types need saying out loud (and the compiler says so under
# /WX if we forget).
REPLY_EXPR = {
    "bool": "steambridge::reply_bool(reply)",
    "int32": "static_cast<std::int32_t>(steambridge::reply_int(reply))",
    "uint32": "static_cast<std::uint32_t>(steambridge::reply_uint(reply))",
    "int64": "steambridge::reply_int(reply)",
    "uint64": "steambridge::reply_uint(reply)",
    "float": "static_cast<float>(steambridge::reply_real(reply))",
    "double": "steambridge::reply_real(reply)",
    # The borrowed text lives in the reply object, which dies with this call, so
    # the stub hands back its own per-thread copy.
    "cstring": "steambridge::reply_cstring(reply, kEmptyString)",
    "opaque_ptr": "steambridge::reply_pointer(reply)",
}

# How an out-parameter in a reply is written back. Type-correct on purpose: a
# JSON boolean read as an integer is silently false, and that is exactly the
# kind of thing a harness must not get wrong.
OUT_EXPR = {
    "bool": "static_cast<bool>(value->as_bool())",
    "int32": "static_cast<std::int32_t>(value->as_int64())",
    "uint32": "static_cast<std::uint32_t>(value->as_uint64())",
    "int64": "value->as_int64()",
    "uint64": "value->as_uint64()",
    "float": "static_cast<float>(value->as_double())",
    "double": "value->as_double()",
}

DIAGNOSTIC_EXPORTS = ["SteamBridge_SessionId", "SteamBridge_Stats", "SteamBridge_Version"]


class IdlError(Exception):
    pass


def load_idl(path: Path) -> dict:
    with path.open("r", encoding="utf-8") as handle:
        idl = json.load(handle)
    if "calls" not in idl or not isinstance(idl["calls"], list):
        raise IdlError("the IDL needs a 'calls' array")

    seen: set[str] = set()
    for call in idl["calls"]:
        name = call.get("name")
        if not name or not isinstance(name, str):
            raise IdlError("every call needs a 'name'")
        if name in seen:
            raise IdlError(f"{name} appears twice")
        seen.add(name)
        returns = call.get("returns", "void")
        if returns != "void" and returns not in CPP_TYPE:
            raise IdlError(f"{name}: unknown return type '{returns}'")
        for param in call.get("params", []):
            ptype = param.get("type")
            if ptype not in CPP_TYPE:
                raise IdlError(f"{name}.{param.get('name')}: unknown type '{ptype}'")
            direction = param.get("dir", "in")
            if direction not in ("in", "out"):
                raise IdlError(f"{name}.{param.get('name')}: dir must be 'in' or 'out'")
            if direction == "out" and ptype in ("cstring", "opaque_ptr"):
                raise IdlError(
                    f"{name}.{param.get('name')}: out parameters of type '{ptype}' need a length or "
                    "an owned buffer - add a dedicated kind when you need one"
                )
    idl["calls"].sort(key=lambda call: call["name"])
    return idl


def param_cpp(param: dict) -> str:
    cpp = CPP_TYPE[param["type"]]
    if param.get("dir", "in") == "out":
        return f"{cpp}*"
    return cpp


def param_name(param: dict) -> str:
    return param["name"]


def signature(call: dict) -> str:
    param_list = ", ".join(
        f"{param_cpp(param)} {param_name(param)}" for param in call.get("params", [])
    )
    # A parameterless function is spelled (void) so the C prototype is explicit.
    if not param_list:
        param_list = "void"
    returns = "void" if call.get("returns", "void") == "void" else CPP_TYPE[call["returns"]]
    return f"{returns} STEAMBRIDGE_CALL {call['name']}({param_list})"


def render_body(call: dict) -> list[str]:
    """The body of one trampoline: build the arguments, ask, fall back."""
    name = call["name"]
    returns = call.get("returns", "void")
    params = call.get("params", [])
    lines: list[str] = []

    if returns != "void":
        lines.append(f"    {CPP_TYPE[returns]} result = {RETURN_DEFAULT[returns]};")
    lines.append("    try {")
    lines.append("        steambridge::Json args = steambridge::Json::object();")
    for param in params:
        target = param_name(param)
        helper = ARG_HELPER[param["type"]]
        source = param_name(param)
        if param.get("dir", "in") == "out":
            # Send what the caller passed in, so the backend can see the current
            # value; a null pointer is reported as null and never dereferenced.
            lines.append(f"        args.set(\"{target}\", {source} != nullptr")
            lines.append(f"                                  ? steambridge::{helper}("
                         f"static_cast<{CPP_TYPE[param['type']]}>(*{source}))")
            lines.append("                                  : steambridge::Json::null());")
        elif param["type"] == "cstring":
            lines.append(f"        args.set(\"{target}\", steambridge::arg_cstring({source}));")
        else:
            lines.append(f"        args.set(\"{target}\", steambridge::{helper}({source}));")
    lines.append("")
    lines.append("        steambridge::Json reply;")
    invoke = f'steambridge::invoke("{name}", args, reply)'
    if returns == "void" and not any(p.get("dir", "in") == "out" for p in params):
        lines.append(f"        (void){invoke};")
    else:
        lines.append(f"        if ({invoke}) {{")
        for param in params:
            if param.get("dir", "in") != "out":
                continue
            target = param_name(param)
            lines.append(f"            if ({target} != nullptr) {{")
            lines.append(f"                const steambridge::Json* value = "
                         f"steambridge::reply_out(reply, \"{target}\");")
            lines.append(f"                if (value != nullptr && !value->is_null()) {{")
            lines.append(f"                    *{target} = {OUT_EXPR[param['type']]};")
            lines.append("                }")
            lines.append("            }")
        if returns != "void":
            lines.append(f"            result = {REPLY_EXPR[returns]};")
        lines.append("        }")
    lines.append("    } catch (...) {")
    lines.append("        // Never let an exception cross into the game.")
    lines.append("    }")
    if returns != "void":
        lines.append("    return result;")
    return lines


def render_cpp(idl: dict) -> str:
    count = len(idl["calls"])
    out: list[str] = []
    out.append("// ============================================================================")
    out.append("//  GENERATED FILE - do not edit by hand.")
    out.append("//")
    out.append(f"//  Source:   gen/steam_api.idl.json (surface '{idl.get('surface', '?')}', "
               f"revision {idl.get('revision', '?')}, {count} calls)")
    out.append("//  Regenerate: python gen/generate.py")
    out.append("// ============================================================================")
    out.append("")
    out.append('#include "bridge/call.hpp"')
    out.append('#include "bridge/export.hpp"')
    out.append("")
    out.append("namespace {")
    out.append("")
    out.append("// Handed back for a string-returning call nobody answered, so a game that")
    out.append("// skips its null check still reads empty text instead of faulting.")
    out.append("const char kEmptyString[] = \"\";")
    out.append("")
    out.append("}  // namespace")
    out.append("")
    for call in idl["calls"]:
        out.append(f"// {call['name']}")
        out.append(f"STEAMBRIDGE_EXPORT {signature(call)} {{")
        out.extend(render_body(call))
        out.append("}")
        out.append("")
    return "\n".join(out)


def render_def(idl: dict) -> str:
    out = ["; GENERATED FILE - do not edit by hand.",
           "; Regenerate: python gen/generate.py",
           ";",
           "; A .def rather than __declspec(dllexport) so the exported names are exactly the",
           "; ones games import, with no decoration on x86 and no difference on x64.",
           "EXPORTS"]
    for name in sorted([call["name"] for call in idl["calls"]] + DIAGNOSTIC_EXPORTS):
        out.append(f"    {name}")
    return "\n".join(out) + "\n"


def render_surface(idl: dict) -> str:
    """The Python side's view: same IDL, normalised, so the backend can answer
    defaults and validate what a game asks for."""
    calls = []
    for call in idl["calls"]:
        calls.append({
            "name": call["name"],
            "returns": call.get("returns", "void"),
            "params": [
                {"name": p["name"], "type": p["type"], "dir": p.get("dir", "in")}
                for p in call.get("params", [])
            ],
        })
    payload = {
        "version": 1,
        "surface": idl.get("surface", "?"),
        "revision": idl.get("revision", 0),
        "calls": calls,
    }
    return json.dumps(payload, indent=2) + "\n"


def write_or_check(path: Path, content: str, check: bool, problems: list[str]) -> None:
    # Bytes, not text: the generated files are line-feed terminated on every
    # platform, and Python 3.9's write_text has no newline argument.
    payload = content.encode("utf-8")
    current = path.read_bytes() if path.exists() else None
    if check:
        if current != payload:
            problems.append(str(path.relative_to(ROOT)))
        return
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(payload)
    print(f"wrote {path.relative_to(ROOT)}")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--idl", type=Path, default=ROOT / "gen" / "steam_api.idl.json")
    parser.add_argument("--check", action="store_true",
                        help="do not write; fail if the generated files are out of date")
    args = parser.parse_args()

    try:
        idl = load_idl(args.idl)
    except IdlError as error:
        print(f"idl error: {error}", file=sys.stderr)
        return 2

    outputs = [
        (ROOT / "src" / "generated" / "api_stub.cpp", render_cpp(idl)),
        (ROOT / "src" / "generated" / "steam_api_exports.def", render_def(idl)),
        (ROOT / "python" / "steambridge" / "surface.json", render_surface(idl)),
    ]

    problems: list[str] = []
    for path, content in outputs:
        write_or_check(path, content, args.check, problems)

    if args.check:
        if problems:
            print("stale generated files (run: python gen/generate.py):", file=sys.stderr)
            for path in problems:
                print(f"  {path}", file=sys.stderr)
            return 1
        print("generated files are up to date")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
