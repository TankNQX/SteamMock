#!/usr/bin/env python3
# ---------------------------------------------------------------------------
#  steamworks_sdk_import - read installed Steamworks SDKs and write the
#  interface layouts, or say what the ones in use are missing.
# ---------------------------------------------------------------------------
#  gen/steam_interfaces.json is the interface layouts the stub hands out: Valve's
#  own data, which is why it is not committed. It is built from SDKs the builder
#  has, and an SDK declares exactly the generation that was current when Valve
#  released it - 1.46 declares SteamUser020, 1.57 declares SteamUser023, and
#  neither declares SteamUser019. A game built in between asks for the strings of
#  its own generation and gets null where it expected an interface, so the file is
#  the union of every SDK a builder can point at: the Spacewar this harness runs
#  against wants twenty-one version strings, and the SDKs that were to hand when
#  the file was last built declare thirteen of them.
#
#  Nothing here invents a generation: what an SDK does not declare is reported by
#  --wanted against the game, so a short file says so rather than handing a game
#  nothing and letting it fail somewhere else.
#
#    python tools/steamworks_sdk_import.py --sdk <sdk>/public/steam --list
#    python tools/steamworks_sdk_import.py --sdk 1.46=<sdk> --out layout.json
#    python tools/steamworks_sdk_import.py --sdk 1.46=<sdk> --diff gen/steam_interfaces.json
#    python tools/steamworks_sdk_import.py --sdk 1.39=<sdk> --wanted <game>/SteamworksExample.exe
#
#  The two halves, because they are worth keeping apart:
#
#    reading     one SDK's headers become versions, slots, structures and events.
#                A version string comes from the interface's own
#                STEAMXXX_INTERFACE_VERSION define, its slots from the class's
#                virtuals in declaration order, and its sizes from the ABI the
#                SDK's own declarations imply. What this cannot read is reported,
#                never guessed at.
#    merging     several SDKs, oldest first, become one file the way the existing
#                one was built: a version string declared twice is the newest
#                generation, and a slot only an older one had is appended.
#
#  Requires cxxheaderparser and pcpp (tools/requirements.txt). They are needed by
#  this tool and by nothing else in the repository: the C++ build still needs no
#  Python at all.
#
#  `--selftest` runs the reader, the merge, the wanted check and the writer against
#  headers and a game written here rather than against an SDK, so the tool can be
#  checked on a machine that has none.

from __future__ import annotations

import argparse
import json
import os
import re
import sys
from dataclasses import dataclass, field, replace
from typing import Dict, List, Optional, Sequence, Tuple

try:
    from cxxheaderparser.options import ParserOptions
    from cxxheaderparser.preprocessor import make_pcpp_preprocessor
    from cxxheaderparser.simple import parse_file
except ImportError:  # pragma: no cover - the message is the point
    sys.stderr.write(
        "steamworks_sdk_import: cxxheaderparser is not installed.\n"
        "  python -m venv .venv && .venv/Scripts/pip install -r tools/requirements.txt\n"
    )
    raise SystemExit(2)

# ---------------------------------------------------------------------------
#  The kind table, mirrored from src/interfaces.cpp
# ---------------------------------------------------------------------------
#  A wire kind is what a layout carries. This is the same list the generator
#  validates against, and it is duplicated rather than read out of the C++ so a
#  disagreement between the two is something the diff can find.

KINDS = (
    "bool",
    "int8",
    "uint8",
    "int16",
    "uint16",
    "int32",
    "uint32",
    "int64",
    "uint64",
    "float",
    "double",
    "intptr",
    "uintptr",
    "enum",
    "cstring",
    "opaque_ptr",
    "bytes",
    "out_bytes",
)

# The fundamental spellings an SDK uses, and the kind each lands on. `long` is
# 32-bit on the targets this is about, which is why it is not int64.
FUNDAMENTAL_KINDS = {
    "bool": "bool",
    "char": "int8",
    "signed char": "int8",
    "unsigned char": "uint8",
    "short": "int16",
    "short int": "int16",
    "signed short": "int16",
    "unsigned short": "uint16",
    "unsigned short int": "uint16",
    "int": "int32",
    "signed": "int32",
    "signed int": "int32",
    "unsigned": "uint32",
    "unsigned int": "uint32",
    "long": "int32",
    "long int": "int32",
    "unsigned long": "uint32",
    "long long": "int64",
    "unsigned long long": "uint64",
    "__int64": "int64",
    "unsigned __int64": "uint64",
    "float": "float",
    "double": "double",
    "intptr_t": "intptr",
    "uintptr_t": "uintptr",
    "size_t": "uintptr",
}

# What a member of a structure or an event is declared as. The C++ side refuses a
# member that is neither one of these nor an enum, which is the rule the sizes in
# the file follow.
MEMBER_KINDS = {
    "bool": ("bool", 1, 1),
    "int8": ("std::int8_t", 1, 1),
    "uint8": ("std::uint8_t", 1, 1),
    "int16": ("std::int16_t", 2, 2),
    "uint16": ("std::uint16_t", 2, 2),
    "int32": ("std::int32_t", 4, 4),
    "uint32": ("std::uint32_t", 4, 4),
    "int64": ("std::int64_t", 8, 8),
    "uint64": ("std::uint64_t", 8, 8),
    "float": ("float", 4, 4),
    "double": ("double", 8, 8),
    "enum": ("std::int32_t", 4, 4),
}

# ---------------------------------------------------------------------------
#  The one policy this cannot read out of a header
# ---------------------------------------------------------------------------
#  A byte buffer is either something the wire carries or something it only
#  reports, and which of the two is a choice about *this* harness rather than a
#  fact about the SDK: the same `const void *` is a payload in SendP2PPacket and
#  a scratch buffer in InitiateGameConnection. The file records the choice, so it
#  is mirrored here, keyed by the class and method it belongs to. Every entry was
#  taken from gen/steam_interfaces.json; a method that is not here maps the way
#  every other method does, and the diff says so if that is wrong.

BYTE_BUFFERS: Dict[Tuple[str, str, str], Tuple[str, str]] = {
    ("ISteamMatchmaking", "SendLobbyChatMsg", "pvMsgBody"): ("bytes", "cubMsgBody"),
    ("ISteamMatchmaking", "GetLobbyChatEntry", "pvData"): ("out_bytes", "cubData"),
    ("ISteamNetworking", "SendP2PPacket", "pubData"): ("bytes", "cubData"),
    ("ISteamNetworking", "ReadP2PPacket", "pubDest"): ("out_bytes", "cubDest"),
    # The auth tickets are the one buffer a call hands over to be filled and then
    # reads back out of the caller's own memory: Spacewar's game server answers
    # `BeginAuthSession` with the ticket it was given, and a ticket that was never
    # written into that buffer is the `GetAuthSessionTicket didn't give us a good
    # ticket` both games print. No other call follows that shape - `BeginAuthSession`
    # takes its own ticket as a `const void *` and only reads it, so its `pAuthTicket`
    # stays an opaque pointer - which is why the choice stays here in the table
    # rather than becoming a rule in the reader.
    ("ISteamGameServer", "GetAuthSessionTicket", "pTicket"): ("out_bytes", "cbMaxTicket"),
    ("ISteamUser", "GetAuthSessionTicket", "pTicket"): ("out_bytes", "cbMaxTicket"),
    ("ISteamUser", "GetEncryptedAppTicket", "pTicket"): ("out_bytes", "cbMaxTicket"),
}

# The pack the layouts were measured at: a callback struct is read by a game at
# eight, and so is anything nested in one. A pointer is eight bytes for the same
# reason - the layouts describe the x64 ABI even where the game is 32-bit.
CALLBACK_PACK = 8
POINTER_SIZE = 8
ENUM_SIZE = 4


# ---------------------------------------------------------------------------
#  Types, normalised out of the parser's nodes
# ---------------------------------------------------------------------------


@dataclass
class Ty:
    """A type as written, with the parser's nodes peeled off it."""

    form: str  # 'fundamental', 'named', 'void', 'pointer', 'function', 'unknown'
    name: str = ""
    const: bool = False
    pointee: Optional["Ty"] = None
    count: Optional[str] = None  # an array extent, for a member

    def describe(self) -> str:
        if self.form == "pointer":
            return ("const " if self.const else "") + self.pointee.describe() + " *"
        if self.form == "function":
            return "<function pointer>"
        if self.form == "unknown":
            return "<unreadable>"
        return self.name


def _typename_text(node) -> str:
    """The spelling of a name node, or its template argument's name."""
    try:
        segments = node.segments
    except AttributeError:
        return str(node)
    names: List[str] = []
    for segment in segments:
        name = getattr(segment, "name", None)
        if name is None:
            names.append(str(segment))
            continue
        try:
            names.append("::".join(part.name for part in name.segments))
        except AttributeError:
            names.append(name)
    return "".join(names)


def normalise(node) -> Ty:
    """One of cxxheaderparser's type nodes as a `Ty`."""
    name = type(node).__name__
    if name == "Pointer":
        return Ty("pointer", const=getattr(node, "const", False), pointee=normalise(node.ptr_to))
    if name == "Array":
        inner = normalise(node.array_of)
        count = _flatten(node.size)
        inner.count = count
        return inner
    if name == "Type":
        inner = normalise(node.typename)
        if getattr(node, "const", False):
            # `const char *` is a string where `char *` is a buffer the call fills
            # in, and the const sits on the pointee rather than on the pointer.
            inner.const = True
        return inner
    if name == "FunctionType":
        return Ty("function", const=getattr(node, "const", False))
    if name == "Reference":
        # A reference is a pointer by another name for what a row says; none of
        # the SDK's slots take one, so it is reported rather than mapped.
        return Ty("unknown")
    if name == "FundamentalSpecifier":
        spelling = getattr(node, "name", "")
        if spelling == "void":
            return Ty("void", name="void")
        return Ty("fundamental", name=spelling)
    if name == "NameSpecifier":
        return _spelling(_typename_text(node.name))
    if name == "PQName":
        return normalise_pqname(node)
    if hasattr(node, "typename"):
        return normalise(node.typename)
    return Ty("unknown")


def normalise_pqname(node) -> Ty:
    return _spelling(_typename_text(node))


def _spelling(text: str) -> Ty:
    """A written type name: the fundamental it is, or a name to look up later."""
    if text == "void":
        return Ty("void", name="void")
    if text in FUNDAMENTAL_KINDS:
        return Ty("fundamental", name=text)
    return Ty("named", name=text)


def _flatten(size) -> Optional[str]:
    """An array extent or an enumerator's value, as it was written.

    The parser hands these back as a `Value` of tokens - a number, and for a sum
    like `k_iSteamMatchmakingCallbacks + 13` the operator between them - so the
    tokens are joined in the order they were spelled.
    """
    if size is None:
        return None
    tokens = getattr(size, "tokens", size)
    if not isinstance(tokens, list):
        tokens = [tokens]
    text = "".join(str(getattr(token, "value", token)) for token in tokens).strip()
    return text or None


def _const_of(node) -> bool:
    return bool(getattr(node, "const", False))


# ---------------------------------------------------------------------------
#  What the headers said
# ---------------------------------------------------------------------------


@dataclass
class Aggregate:
    """A class, struct or union: what its members are, or that it has virtuals."""

    name: str
    is_union: bool = False
    aggregates: List["Aggregate"] = field(default_factory=list)
    members: List[Tuple[Ty, str]] = field(default_factory=list)
    has_virtuals: bool = False


@dataclass
class Interface:
    """One class in one header, with the version string that header declares."""

    name: str
    version: str
    slots: List[object] = field(default_factory=list)  # rows, built later
    notes: List[str] = field(default_factory=list)


@dataclass
class Callback:
    name: str
    callback_id: int
    aggregate: Aggregate


@dataclass
class FlatFunction:
    name: str
    interface: str
    params: List[Ty]
    returns: Ty


@dataclass
class Sdk:
    """One SDK's headers, read once."""

    label: str
    path: str
    typedefs: Dict[str, Ty] = field(default_factory=dict)
    enums: Dict[str, List[Tuple[str, int]]] = field(default_factory=dict)
    # The plain integers a header declares at file scope - `const uint32
    # k_unEnumeratePublishedFilesMaxResults = 50;` - which is what an array extent
    # is written as when it is not a bare number.
    constants: Dict[str, int] = field(default_factory=dict)
    aggregates: Dict[str, Aggregate] = field(default_factory=dict)
    # The classes each header declared, in the order the headers were read: an
    # Aggregate says what a structure is made of, while a vtable also needs the
    # declaration itself - the order of the virtuals, and which are protected.
    headers: List[List[object]] = field(default_factory=list)
    interfaces: List[Interface] = field(default_factory=list)
    callbacks: Dict[str, Callback] = field(default_factory=dict)
    flat: Dict[str, List[FlatFunction]] = field(default_factory=dict)
    unread: List[str] = field(default_factory=list)

    # A payload declares its id as `k_iCallback = k_iSteamXxxCallbacks + n`, and the
    # base is declared in another header - `steam_api_internal.h` in the 1.46 era,
    # `isteamclient.h` in the 1.39 one. Which header that is decides whether the sum
    # can be read at the moment the payload is: the bases are recorded here and the
    # ids are resolved once every header has been read, so the order headers are read
    # in cannot decide whether a payload is read at all.
    pending: List[Tuple[str, str, object, Aggregate]] = field(default_factory=list)

    def find_aggregate(self, name: str) -> Optional[Aggregate]:
        return self.aggregates.get(name)

    def extent(self, text: Optional[str]) -> Optional[int]:
        """An array extent as a number, against the tables this SDK declares.

        An extent is written the way its header spells it - `16`, or the name of a
        constant - and the file has to carry a number: the declaration rendered
        from it is read by a compiler that has never seen these headers, so a name
        that survives into the file is an undeclared identifier there.
        """
        if not text:
            return None
        written = text.strip()
        try:
            return int(written, 0)
        except ValueError:
            pass
        match = re.match(r"^\s*(k_[A-Za-z_0-9]+)\s*(?:([+-])\s*(\d+))?\s*$", written)
        if match is None:
            return None
        base_name, sign, offset = match.groups()
        base = self.constants.get(base_name)
        if base is None:
            base = _enum_member(self, base_name)
        if base is None:
            return None
        if sign is None:
            return base
        return base + int(offset) if sign == "+" else base - int(offset)

    # -- typedefs ----------------------------------------------------------

    def canonical(self, ty: Ty) -> Ty:
        """A type with its typedefs expanded, one level of pointer kept.

        The const comes with it: `const SteamItemInstanceID_t *` is a list of ids
        the call reads, and the const is written on the type name rather than on the
        pointer, so dropping it on the way through the typedef would lose the one
        thing that says so.
        """
        seen = set()
        current = ty
        while current.form == "named" and current.name in self.typedefs:
            if current.name in seen:
                return Ty("unknown")
            seen.add(current.name)
            expanded = self.typedefs[current.name]
            if current.const:
                expanded = replace(expanded, const=True)
            if current.count is not None:
                # The extent belongs to the declaration, not to the typedef that
                # spelled its element: `uint8 m_FileSHA[20]` is twenty bytes even
                # though `uint8` is a name by the time the member is read, and a
                # size that counted it as one byte is a size the assert catches.
                expanded = replace(expanded, count=current.count)
            current = expanded
        if current.form == "pointer":
            inner = self.canonical(current.pointee)
            return Ty("pointer", const=current.const, pointee=inner, count=current.count)
        return current


def _walk_scopes(scope, prefix: str = "") -> Aggregate:
    """A ClassScope tree as an Aggregate tree."""
    aggregate = Aggregate(name=prefix, is_union=getattr(scope.class_decl, "classkey", "") == "union")
    for method in getattr(scope, "methods", []):
        if getattr(method, "virtual", False):
            aggregate.has_virtuals = True
    for member in getattr(scope, "fields", []):
        aggregate.members.append((normalise(member.type), member.name or ""))
    for nested in getattr(scope, "classes", []):
        nested_name = _typename_text(nested.class_decl.typename)
        aggregate.aggregates.append(_walk_scopes(nested, nested_name))
    return aggregate


def _interface_version_defines(path: str) -> Dict[str, str]:
    """`STEAMXXX_INTERFACE_VERSION` -> the string it declares, from a raw header.

    Read as text rather than after preprocessing, because these are the defines a
    game is compiled against and they survive preprocessing only as literals.
    """
    text = read_text(path)
    found: Dict[str, str] = {}
    for match in re.finditer(
        r'^\s*#\s*define\s+([A-Za-z0-9_]*INTERFACE_VERSION)\s+"([^"]+)"', text, re.MULTILINE
    ):
        found[match.group(1)] = match.group(2)
    return found


def _define_key(define: str) -> str:
    key = re.sub(r"^STEAM", "", define)
    key = re.sub(r"_?INTERFACE_VERSION.*$", "", key)
    return key.lower()


def _class_key(name: str) -> str:
    """A class name as a define spells it: `ISteamApps` is `APPS`."""
    return re.sub(r"^isteam", "", name.lower())


def _interface_class_for(define: str, names: Sequence[str]) -> Optional[str]:
    """Which class a header's version define belongs to.

    `isteammatchmaking.h` declares four of each, and the define names them the way
    Valve spells them - STEAMMATCHMAKINGSERVERS against ISteamMatchmakingServers -
    so the class is the one whose name, minus its leading I and Steam, is the
    define minus its STEAM and INTERFACE_VERSION.
    """
    key = _define_key(define)
    for name in names:
        if _class_key(name) == key:
            return name
    for name in names:
        if _class_key(name).startswith(key) or key.startswith(_class_key(name)):
            return name
    return None


def read_text(path: str) -> str:
    # utf-8-sig, because a Steamworks header may open with a byte order mark.
    with open(path, "r", encoding="utf-8-sig", errors="replace") as handle:
        return handle.read()


def _preprocessor(steam_dir: str):
    """pcpp, plus the spellings it leaves behind that the parser cannot read.

    Two of them, both about how a declaration is written rather than what it says:

    `extern "C" typedef void (__cdecl *Hook)(int, const char *);` is legal C++ and
    cxxheaderparser 2.0 refuses it, so the `extern "C"` half is dropped. And
    `steam_api_internal.h` is meant to be included *from* `steam_api_common.h`, so
    its own declarations are spelled `S_API void S_CALLTYPE SteamAPI_RegisterCallback(
    ... )` with neither macro defined in the file itself: read on its own - which is
    what reading every header on its own means - the parser reads `S_API void` as a
    declaration of `S_API`, and stops there. Both spellings are dropped, and nothing
    about a slot turns on either.

    The encoding is named rather than left to the platform, because these headers
    are not utf-8: a © in a copyright line is one 0xa9 byte, which every locale but
    the ANSI one refuses. `isteamclient.h` then opens with a byte order mark on top
    of that - three bytes that latin-1 reads as three characters, and the lexer
    calls those illegal - so the three are dropped again below.
    """
    inner = make_pcpp_preprocessor(
        include_paths=[steam_dir, os.path.dirname(os.path.abspath(steam_dir))],
        encoding="latin-1",
        # A header with a payload in it brackets that payload in
        # `#pragma pack( push, 4 )` or `( push, 8 )` and errors out if neither pack
        # is named, so the pack these layouts are measured at is named: eight, which
        # is what the file in the tree records every structure and payload at.
        defines=["VALVE_CALLBACK_PACK_LARGE"],
    )

    # The annotations Valve's own binding generator reads, in the form an SDK of the
    # 1.39 era writes them: `CALL_RESULT( FileDetailsResult_t )` and its dozen
    # relatives are spelled in headers that do not include the half of the chain
    # defining them, and read on its own the annotation survives into the
    # declaration - `isteamapps.h` stops the parser at the `virtual` after it, which
    # costs the whole interface. They are annotations (`API_GEN` turns them into a
    # clang attribute and every other build gets nothing) and say nothing about a
    # slot: the call goes, and the declaration reads as it does everywhere else.
    ANNOTATIONS = re.compile(
        r"\b(?:CALL_RESULT|CALL_BACK|METHOD_DESC|IGNOREATTR|OUT_STRUCT|OUT_STRING"
        r"|OUT_ARRAY_CALL|OUT_ARRAY_COUNT|ARRAY_COUNT_D|ARRAY_COUNT|OUT_BUFFER_COUNT"
        r"|BUFFER_COUNT|OUT_STRING_COUNT|DESC)\s*\([^()]*\)"
    )

    def preprocess(filename: str, content: Optional[str]) -> str:
        text = inner(filename, content)
        text = text.replace("\ufeff", "").replace("\u00ef\u00bb\u00bf", "")
        text = re.sub(r"\bS_API\b", "", text)
        text = re.sub(r"\bS_CALLTYPE\b", "", text)
        text = ANNOTATIONS.sub("", text)
        return re.sub(r'\bextern\s*"C"\s+typedef\b', "typedef", text)

    return preprocess


def parse_sdk(label: str, steam_dir: str) -> Sdk:
    """Read one SDK's `public/steam` directory."""
    sdk = Sdk(label=label, path=steam_dir)
    options = ParserOptions(preprocessor=_preprocessor(steam_dir))

    for name in _read_order(os.listdir(steam_dir)):
        if not name.endswith(".h"):
            continue
        path = os.path.join(steam_dir, name)
        try:
            parsed = parse_file(path, options=options)
        except Exception as error:  # a header this cannot read is reported, not skipped silently
            sdk.unread.append("%s: %s" % (name, _one_line(str(error))))
            continue
        _absorb(sdk, name, path, parsed)

    _read_flat_header(sdk, options, steam_dir)
    _resolve_callbacks(sdk)
    return sdk


def _one_line(text: str) -> str:
    return re.sub(r"\s+", " ", text).strip()


def _read_order(names: Sequence[str]) -> List[str]:
    """The headers to read, name by name, so that one SDK is read the same way twice.

    The order used to matter: a payload writes its id as `k_iSteamXxxCallbacks + n`
    and the base lives in another header, which is `steam_api_internal.h` in the 1.46
    era and `isteamclient.h` in the 1.39 one. It no longer does - the ids are
    resolved once every header has been read - so this is only the reading's own
    order, which keeps two runs over one SDK comparable.
    """
    return sorted(names)


def _enum_key(name: str, where: str) -> str:
    """What an enum is filed under, which for one with no name is where it sits.

    An enum written as `enum { k_iCallback = ... }` has no name of its own, and the
    name the parser invents for it - `AnonymousName(1)` - is not unique across the
    headers of one SDK. The enumerators still matter - a payload's id is written as
    one of the `k_iSteamXxxCallbacks` bases - so it is filed under the header or the
    class it was declared in rather than dropped.
    """
    if name.startswith("AnonymousName"):
        return "%s%s" % (where, _ANONYMOUS)
    return name


_ANONYMOUS = ": <anonymous>"


def _record_enum(sdk: Sdk, enum, where: str) -> None:
    """One parsed enum: its enumerators, under the name it goes by or where it sits."""
    values: List[Tuple[str, int]] = []
    for value in enum.values:
        number = _enum_number(value.value)
        if number is not None:
            values.append((value.name, number))
    if not values:
        return
    key = _enum_key(_typename_text(enum.typename), where)
    if key not in sdk.enums:
        sdk.enums[key] = values
    elif key.endswith(_ANONYMOUS):
        # The callback bases are one nameless enum to a line, and a payload's id is
        # written as one of them plus an offset: every one has to be kept, rather
        # than only the first that was read.
        sdk.enums[key].extend(values)


def _absorb(sdk: Sdk, header: str, path: str, parsed) -> None:
    """Everything one parsed header adds to the SDK."""
    namespace = parsed.namespace

    for typedef in namespace.typedefs:
        name = _typename_text(typedef.name)
        sdk.typedefs[name] = normalise(typedef.type)

    for variable in namespace.variables:
        number = _enum_number(variable.value)
        if number is not None:
            sdk.constants[_typename_text(variable.name)] = number

    for enum in namespace.enums:
        _record_enum(sdk, enum, header)
    for scope in namespace.classes:
        class_name = _typename_text(scope.class_decl.typename)
        for enum in getattr(scope, "enums", []):
            _record_enum(sdk, enum, class_name)

    classes: Dict[str, object] = {}
    for scope in namespace.classes:
        name = _typename_text(scope.class_decl.typename)
        classes[name] = scope
        sdk.aggregates.setdefault(name, _walk_scopes(scope, name))
    sdk.headers.append(list(classes.values()))

    # A version define belongs to a class in the same header.
    defines = _interface_version_defines(path)
    interface_names = [name for name in classes if name.startswith("ISteam")]
    for define, version in defines.items():
        owner = _interface_class_for(define, interface_names)
        if owner is None:
            sdk.unread.append("%s: %s names no class" % (header, define))
            continue
        sdk.interfaces.append(Interface(name=owner, version=version))

    # A struct that names its own callback id is a payload the backend can send. The
    # id is not resolved here: the base it adds to may be in a header not read yet.
    for name, scope in classes.items():
        for enum in getattr(scope, "enums", []):
            for value in enum.values:
                if value.name != "k_iCallback":
                    continue
                sdk.pending.append((header, name, value.value, sdk.aggregates[name]))


def _resolve_callbacks(sdk: Sdk) -> None:
    """Every payload's id, now that every header's numbers have been read."""
    for header, name, value, aggregate in sdk.pending:
        identifier = _callback_number(value, sdk)
        if identifier is None:
            sdk.unread.append("%s: %s has an unreadable k_iCallback" % (header, name))
            continue
        sdk.callbacks[name] = Callback(name=name, callback_id=identifier, aggregate=aggregate)


def _enum_number(node) -> Optional[int]:
    """An enumerator's value when it is a plain integer."""
    text = _flatten(node)
    if text is None:
        return None
    try:
        return int(text, 0)
    except ValueError:
        return None


def _callback_number(node, sdk: Sdk) -> Optional[int]:
    """`k_iSteamMatchmakingCallbacks + 13`, against the table this SDK declares.

    The bases live in steam_api_internal.h and are plain integers, so this
    evaluates the sum rather than compiling anything. A payload that is the first
    of its group - GSStatsReceived_t is - writes its base on its own.
    """
    text = _flatten(node)
    if text is None:
        return None
    # The SDK's own spelling, capital I and all: `k_ISteamParentalSettingsCallbacks`.
    match = re.match(r"^\s*(k_[A-Za-z_]+)\s*(?:([+-])\s*(\d+))?\s*$", text)
    if match is None:
        return _enum_number(node)
    base_name, sign, offset = match.groups()
    base = _enum_member(sdk, base_name)
    if base is None:
        return None
    if sign is None:
        return base
    return base + int(offset) if sign == "+" else base - int(offset)


def _enum_member(sdk: Sdk, name: str) -> Optional[int]:
    """The number an enumerator was given, in whichever enum declared it."""
    for values in sdk.enums.values():
        for member, number in values:
            if member == name:
                return number
    return None


def _read_flat_header(sdk: Sdk, options: ParserOptions, steam_dir: str) -> None:
    path = os.path.join(steam_dir, "steam_api_flat.h")
    if not os.path.exists(path):
        sdk.unread.append("steam_api_flat.h is missing: call names cannot be checked")
        return
    try:
        parsed = parse_file(path, options=options)
    except Exception as error:
        sdk.unread.append("steam_api_flat.h: %s" % _one_line(str(error)))
        return
    for function in parsed.namespace.functions:
        name = _typename_text(function.name)
        match = re.match(r"^SteamAPI_(ISteam[A-Za-z0-9]+)_", name)
        if match is None:
            continue
        interface = match.group(1)
        entry = FlatFunction(
            name=name,
            interface=interface,
            params=[normalise(parameter.type) for parameter in function.parameters],
            returns=normalise(function.return_type) if function.return_type else Ty("void"),
        )
        sdk.flat.setdefault(interface, []).append(entry)


# ---------------------------------------------------------------------------
#  A slot's type, as the file records it
# ---------------------------------------------------------------------------


class Mapper:
    """Turns a declared type into the kind (or the name) a row carries."""

    def __init__(self, sdk: Sdk):
        self.sdk = sdk
        self.value_types: Dict[str, str] = {}
        self.structures: Dict[str, Aggregate] = {}
        self.unmappable: List[str] = []
        self._find_value_classes()

    def _find_value_classes(self) -> None:
        """A class that is one 64-bit integer is a value: CSteamID, CGameID.

        The integer is not always a member of the class itself. CSteamID holds
        `union SteamID_t { SteamIDComponent_t m_comp; uint64 m_unAll64Bits; }
        m_steamid`, which is three names for the same eight bytes, and CGameID holds
        an unnamed one - so the single member is followed through the union it is,
        and what the file records is the name nearest the class: `m_steamid` rather
        than `m_unAll64Bits`.
        """
        for name, aggregate in self.sdk.aggregates.items():
            if aggregate.has_virtuals or len(aggregate.members) != 1:
                continue
            ty, member_name = aggregate.members[0]
            if _member_kind_of(self, self.sdk.canonical(ty)) != "uint64":
                member_name = self._through_union(aggregate, ty, member_name)
            if member_name:
                self.value_types[name] = member_name

    def _through_union(self, aggregate: Aggregate, ty: Ty, member_name: str) -> Optional[str]:
        """The member of a union that makes it eight bytes, if that is what it is."""
        resolved = self.sdk.canonical(ty)
        if resolved.form != "named":
            return None
        union = self.underlying(aggregate, resolved.name)
        if union is None or not union.is_union:
            return None
        for inner_ty, inner_name in union.members:
            if _member_kind_of(self, self.sdk.canonical(inner_ty)) == "uint64":
                return member_name or inner_name or inner_ty.name
        return None

    def is_enum(self, name: str) -> bool:
        if name in self.sdk.enums:
            return True
        # Some enum names are only ever declared, never enumerated in a header
        # this read (they come from a platform header): the SDK spells those
        # with a leading E, which is the rule the C++ side uses for a member.
        return len(name) > 1 and name[0] == "E" and name[1].isupper()

    def underlying(self, aggregate: Aggregate, name: str) -> Optional[Aggregate]:
        """The aggregate a member's type names: one at file scope, or one nested.

        A union written in place inside a structure - `union { SteamIDComponent_t
        m_steamid; } m_steamid;` - has no name to look up at file scope, so a
        member's type is resolved against the structure it was declared in as well.
        """
        found = self.sdk.aggregates.get(name)
        if found is not None:
            return found
        for inner in aggregate.aggregates:
            if inner.name == name:
                return inner
        return None

    def row_type(self, ty: Ty, where: str) -> Tuple[str, List[str]]:
        """The type a row writes, and the flags that go with it."""
        resolved = self.sdk.canonical(ty)

        if resolved.form == "void":
            return "void", []
        if resolved.form == "fundamental":
            kind = FUNDAMENTAL_KINDS.get(resolved.name)
            if kind is None:
                self.unmappable.append("%s: the fundamental '%s'" % (where, resolved.name))
                return "void", []
            return kind, []
        if resolved.form == "named":
            name = resolved.name
            if name in self.value_types:
                return name, []
            if name in self.sdk.aggregates and not self.sdk.aggregates[name].has_virtuals:
                # A structure is a shape the wire has no answer for: the file
                # records it, and says the call cannot be carried.
                self.structures.setdefault(name, self.sdk.aggregates[name])
                return name, ["unmarshalable"]
            if self.is_enum(name):
                return "enum", []
            self.unmappable.append("%s: the class '%s' is neither a kind nor a value" % (where, name))
            return "opaque_ptr", []
        if resolved.form == "function":
            return "opaque_ptr", ["unmarshalable"]
        if resolved.form == "pointer":
            return self._pointer_type(resolved, where)
        self.unmappable.append("%s: an unreadable declaration" % where)
        return "opaque_ptr", []

    def _pointer_type(self, ty: Ty, where: str) -> Tuple[str, List[str]]:
        pointee = ty.pointee
        if pointee is None:
            return "opaque_ptr", []
        inner = pointee
        const = ty.const or pointee.const
        # const char * is a string; the const is what says it is not a buffer.
        if inner.form == "fundamental" and inner.name == "char":
            if const:
                return "cstring", []
            return "opaque_ptr", ["unmarshalable"]
        if inner.form == "void":
            return "opaque_ptr", []
        if inner.form == "pointer":
            # A pointer to a pointer, or to a function pointer: nothing the wire
            # has an answer for.
            return "opaque_ptr", []
        if inner.form == "function":
            return "opaque_ptr", ["unmarshalable"]
        kind, flags = self.row_type(inner, where)
        if kind in ("opaque_ptr", "cstring"):
            return "opaque_ptr", []
        if const or "unmarshalable" in flags:
            # A pointer to a const something is a list the call reads, and a pointer
            # to a structure is a list either way: a list is what the wire has no
            # answer for, because the count that sizes it is the caller's business.
            # Both are written as the opaque pointer they are, with nothing to write
            # back - only a pointer to something that can travel is an out parameter.
            return "opaque_ptr", []
        return kind, ["out"]

    def member_type(self, ty: Ty, where: str, event: bool = False) -> Optional[Tuple[str, str]]:
        """A structure member as the file writes one: (kind, name).

        An enum member keeps its own name in a structure, which the generator
        declares as a type the game reads that member through; in a payload it is
        the int32 the wire carries, because an event is filled in field by field.
        """
        del where
        ty = self.sdk.canonical(ty)
        if ty.form == "fundamental":
            kind = FUNDAMENTAL_KINDS.get(ty.name)
            if kind in MEMBER_KINDS:
                return kind, ty.name
            return None
        if ty.form == "named":
            if self.is_enum(ty.name):
                return ("int32" if event else ty.name), ty.name
            if ty.name in self.value_types:
                # A value class is its own eight bytes on the wire, so a member of
                # one is written as the uint64 it is rather than by its name.
                return "uint64", ty.name
            return None
        if ty.form == "pointer":
            return None
        return None


def _member_kind_of(mapper: Mapper, ty: Ty) -> Optional[str]:
    """The kind a member's declared type lands on, for a size."""
    if ty.form == "fundamental":
        return FUNDAMENTAL_KINDS.get(ty.name)
    if ty.form == "named":
        if mapper.is_enum(ty.name):
            return "enum"
        if ty.name in mapper.value_types:
            return "uint64"
    return None


# ---------------------------------------------------------------------------
#  Sizes
# ---------------------------------------------------------------------------
#  The ABI a structure or a payload has, computed rather than compiled: members
#  are laid out at their natural alignment, capped at the pack the SDK packs its
#  callbacks at, and an empty payload is one byte - which is what a C++ compiler
#  says about a struct with no members, and what the file records.


class SizeOf:
    def __init__(self, mapper: Mapper):
        self.mapper = mapper
        self.cache: Dict[int, Optional[Tuple[int, int]]] = {}

    def member(self, ty: Ty) -> Optional[Tuple[int, int]]:
        """(size, alignment) of one field, or None when it cannot be known."""
        if ty.count is not None:
            element = Ty(ty.form, name=ty.name, const=ty.const, pointee=ty.pointee)
            inner = self.member(element)
            if inner is None:
                return None
            count = self.mapper.sdk.extent(ty.count)
            if count is None:
                return None
            return inner[0] * count, inner[1]
        resolved = self.mapper.sdk.canonical(ty)
        kind = _member_kind_of(self.mapper, resolved)
        if kind is None:
            if resolved.form == "pointer" or resolved.form == "function":
                return POINTER_SIZE, POINTER_SIZE
            return None
        _, size, align = MEMBER_KINDS.get(kind, (kind, ENUM_SIZE, ENUM_SIZE))
        return size, align

    def of(self, aggregate: Aggregate) -> Optional[Tuple[int, int]]:
        """The (size, alignment) of one aggregate, in whole bytes.

        Measured over the members the file *writes*, not over the declaration the
        SDK has: the two are not always the same shape, and the number the file
        records is asserted against the declaration the generator renders from
        those members. `SteamNetworkingIPAddr` is 24 bytes to a compiler - the
        anonymous union of its first sixteen bytes is aligned at eight - and 18 as
        the file spells it, which is the arm it kept.
        """
        key = id(aggregate)
        if key in self.cache:
            return self.cache[key]
        self.cache[key] = None  # a cycle is a type that cannot be sized
        measured = self._body(aggregate)
        self.cache[key] = measured
        return measured

    def _body(self, aggregate: Aggregate) -> Optional[Tuple[int, int]]:
        if not aggregate.members:
            # An empty payload is one byte, which is what a C++ compiler says about a
            # structure with no members and what the file records for a notification
            # with no fields. A union written with nothing but nested scopes in it is
            # as large as its largest one.
            if aggregate.aggregates:
                measured = [self.of(inner) for inner in aggregate.aggregates]
                if any(item is None for item in measured):
                    return None
                return max(item[0] for item in measured), max(item[1] for item in measured)
            return 1, 1
        members: List[Tuple[int, int]] = []
        for ty, _name in _flatten_members(self.mapper.sdk, self.mapper, aggregate):
            item = self._member_of(aggregate, ty)
            if item is None:
                return None
            members.append(item)
        if aggregate.is_union:
            # Every member of a union sits at zero; the union is as large as its
            # largest, and as aligned as the strictest of them.
            return max(size for size, _align in members), max(align for _size, align in members)
        offset = 0
        width = 1
        for size, align in members:
            align = min(align, CALLBACK_PACK)
            offset = _pad(offset, align)
            offset += size
            width = max(width, align)
        return _pad(offset, width), width

    def _member_of(self, aggregate: Aggregate, ty: Ty) -> Optional[Tuple[int, int]]:
        """(size, alignment) of a member, following what its type really is.

        A member the parser gave no name of its own - a union written in place - is
        found in the aggregate the member was declared in, and is as large as what it
        holds rather than as a pointer.
        """
        resolved = self.mapper.sdk.canonical(ty)
        if resolved.form == "named" and resolved.name not in self.mapper.value_types:
            if self.mapper.is_enum(resolved.name):
                return ENUM_SIZE, ENUM_SIZE
            nested = self.mapper.underlying(aggregate, resolved.name)
            if nested is None:
                return None
            return self.of(nested)
        return self.member(resolved)


def _pad(offset: int, alignment: int) -> int:
    if alignment <= 1:
        return offset
    remainder = offset % alignment
    return offset if remainder == 0 else offset + (alignment - remainder)


# ---------------------------------------------------------------------------
#  Building a layout out of one SDK
# ---------------------------------------------------------------------------


@dataclass
class Version:
    name: str
    version: str
    slots: List[list] = field(default_factory=list)
    generation: str = ""


@dataclass
class Layout:
    value_types: List[dict] = field(default_factory=list)
    structures: List[dict] = field(default_factory=list)
    events: List[dict] = field(default_factory=list)
    versions: List[Version] = field(default_factory=list)
    unmeasured: List[str] = field(default_factory=list)
    unread: List[str] = field(default_factory=list)


class Builder:
    """One SDK's interfaces as rows."""

    def __init__(self, sdk: Sdk, events: Sequence[str]):
        self.sdk = sdk
        self.mapper = Mapper(sdk)
        self.sizes = SizeOf(self.mapper)
        self.events_wanted = list(events)
        self.anomalies: List[str] = []
        self.versions: List[Version] = []

    def build(self) -> Layout:
        layout = Layout()
        for interface in self.sdk.interfaces:
            self.versions.append(self._version(interface))
        layout.versions = self.versions
        # A payload is declared from its own entry below, and a name that is one is
        # not declared anywhere else. The lists can overlap: a payload looks like a
        # value class when its one member is the `uint64` a value class is
        # (`SteamInputDeviceConnected_t` is exactly that), and like a structure when
        # a call names it as well (`SteamNetAuthenticationStatus_t` is). Both ways
        # the generator renders one C++ declaration per name, so writing two is a
        # redefinition - and the payload's entry is the one with the members in it.
        payloads = set()
        for name in self.events_wanted:
            if name in self.sdk.callbacks:
                payloads.add(name)
        for name, member in sorted(self.mapper.value_types.items()):
            if name in payloads:
                continue
            layout.value_types.append(
                {"name": name, "wire": "uint64", "size": 8, "member": member}
            )
        for name in sorted(self.mapper.structures):
            if name in payloads:
                continue
            entry = self._structure(self.mapper.structures[name])
            if entry is None:
                layout.unmeasured.append(name)
            else:
                layout.structures.append(entry)
        for name in self.events_wanted:
            callback = self.sdk.callbacks.get(name)
            if callback is None:
                continue
            entry = self._structure(callback.aggregate, event=True)
            if entry is None:
                layout.unmeasured.append(name)
                continue
            entry = dict(entry)
            entry["callback"] = callback.callback_id
            layout.events.append(entry)
        layout.unread = list(self.sdk.unread)
        layout.unmeasured.extend(self.anomalies)
        return layout

    # -- slots -------------------------------------------------------------

    def _version(self, interface: Interface) -> Version:
        version = Version(name=interface.name, version=interface.version, generation=self.sdk.label)
        scope = self._scope(interface.name)
        if scope is None:
            self.anomalies.append("%s: no class was read for it" % interface.version)
            return version

        for method in scope.methods:
            if not getattr(method, "virtual", False):
                continue
            if getattr(method, "static", False):
                continue
            method_name = _typename_text(method.name)
            protected = getattr(method, "access", "") in ("protected", "private")
            if method_name.startswith("~"):
                version.slots.append(self._destructor(interface, protected))
                continue
            version.slots.append(self._slot(interface, method, method_name, protected))

        if not version.slots:
            self.anomalies.append("%s: no virtuals were read" % interface.version)
        return version

    def _scope(self, class_name: str):
        return self._scopes.get(class_name)

    @property
    def _scopes(self) -> Dict[str, object]:
        if not hasattr(self, "_scope_cache"):
            self._scope_cache: Dict[str, object] = {}
            for header_scopes in self.sdk.headers:
                for scope in header_scopes:
                    self._scope_cache[_typename_text(scope.class_decl.typename)] = scope
        return self._scope_cache

    def _destructor(self, interface: Interface, protected: bool) -> list:
        row: list = ["~"]
        for function in self.sdk.flat.get(interface.name, []):
            if function.name == "SteamAPI_%s_Destruct%s" % (interface.name, interface.name):
                row.append({"call": function.name})
                if protected:
                    row[1]["private"] = True
                break
        return row

    def _slot(self, interface: Interface, method, method_name: str, protected: bool) -> list:
        where = "%s.%s" % (interface.version, method_name)
        returns, notes = self._returns(method, interface, where)
        params: List[list] = []
        for index, parameter in enumerate(method.parameters):
            params.append(self._param(interface, method_name, parameter, where, index))
        note: Dict[str, object] = {}
        if protected:
            note["call"] = "%s::%s" % (interface.name, method_name)
            note["private"] = True
        elif notes:
            note.update(notes)
        else:
            flat = self._flat_name(interface, method, method_name)
            if flat is not None and flat != "SteamAPI_%s_%s" % (interface.name, method_name):
                note["call"] = flat
        row: list = [method_name, returns]
        # The parameters slot is written even when there are none, if a note
        # follows it: a row is read by position, so an omitted slot would put the
        # note where the parameters belong.
        if params or note:
            row.append(params)
        if note:
            row.append(note)
        return row

    def _returns(self, method, interface: Interface, where: str) -> Tuple[str, Dict[str, object]]:
        if method.return_type is None:
            return "void", {}
        kind, flags = self.mapper.row_type(normalise(method.return_type), where)
        notes: Dict[str, object] = {}
        if "unmarshalable" in flags:
            notes["unmarshalable"] = True
        del interface
        return kind, notes

    def _param(self, interface: Interface, method_name: str, parameter, where: str, index: int) -> list:
        # A parameter nobody named - `void DEPRECATED_Foo( void (*)() )` - is named
        # by where it sits, which is what the file records for one.
        name = parameter.name or "arg%d" % index
        ty = normalise(parameter.type)
        key = (interface.name, method_name, name)
        if key in BYTE_BUFFERS:
            kind, length = BYTE_BUFFERS[key]
            return [name, kind, length]
        kind, flags = self.mapper.row_type(ty, "%s.%s" % (where, name))
        row = [name, kind]
        row.extend(flags)
        return row

    def _flat_name(self, interface: Interface, method, method_name: str) -> Optional[str]:
        """The flat name a call travels under, from steam_api_flat.h.

        The method's own name is what usually applies; where an SDK named an
        overload differently - two `GetStat`s become GetStatInt32 and GetStatFloat -
        the signature is what finds the right one.
        """
        candidates = self.sdk.flat.get(interface.name)
        if not candidates:
            return None
        derived = "SteamAPI_%s_%s" % (interface.name, method_name)
        wanted = self._param_kinds(method)
        for candidate in candidates:
            if candidate.name == derived:
                return derived
        for candidate in candidates:
            if len(candidate.params) != len(method.parameters) + 1:
                continue
            # An overload's flat name is the method's own name with the argument's
            # type spelled on the end - GetStatInt32 for GetStat - so a name that
            # does not begin with the method's is a different method that happens to
            # take the same arguments, and not the name this call travels under.
            if not candidate.name.startswith(derived):
                continue
            if self._flat_param_kinds(candidate) == wanted:
                return candidate.name
        self.anomalies.append("%s: no flat name matches" % derived)
        return None

    def _param_kinds(self, method) -> List[str]:
        kinds: List[str] = []
        for parameter in method.parameters:
            kind, _flags = self.mapper.row_type(normalise(parameter.type), "signature")
            kinds.append(kind)
        return kinds

    def _flat_param_kinds(self, function: FlatFunction) -> List[str]:
        kinds: List[str] = []
        for parameter in function.params[1:]:
            kind, _flags = self.mapper.row_type(parameter, "signature")
            kinds.append(kind)
        return kinds

    # -- structures and payloads -------------------------------------------

    def _structure(self, aggregate: Aggregate, event: bool = False) -> Optional[dict]:
        members: List[list] = []
        for ty, name in _flatten_members(self.sdk, self.mapper, aggregate):
            mapped = self.mapper.member_type(ty, aggregate.name, event=event)
            if mapped is None:
                return None
            kind, _spelling = mapped
            declared = kind if kind in MEMBER_KINDS else kind
            written = name
            if ty.count is not None:
                extent = self.sdk.extent(ty.count)
                if extent is None:
                    # The size of this member is not knowable from here, and a name
                    # the generated file cannot look up is not a declaration: the
                    # structure is reported instead of written wrong.
                    return None
                written = "%s[%d]" % (name, extent)
            members.append([declared, written])
        measured = self.sizes.of(aggregate)
        if measured is None:
            return None
        return {"name": aggregate.name, "members": members, "size": measured[0]}


def _flatten_members(sdk: Sdk, mapper: Mapper, aggregate: Aggregate) -> List[Tuple[Ty, str]]:
    """A structure's members, with an anonymous union laid out as its largest.

    `SteamIPAddress_t` is a union with three views of the same sixteen bytes, and
    a union that is not named has no name of its own in a declaration; the file
    records the largest member (ties going to the first written), which is what
    makes its size right.
    """
    members: List[Tuple[Ty, str]] = []
    for ty, name in aggregate.members:
        nested = _anonymous_of(sdk, aggregate, ty) if not name else None
        if nested is not None:
            best: Optional[Tuple[Ty, str]] = None
            best_size = -1
            for inner_ty, inner_name in _flatten_members(sdk, mapper, nested):
                measured = SizeOf(mapper).member(inner_ty)
                if measured is None:
                    continue
                if measured[0] > best_size:
                    best = (inner_ty, inner_name)
                    best_size = measured[0]
            if best is not None:
                members.append(best)
                continue
        members.append((ty, name))
    return members


def _anonymous_of(sdk: Sdk, aggregate: Aggregate, ty: Ty) -> Optional[Aggregate]:
    """The nested aggregate a member with no name of its own declares."""
    del sdk
    if ty.form == "named" and ty.name.startswith("AnonymousName"):
        return aggregate.aggregates[0] if aggregate.aggregates else None
    return None


# ---------------------------------------------------------------------------
#  Merging several SDKs into one file
# ---------------------------------------------------------------------------


def merge(layouts: Sequence[Layout]) -> Layout:
    """Oldest first, newest winning, anything only an older one had appended.

    This is the rule the file in the tree was built with: a version string is
    meant to name one ABI, so two generations that declare the same one are one
    entry - the newer one's slots, in the newer one's order, with a slot only the
    older generation had appended at the end, because that is where a version
    string grows.
    """
    merged = Layout()
    if not layouts:
        return merged
    merged.value_types = list(layouts[-1].value_types)
    merged.structures = list(layouts[-1].structures)
    merged.events = list(layouts[-1].events)
    merged.unmeasured = sorted({name for layout in layouts for name in layout.unmeasured})
    merged.unread = sorted({line for layout in layouts for line in layout.unread})

    by_version: Dict[str, Version] = {}
    order: List[str] = []
    for layout in layouts:
        for version in layout.versions:
            existing = by_version.get(version.version)
            if existing is None:
                by_version[version.version] = Version(
                    name=version.name,
                    version=version.version,
                    slots=[list(row) for row in version.slots],
                    generation=version.generation,
                )
                order.append(version.version)
                continue
            # The newer generation is the winner, and its rows replace the older
            # ones slot by slot - which is also what settles a slot's own name. A
            # slot only the older one had sits past the end of the newer's rows and
            # is appended, because that is where a version string grows.
            existing.slots = [list(row) for row in version.slots] + existing.slots[len(version.slots):]
            existing.generation = version.generation
    merged.versions = [by_version[key] for key in order]
    merged.value_types = _newest_by_name([layout.value_types for layout in layouts])
    merged.structures = _newest_by_name([layout.structures for layout in layouts])
    merged.events = _newest_by_name([layout.events for layout in layouts])
    return merged


def _newest_by_name(groups: Sequence[List[dict]]) -> List[dict]:
    """One entry per name, from the newest SDK that had it, order kept."""
    out: List[dict] = []
    seen: Dict[str, int] = {}
    for group in groups:
        for entry in group:
            name = entry["name"]
            if name in seen:
                out[seen[name]] = entry
                continue
            seen[name] = len(out)
            out.append(entry)
    return out


# ---------------------------------------------------------------------------
#  Writing a layout
# ---------------------------------------------------------------------------


def render(layout: Layout) -> str:
    """The file, one row to a line, the way the one in the tree is written."""
    lines = ["{"]
    lines.append("  \"source\": \"Steamworks SDK headers, read by tools/steamworks_sdk_import.py\",")
    lines.append(
        '  "format": "the same shape gen/steam_interfaces.json uses: one row per slot, in vtable order",'
    )
    lines.append('  "value_types": %s,' % _rows(layout.value_types, 2))
    lines.append('  "structures": %s,' % _rows(layout.structures, 2))
    lines.append('  "events": %s,' % _rows(layout.events, 2))

    blocks: List[str] = []
    for version in sorted(layout.versions, key=lambda item: (item.name, _version_key(item.version))):
        slots = ",\n".join(
            _indent(2, json.dumps(row, separators=(", ", ": "))) for row in version.slots
        )
        blocks.append(
            "    {\n"
            '      "name": %s,\n      "version": %s,\n      "slots": [\n%s\n      ]\n    }'
            % (json.dumps(version.name), json.dumps(version.version), _indent(3, slots))
        )
    lines.append('  "interfaces": [\n%s\n  ]' % ",\n".join(blocks))
    lines.append("}")
    return "\n".join(lines) + "\n"


def _version_key(version: str) -> Tuple[str, Tuple[int, ...]]:
    head = re.sub(r"\d+$", "", version)
    digits = re.findall(r"\d+", version)
    return head, tuple(int(digit) for digit in digits)


def _rows(entries: Sequence[dict], depth: int) -> str:
    if not entries:
        return "[]"
    body = ",\n".join(_indent(depth + 1, json.dumps(entry, separators=(", ", ": "))) for entry in entries)
    return "[\n%s\n%s]" % (body, "  " * depth)


def _indent(levels: int, text: str) -> str:
    pad = "  " * levels
    return "\n".join(pad + line for line in text.split("\n"))


# ---------------------------------------------------------------------------
#  Comparing two layouts
# ---------------------------------------------------------------------------


@dataclass
class Comparison:
    lines: List[str] = field(default_factory=list)
    counts: Dict[str, int] = field(default_factory=dict)

    def say(self, kind: str, text: str) -> None:
        self.counts[kind] = self.counts.get(kind, 0) + 1
        self.lines.append("[%s] %s" % (kind, text))

    @property
    def differences(self) -> int:
        """How many things are not the same, which is what the exit code is.

        The `identical-` kinds are counted too, because how much two files agree
        about belongs next to how much they do not - but they are not
        differences, and a check that reads this number has to come out zero for
        two files that say the same thing.
        """
        return sum(
            count for kind, count in self.counts.items() if not kind.startswith("identical-")
        )


def layout_from_document(document) -> Layout:
    """A layout out of the JSON one is written as."""
    layout = Layout()
    layout.value_types = [dict(entry) for entry in document.get("value_types", [])]
    layout.structures = [dict(entry) for entry in document.get("structures", [])]
    layout.events = [dict(entry) for entry in document.get("events", [])]
    for entry in document.get("interfaces", []):
        layout.versions.append(
            Version(
                name=entry["name"],
                version=entry["version"],
                slots=[list(row) for row in entry["slots"]],
                generation="file",
            )
        )
    return layout


def load_layout(path: str) -> Layout:
    return layout_from_document(json.loads(read_text(path)))


def compare(ours: Layout, theirs: Layout) -> Comparison:
    """What the two say differently, with the categories that matter kept apart.

    A *renamed* slot is the documented rule at work - two generations under one
    version string, the newer one's name on the slot - and not a disagreement. A
    slot whose signature differs is a disagreement, and it is the thing this whole
    exercise exists to find.
    """
    result = Comparison()
    our_versions = {version.version: version for version in ours.versions}
    their_versions = {version.version: version for version in theirs.versions}

    for version in sorted(set(our_versions) - set(their_versions)):
        result.say("only-from-sdk", "%s (%s): not in the file" % (version, our_versions[version].name))
    for version in sorted(set(their_versions) - set(our_versions)):
        result.say("only-in-file", "%s (%s): no SDK here declares it" % (version, their_versions[version].name))

    for version in sorted(set(our_versions) & set(their_versions)):
        mine = our_versions[version]
        theirs_version = their_versions[version]
        if mine.name != theirs_version.name:
            result.say(
                "class-name", "%s: the file says %s, the SDK class is %s" % (version, theirs_version.name, mine.name)
            )
        shared = min(len(mine.slots), len(theirs_version.slots))
        for index in range(shared):
            left = mine.slots[index]
            right = theirs_version.slots[index]
            if left == right:
                result.counts["identical-slot"] = result.counts.get("identical-slot", 0) + 1
                continue
            if _renamed(left, right):
                result.say(
                    "renamed-slot",
                    "%s slot %d: the SDK calls it %s, the file says %s (same signature)"
                    % (version, index, left[0], right[0]),
                )
                continue
            result.say(
                "slot-differs",
                "%s slot %d:\n      sdk:  %s\n      file: %s"
                % (version, index, json.dumps(left), json.dumps(right)),
            )
        for index in range(shared, len(mine.slots)):
            result.say(
                "slot-only-from-sdk",
                "%s slot %d: the SDK has %s where the file stops" % (version, index, json.dumps(mine.slots[index][0])),
            )
        for index in range(shared, len(theirs_version.slots)):
            result.say(
                "slot-only-in-file",
                "%s slot %d: the file has %s where the SDK stops" % (version, index, json.dumps(theirs_version.slots[index][0])),
            )

    _compare_named(result, "value type", ours.value_types, theirs.value_types)
    _compare_named(result, "structure", ours.structures, theirs.structures)
    _compare_named(result, "event", ours.events, theirs.events)
    for name in sorted({entry["name"] for entry in ours.events} - {entry["name"] for entry in theirs.events}):
        result.counts["event-not-in-file"] = result.counts.get("event-not-in-file", 0) + 1
    return result


def _renamed(left: list, right: list) -> bool:
    """Same signature, different name - which is the rule, not a disagreement."""
    return left[0] != right[0] and left[1:] == right[1:]


def _compare_named(result: Comparison, what: str, ours: Sequence[dict], theirs: Sequence[dict]) -> None:
    mine = {entry["name"]: entry for entry in ours}
    other = {entry["name"]: entry for entry in theirs}
    for name in sorted(set(mine) - set(other)):
        result.say("%s-only-from-sdk" % what.replace(" ", "-"), "%s: not in the file" % name)
    for name in sorted(set(other) - set(mine)):
        result.say("%s-only-in-file" % what.replace(" ", "-"), "%s: no SDK here declares it" % name)
    for name in sorted(set(mine) & set(other)):
        if mine[name] == other[name]:
            result.counts["identical-%s" % what.replace(" ", "-")] = (
                result.counts.get("identical-%s" % what.replace(" ", "-"), 0) + 1
            )
            continue
        result.say(
            "%s-differs" % what.replace(" ", "-"),
            "%s (%s):\n      sdk:  %s\n      file: %s"
            % (name, what, json.dumps(mine[name]), json.dumps(other[name])),
        )


# ---------------------------------------------------------------------------
#  What a game asks for
# ---------------------------------------------------------------------------
#  A game does not import an interface: it asks for a version string and calls
#  what it gets back, so a string this file does not carry is a game handed null
#  where it expected an object. An SDK declares one generation per interface, and
#  a game older than every SDK here asks for a generation none of them has - the
#  Spacewar this harness runs against asks for twenty-one strings, and the SDKs
#  that were to hand when the file was last built declare thirteen of them. That
#  is worth knowing before a run rather than after one.
#
#  The strings come out of a binary without reading its code: the shapes Valve
#  spells them in, whole, and - for the families the layouts already carry - the
#  family's own spelling followed by digits. The second rule is what catches a
#  game built against the generation *before* the one here, which is the case that
#  matters; a family nothing here declares is not looked for, because a pattern
#  loose enough to find it would find text that is not a version string.

VERSION_SHAPE = re.compile(rb"STEAM[A-Z0-9_]*_INTERFACE_V[A-Z0-9_]*")


def wanted_strings(path: str, families: Sequence[str]) -> List[str]:
    """The version strings one binary asks for, as far as they can be recognised."""
    with open(path, "rb") as handle:
        data = handle.read()
    found = {match.group(0).decode("ascii") for match in VERSION_SHAPE.finditer(data)}
    for family in families:
        head = family.rstrip("0123456789")
        if not head:
            continue
        pattern = re.compile(re.escape(head).encode("ascii") + rb"[0-9]+")
        found.update(match.group(0).decode("ascii") for match in pattern.finditer(data))
    return sorted(found)


def report_wanted(paths: Sequence[str], carried: Sequence[str], what: str) -> None:
    """Say what a game asks for, and what it will not get."""
    have = set(carried)
    for path in paths:
        try:
            asked = wanted_strings(path, sorted(have))
        except OSError as error:
            sys.stderr.write(
                "steamworks_sdk_import: %s could not be read (%s)\n"
                % (path, _one_line(str(error)))
            )
            continue
        if not asked:
            print("%s: no version string this reader recognises" % path)
            continue
        missing = [version for version in asked if version not in have]
        print("%s asks for %d version string(s); %d are in %s"
              % (path, len(asked), len(asked) - len(missing), what))
        if missing:
            print("   not carried: %s" % ", ".join(missing))


# ---------------------------------------------------------------------------
#  Finding an SDK
# ---------------------------------------------------------------------------


def resolve_steam_dir(path: str) -> Optional[str]:
    """The `steam` header directory inside whatever was pointed at."""
    candidates = [
        path,
        os.path.join(path, "public", "steam"),
        os.path.join(path, "sdk", "public", "steam"),
        os.path.join(path, "steam"),
    ]
    for candidate in candidates:
        if os.path.exists(os.path.join(candidate, "steam_api_flat.h")):
            return os.path.abspath(candidate)
    return None


def infer_label(path: str) -> Optional[str]:
    """A version out of the path, which is what orders two SDKs."""
    match = re.search(r"[Ss]teamv(\d{2,3})", path) or re.search(r"sdk[ _-]?(\d{3})", path)
    if match is None:
        return None
    digits = match.group(1)
    return "%s.%s" % (digits[0], digits[1:]) if len(digits) == 3 else digits


def label_key(label: str) -> Tuple[int, ...]:
    return tuple(int(part) for part in re.findall(r"\d+", label))


# ---------------------------------------------------------------------------
#  The command line
# ---------------------------------------------------------------------------


def parse_arguments(argv: Sequence[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        prog="steamworks_sdk_import",
        description=(
            "Read one or more installed Steamworks SDKs and write the interface layouts "
            "gen/steam_interfaces.json holds, or report how the two differ. Nothing is "
            "written unless --out is given, and never into gen/."
        ),
        epilog=(
            "a --sdk may carry a version of its own (--sdk 1.51=C:/sdk), which is what "
            "orders several of them; without one, the order they are given in is oldest first"
        ),
    )
    parser.add_argument(
        "--sdk",
        action="append",
        default=[],
        metavar="[VERSION=]PATH",
        help="an SDK, or a directory holding one (repeatable)",
    )
    parser.add_argument("--out", metavar="FILE", help="write the layouts, oldest SDK first")
    parser.add_argument("--diff", metavar="FILE", help="compare against a layout file")
    parser.add_argument("--list", action="store_true", help="print what was read, then stop")
    parser.add_argument(
        "--wanted",
        action="append",
        default=[],
        metavar="FILE",
        help="a game or a library to read version strings out of, and report the ones the "
        "layouts in play do not carry (repeatable)",
    )
    parser.add_argument(
        "--all-events",
        action="store_true",
        help="every payload the SDK declares, not only the ones the compared file names",
    )
    parser.add_argument("--selftest", action="store_true", help="check the reader without an SDK")
    parser.add_argument("--quiet", action="store_true", help="only the summary and the differences")
    return parser.parse_args(argv)


def emit(layout: Layout, arguments: argparse.Namespace) -> int:
    """Write the file if one was asked for, and compare it if one was named."""
    if arguments.out:
        with open(arguments.out, "w", encoding="utf-8", newline="\n") as handle:
            handle.write(render(layout))
        print("wrote %s" % arguments.out)

    if arguments.diff:
        theirs = load_layout(arguments.diff)
        comparison = compare(layout, theirs)
        for line in sorted(comparison.lines):
            print(line)
        print()
        print("compared against %s" % arguments.diff)
        for kind, count in sorted(comparison.counts.items()):
            print("  %-24s %d" % (kind, count))
        print("  %-24s %d" % ("differences", comparison.differences))
        return 0 if comparison.differences == 0 else 1

    return 0


def main(argv: Sequence[str]) -> int:
    arguments = parse_arguments(argv)
    if arguments.selftest:
        return selftest()

    if not arguments.sdk:
        sys.stderr.write("steamworks_sdk_import: at least one --sdk is needed (or --selftest)\n")
        return 2

    if arguments.diff and not arguments.all_events:
        events = [entry["name"] for entry in load_layout(arguments.diff).events]
    else:
        events = []

    sdks: List[Sdk] = []
    for specification in arguments.sdk:
        label, _, path = specification.partition("=")
        if not path:
            label, path = "", specification
        directory = resolve_steam_dir(path)
        if directory is None:
            sys.stderr.write("steamworks_sdk_import: no steam_api_flat.h under '%s'\n" % path)
            return 2
        label = label or infer_label(path) or os.path.basename(os.path.abspath(path))
        sdks.append(parse_sdk(label, directory))

    labelled = all(infer_label(sdk.path) or re.match(r"^\d", sdk.label) for sdk in sdks)
    if labelled:
        sdks.sort(key=lambda sdk: label_key(sdk.label))

    layouts: List[Layout] = []
    for sdk in sdks:
        builder = Builder(sdk, events if events else list(sdk.callbacks))
        layouts.append(builder.build())
    merged = merge(layouts)

    if not arguments.quiet:
        for sdk in sdks:
            print(
                "read %-8s %s: %d versions, %d payloads, %d structures%s"
                % (
                    sdk.label,
                    sdk.path,
                    len(sdk.interfaces),
                    len(sdk.callbacks),
                    len([name for name in sdk.aggregates.values() if not name.has_virtuals]),
                    "" if not sdk.unread else "  (%d unread)" % len(sdk.unread),
                )
            )
        for line in merged.unread:
            print("  unread: %s" % line)
        for line in merged.unmeasured:
            print("  unmeasured: %s" % line)

    if arguments.wanted:
        # The file in play is the one being compared with if there is one - that is
        # the file the games would run against - and otherwise the one this run
        # would write.
        against = load_layout(arguments.diff) if arguments.diff else merged
        report_wanted(
            arguments.wanted,
            [version.version for version in against.versions],
            arguments.diff if arguments.diff else "these SDKs' layouts",
        )
        print()

    if arguments.list:
        for version in sorted(merged.versions, key=lambda item: (item.name, _version_key(item.version))):
            print("%-40s %-34s %3d slots" % (version.name, version.version, len(version.slots)))
        return 0

    return emit(merged, arguments)

# ---------------------------------------------------------------------------
#  Checking the reader without an SDK
# ---------------------------------------------------------------------------
#  Headers written here rather than taken from a Steamworks SDK: a version define,
#  a class with a vtable worth reading (a destructor, a protected method, an out
#  parameter, two overloads, a payload), a callback and a structure with an
#  anonymous union. The SDK itself is not redistributable, so the check that runs
#  everywhere uses text that is not Valve's.


SELFTEST_HEADERS = {
    "steam_api_common.h": """
#ifndef STEAM_API_COMMON_H
#define STEAM_API_COMMON_H
#define STEAM_PRIVATE_API( ... ) protected: __VA_ARGS__ public:
#define S_API extern "C"
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef int int32;
extern "C" typedef void ( *SteamAPIWarningMessageHook_t )( int, const char * );
#endif
""",
    "steamtypes.h": """
#ifndef STEAMTYPES_H
#define STEAMTYPES_H
#include "steam_api_common.h"
enum ESteamIPType { k_ESteamIPTypeIPv4 = 0 };
struct SteamIPAddress_t
{
    union {
        uint32 m_unIPv4;
        unsigned char m_rgubIPv6[16];
    };
    ESteamIPType m_eType;
};
#endif
""",
    "isteamtest.h": """
#ifndef ISTEAMTEST_H
#define ISTEAMTEST_H
#include "steam_api_common.h"
#include "steamtypes.h"
enum ESteamTestResult { k_ESteamTestResultOK = 1 };
// An array extent written as a name: the file has to carry the number, because
// the declaration rendered from it is compiled by something that has never read
// this header.
const uint32 k_unSteamTestMaxResults = 4;
struct SteamTestTable_t
{
    uint32 m_nCount;
    uint64 m_rgIds[ k_unSteamTestMaxResults ];
};
// A structure whose first member is an anonymous union: the arm the file keeps is
// the one that was written first, so the shape it sizes is not the shape the SDK
// has - the union's own alignment is not the arm's.
struct SteamTestInside_t
{
    union {
        unsigned char m_bytes[16];
        uint64 m_words[2];
    };
    unsigned short m_port;
};
struct SteamTestDone_t
{
    enum { k_iCallback = k_iSteamTestCallbacks + 7 };
    ESteamTestResult m_eResult;
    uint64 m_ulValue;
};
class ISteamTest
{
public:
    virtual ~ISteamTest() { }
    virtual uint64 GetTestID() = 0;
    virtual bool GetStat( const char *pchName, int32 *pData ) = 0;
    virtual bool GetStat( const char *pchName, float *pData ) = 0;
    virtual SteamIPAddress_t GetAddress() = 0;
    // An SDK of the 1.39 era annotates a method in headers that do not include the
    // half of the chain defining the annotation, and a reader that leaves it in the
    // declaration stops at the `virtual` after it - which is what 1.39's
    // `isteamapps.h` does in the wild.
    CALL_RESULT( SteamTestDone_t )
    virtual bool GetTable( SteamTestTable_t *pTable ) = 0;
    virtual SteamTestInside_t GetInside() = 0;
    virtual void FillBuffer( char *pchBuffer, int cubBuffer ) = 0;
    STEAM_PRIVATE_API( virtual void RunFrame() = 0; )
};
#define STEAMTEST_INTERFACE_VERSION "SteamTest001"
#endif
""",
    "steam_api_flat.h": """
#ifndef STEAM_API_FLAT_H
#define STEAM_API_FLAT_H
#include "steam_api_common.h"
#include "isteamtest.h"
S_API uint64 SteamAPI_ISteamTest_GetTestID( ISteamTest* self );
S_API bool SteamAPI_ISteamTest_GetStatInt32( ISteamTest* self, const char * pchName, int32 * pData );
S_API bool SteamAPI_ISteamTest_GetStatFloat( ISteamTest* self, const char * pchName, float * pData );
S_API SteamIPAddress_t SteamAPI_ISteamTest_GetAddress( ISteamTest* self );
S_API bool SteamAPI_ISteamTest_GetTable( ISteamTest* self, SteamTestTable_t * pTable );
S_API SteamTestInside_t SteamAPI_ISteamTest_GetInside( ISteamTest* self );
S_API void SteamAPI_ISteamTest_FillBuffer( ISteamTest* self, char * pchBuffer, int cubBuffer );
S_API void SteamAPI_ISteamTest_DestructISteamTest( ISteamTest* self );
#endif
""",
    "steam_api_internal.h": """
#ifndef STEAM_API_INTERNAL_H
#define STEAM_API_INTERNAL_H
enum { k_iSteamTestCallbacks = 300 };
#endif
""",
}


def selftest() -> int:
    import tempfile

    failures: List[str] = []
    with tempfile.TemporaryDirectory() as root:
        steam_dir = os.path.join(root, "sdk", "public", "steam")
        os.makedirs(steam_dir)
        for name, text in SELFTEST_HEADERS.items():
            with open(os.path.join(steam_dir, name), "w", encoding="utf-8") as handle:
                handle.write(text)

        sdk = parse_sdk("1.0", steam_dir)
        builder = Builder(sdk, ["SteamTestDone_t"])
        layout = builder.build()
        versions = {version.version: version for version in layout.versions}

        if "SteamTest001" not in versions:
            failures.append("the version define in isteamtest.h was not read")
        else:
            slots = versions["SteamTest001"].slots
            expected = [
                ["~", {"call": "SteamAPI_ISteamTest_DestructISteamTest"}],
                ["GetTestID", "uint64"],
                ["GetStat", "bool", [["pchName", "cstring"], ["pData", "int32", "out"]], {"call": "SteamAPI_ISteamTest_GetStatInt32"}],
                ["GetStat", "bool", [["pchName", "cstring"], ["pData", "float", "out"]], {"call": "SteamAPI_ISteamTest_GetStatFloat"}],
                ["GetAddress", "SteamIPAddress_t", [], {"unmarshalable": True}],
                ["GetTable", "bool", [["pTable", "opaque_ptr"]]],
                ["GetInside", "SteamTestInside_t", [], {"unmarshalable": True}],
                ["FillBuffer", "void", [["pchBuffer", "opaque_ptr", "unmarshalable"], ["cubBuffer", "int32"]]],
                ["RunFrame", "void", [], {"call": "ISteamTest::RunFrame", "private": True}],
            ]
            if slots != expected:
                failures.append(
                    "the slots read differently:\n    read:     %s\n    expected: %s"
                    % (json.dumps(slots), json.dumps(expected))
                )

        structures = {entry["name"]: entry for entry in layout.structures}
        if "SteamIPAddress_t" not in structures:
            failures.append("SteamIPAddress_t was not read as a structure")
        elif structures["SteamIPAddress_t"] != {
            "name": "SteamIPAddress_t",
            "members": [["uint8", "m_rgubIPv6[16]"], ["ESteamIPType", "m_eType"]],
            "size": 20,
        }:
            failures.append("SteamIPAddress_t read as %s" % json.dumps(structures["SteamIPAddress_t"]))

        # An extent the header spells as a name is a number in the file, and one
        # whose element is a typedef is as many bytes as the array really is.
        if structures.get("SteamTestTable_t") != {
            "name": "SteamTestTable_t",
            "members": [["uint32", "m_nCount"], ["uint64", "m_rgIds[4]"]],
            "size": 40,
        }:
            failures.append("SteamTestTable_t read as %s" % json.dumps(structures.get("SteamTestTable_t")))

        # And the size is the one the written shape has, not the one the SDK's
        # declaration has: the arm kept for the union is aligned at one, so this
        # is 18 bytes here and 24 to a compiler that saw the `uint64` arm.
        if structures.get("SteamTestInside_t") != {
            "name": "SteamTestInside_t",
            "members": [["uint8", "m_bytes[16]"], ["uint16", "m_port"]],
            "size": 18,
        }:
            failures.append("SteamTestInside_t read as %s" % json.dumps(structures.get("SteamTestInside_t")))

        events = {entry["name"]: entry for entry in layout.events}
        if "SteamTestDone_t" not in events:
            failures.append("the payload in isteamtest.h was not read")
        else:
            entry = events["SteamTestDone_t"]
            if entry["callback"] != 307:
                failures.append("the payload's callback id came out as %s, not 307" % entry["callback"])
            if entry["size"] != 16:
                failures.append("the payload's size came out as %s, not 16" % entry["size"])

        # Two SDKs, the same version string, the newer one renamed and the older
        # one with a slot the newer does not have: the merge has to keep the new
        # name and append the old slot.
        older = Layout(
            versions=[
                Version(
                    name="ISteamTest",
                    version="SteamTest001",
                    slots=[["Old", "void"], ["Gone", "void"]],
                    generation="0.9",
                )
            ]
        )
        newer = Layout(
            versions=[
                Version(
                    name="ISteamTest",
                    version="SteamTest001",
                    slots=[["New", "void"]],
                    generation="1.0",
                )
            ]
        )
        merged = merge([older, newer])
        if merged.versions[0].slots != [["New", "void"], ["Gone", "void"]]:
            failures.append("the merge came out as %s" % json.dumps(merged.versions[0].slots))

        # And the comparison has to tell the documented rule apart from a
        # disagreement.
        left = Layout(versions=[Version(name="ISteamTest", version="V", slots=[["A", "uint32", [["p", "int32", "out"]]]])])
        right = Layout(versions=[Version(name="ISteamTest", version="V", slots=[["B", "uint32", [["p", "int32", "out"]]]])])
        if compare(left, right).counts.get("renamed-slot") != 1:
            failures.append("a rename was not read as a rename")
        right.versions[0].slots = [["A", "uint32", [["p", "uint64", "out"]]]]
        if compare(left, right).counts.get("slot-differs") != 1:
            failures.append("a signature difference was not read as one")

        # The rendered file has to be the shape the generator reads.
        document = json.loads(render(merged))
        for key in ("value_types", "structures", "events", "interfaces"):
            if key not in document:
                failures.append("the rendered file has no '%s'" % key)

        # What a game asks for is read out of the binary, and the version strings
        # this file does not carry are the ones to worry about: the family rules
        # are what catch a game built against the generation before the one here.
        game_path = os.path.join(root, "game.bin")
        with open(game_path, "wb") as handle:
            handle.write(
                b"\x00\x01SteamTest001\x00"          # carried
                b"\x00SteamTest002\x00"              # the same family, one on
                b"\x00SteamOther004\x00"             # a family this file has none of
                b"\x00STEAMTEST_INTERFACE_VERSION001\x00"
            )
        asked = wanted_strings(game_path, ["SteamTest001"])
        if asked != ["STEAMTEST_INTERFACE_VERSION001", "SteamTest001", "SteamTest002"]:
            failures.append("the wanted strings came out as %s" % json.dumps(asked))

    if failures:
        for failure in failures:
            sys.stderr.write("selftest: %s\n" % failure)
        sys.stderr.write("selftest: %d checks failed\n" % len(failures))
        return 1
    print("selftest: the reader, the merge, the comparison, the wanted check and the writer "
          "all check out")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
