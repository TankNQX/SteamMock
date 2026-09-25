#include "bridge/idl.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <string>
#include <utility>
#include <vector>

namespace steammock {
namespace {

// ---------------------------------------------------------------------------
//  The type table.
// ---------------------------------------------------------------------------
//  Adding a kind of parameter means adding a row here and nothing else: the
//  generator writes one function per IDL entry and these are the only places a
//  type name is turned into C++.

struct TypeInfo {
    const char* idl;
    const char* cpp;
    const char* arg_helper;
    const char* return_default;
    const char* reply_expr;
    const char* out_expr;
};

constexpr TypeInfo kTypes[] = {
    {"bool", "bool", "arg_bool", "false", "steammock::reply_bool(reply)",
     "static_cast<bool>(steammock::as_bool(*value))"},
    {"int32", "std::int32_t", "arg_int", "0",
     "static_cast<std::int32_t>(steammock::reply_int(reply))",
     "static_cast<std::int32_t>(steammock::as_int64(*value))"},
    {"uint32", "std::uint32_t", "arg_uint", "0",
     "static_cast<std::uint32_t>(steammock::reply_uint(reply))",
     "static_cast<std::uint32_t>(steammock::as_uint64(*value))"},
    // A port number is a uint16 in the real headers and stays one here: read as
    // a wider type it would carry whatever the caller left in the top half.
    {"uint16", "std::uint16_t", "arg_uint", "0",
     "static_cast<std::uint16_t>(steammock::reply_uint(reply))",
     "static_cast<std::uint16_t>(steammock::as_uint64(*value))"},
    {"int64", "std::int64_t", "arg_int", "0", "steammock::reply_int(reply)",
     "steammock::as_int64(*value)"},
    {"uint64", "std::uint64_t", "arg_uint", "0", "steammock::reply_uint(reply)",
     "steammock::as_uint64(*value)"},
    {"float", "float", "arg_real", "0.0f", "static_cast<float>(steammock::reply_real(reply))",
     "static_cast<float>(steammock::as_double(*value))"},
    {"double", "double", "arg_real", "0.0", "steammock::reply_real(reply)",
     "steammock::as_double(*value)"},
    // A returned string is copied by the stub before the reply dies with the
    // call, so the game gets its own text (see bridge/call.hpp).
    {"cstring", "const char*", "arg_cstring", "kEmptyString",
     "steammock::reply_cstring(reply, kEmptyString)", nullptr},
    {"opaque_ptr", "void*", "arg_pointer", "nullptr", "steammock::reply_pointer(reply)", nullptr},
};

const TypeInfo* find_type(const std::string& name) noexcept {
    for (const TypeInfo& type : kTypes) {
        if (name == type.idl) {
            return &type;
        }
    }
    return nullptr;
}

// The stub's own exports, which are not Steam API calls: they exist so the
// harness can be asked which build is loaded and whether it reached a backend.
const char* const kDiagnosticExports[] = {"SteamMock_SessionId", "SteamMock_Stats",
                                          "SteamMock_Version"};
constexpr std::size_t kDiagnosticExportCount =
    sizeof(kDiagnosticExports) / sizeof(kDiagnosticExports[0]);

constexpr const char* kGeneratedNote = "//  GENERATED FILE - do not edit by hand.";
constexpr const char* kRegenerate = "//  Regenerate: steammock_codegen";
constexpr const char* kRegenerateDef = "; Regenerate: steammock_codegen";

std::string joined(const std::vector<std::string>& lines) {
    std::string text;
    for (std::size_t index = 0; index < lines.size(); ++index) {
        if (index != 0u) {
            text += "\n";
        }
        text += lines[index];
    }
    return text;
}

// A C++ string literal for `text`. Escapes backslash and quote, and writes any
// other byte outside printable ASCII as a three-digit octal escape - not \xNN,
// which in C++ swallows as many hex digits as follow it.
std::string cpp_string_literal(const std::string& text) {
    std::string out = "\"";
    for (const char ch : text) {
        const unsigned char byte = static_cast<unsigned char>(ch);
        if (ch == '\\' || ch == '"') {
            out += '\\';
            out += ch;
        } else if (byte < 0x20u || byte >= 0x7Fu) {
            char escape[8] = {};
            std::snprintf(escape, sizeof(escape), "\\%03o", static_cast<unsigned>(byte));
            out += escape;
        } else {
            out += ch;
        }
    }
    out += '"';
    return out;
}

std::string signature(const IdlCall& call) {
    std::string params;
    for (const IdlParam& param : call.params) {
        if (!params.empty()) {
            params += ", ";
        }
        params += find_type(param.type)->cpp;
        if (param.out) {
            params += "*";
        }
        params += " ";
        params += param.name;
    }
    // A parameterless function is spelled (void) so the C prototype is explicit.
    if (params.empty()) {
        params = "void";
    }
    const std::string returns = call.returns == "void" ? "void" : find_type(call.returns)->cpp;
    return returns + " STEAMMOCK_CALL " + call.name + "(" + params + ")";
}

// The body of one trampoline: build the arguments, ask, fall back.
void render_body(const IdlCall& call, std::vector<std::string>& out) {
    const bool returns_void = call.returns == "void";

    if (!returns_void) {
        out.push_back(std::string("    ") + find_type(call.returns)->cpp +
                      " result = " + find_type(call.returns)->return_default + ";");
    }
    out.push_back("    try {");

    if (!call.hook.empty()) {
        // A trampoline that has to do something a forward cannot: the calls that
        // remember what a game registered, and the pump that hands it what the
        // backend sent. Recording happens before the forward, so an answer that
        // carries an event of its own arrives with somewhere for it to go.
        if (call.hook == "register_callback") {
            out.push_back("        steammock::callback_registered(" + call.params[0].name + ", " +
                          call.params[1].name + ");");
        } else if (call.hook == "unregister_callback") {
            out.push_back("        steammock::callback_unregistered(" + call.params[0].name + ");");
        } else if (call.hook == "register_call_result") {
            out.push_back("        steammock::call_result_registered(" + call.params[0].name +
                          ", " + call.params[1].name + ");");
        } else if (call.hook == "unregister_call_result") {
            out.push_back("        steammock::call_result_unregistered(" + call.params[0].name +
                          ", " + call.params[1].name + ");");
        } else if (call.hook == "deliver_events") {
            out.push_back("        steammock::deliver_events();");
        }
    }

    if (call.fallback == "context") {
        // The SDK's lazy accessor is the stub's to answer, and it does its own
        // reporting: the game calls this once per use of an interface, and the
        // SDK's cache is what makes that cheap. Forwarding every one of those to
        // the backend would put thousands of records a second in the transcript
        // for a game that polls - Spacewar's own loop asked 26,228 times in
        // fourteen seconds - so the call reaches the backend from the initialiser
        // inside, once, and nowhere else.
        out.push_back(std::string("        result = steammock::context_init(") +
                      call.params[0].name + ", \"" + call.name + "\");");
        out.push_back("    } catch (...) {");
        out.push_back("        // Never let an exception cross into the game.");
        out.push_back("    }");
        out.push_back("    return result;");
        return;
    }

    out.push_back("        steammock::Json args = steammock::Json::object();");
    for (const IdlParam& param : call.params) {
        const TypeInfo& type = *find_type(param.type);
        if (param.out) {
            // Send what the caller passed in, so the backend can see the current
            // value; a null pointer is reported as null and never dereferenced.
            out.push_back("        args[\"" + param.name + "\"] = " + param.name + " != nullptr");
            out.push_back(std::string("                                  ? steammock::") +
                          type.arg_helper + "(static_cast<" + type.cpp + ">(*" + param.name + "))");
            out.push_back("                                  : steammock::Json();");
        } else if (std::string(param.type) == "cstring") {
            out.push_back("        args[\"" + param.name + "\"] = steammock::arg_cstring(" +
                          param.name + ");");
        } else {
            out.push_back(std::string("        args[\"") + param.name +
                          "\"] = steammock::" + type.arg_helper + "(" + param.name + ");");
        }
    }
    out.push_back("");
    out.push_back("        steammock::Json reply;");
    const std::string invoke = "steammock::invoke(\"" + call.name + "\", args, reply)";

    bool has_out = false;
    for (const IdlParam& param : call.params) {
        has_out = has_out || param.out;
    }

    if (returns_void && !has_out) {
        out.push_back("        (void)" + invoke + ";");
    } else {
        out.push_back("        if (" + invoke + ") {");
        for (const IdlParam& param : call.params) {
            if (!param.out) {
                continue;
            }
            out.push_back("            if (" + param.name + " != nullptr) {");
            out.push_back(std::string("                const steammock::Json* value = "
                                      "steammock::reply_out(reply, \"") +
                          param.name + "\");");
            out.push_back("                if (value != nullptr && !value->is_null()) {");
            out.push_back(std::string("                    *") + param.name + " = " +
                          find_type(param.type)->out_expr + ";");
            out.push_back("                }");
            out.push_back("            }");
        }
        if (!returns_void) {
            out.push_back(
                "            result = " + std::string(find_type(call.returns)->reply_expr) + ";");
        }
        out.push_back("        }");
    }
    if (!call.fallback.empty()) {
        // Asked for something this stub can answer itself. A factory call is
        // asked of the backend first, because a scenario may want to name the
        // object or to answer for a version string we have no layout for; the
        // lazy accessor is answered here, and tells the backend when it does.
        out.push_back("        if (result == nullptr) {");
        if (call.fallback == "interface") {
            // The user handle this interface is asked for under, when the call names
            // one: the game-server half of an interface has a handle of its own, and at
            // the hand-out that is the only thing that can say which end is asking.
            std::string fallback_user = "0";
            for (const auto& param : call.params) {
                if (param.name == "hSteamUser" || param.name == "hSteamuser") {
                    fallback_user = param.name;
                }
            }
            out.push_back(std::string("            result = steammock::interface_object(") +
                          call.fallback_param + ", " + fallback_user + ");");
        } else {
            out.push_back(std::string("            result = steammock::context_init(") +
                          call.params[0].name + ", \"" + call.name + "\");");
        }
        out.push_back("        }");
    }
    out.push_back("    } catch (...) {");
    out.push_back("        // Never let an exception cross into the game.");
    out.push_back("    }");
    if (!returns_void) {
        out.push_back("    return result;");
    }
}

std::vector<std::string> generated_header(const Idl& idl, const char* regenerate, const char* open,
                                          const char* close) {
    return {open,
            kGeneratedNote,
            "//",
            std::string("//  Source:   gen/steam_api.idl.json (surface '") + idl.surface() +
                "', revision " + std::to_string(idl.revision()) + ", " +
                std::to_string(idl.calls().size()) + " calls)",
            regenerate,
            close};
}

}  // namespace

// ---------------------------------------------------------------------------
//  The IDL
// ---------------------------------------------------------------------------

bool Idl::from_json(const Json& document, Idl& out, std::string& error) {
    if (!document.is_object()) {
        error = "the IDL has to be a JSON object";
        return false;
    }
    const Json* calls_json = json_member(document, "calls");
    if (calls_json == nullptr || !calls_json->is_array()) {
        error = "the IDL needs a 'calls' array";
        return false;
    }

    Idl parsed;
    if (const Json* surface = json_member(document, "surface");
        surface != nullptr && surface->is_string()) {
        parsed._surface = as_string(*surface);
    }
    if (const Json* revision = json_member(document, "revision");
        revision != nullptr && revision->is_number()) {
        parsed._revision = static_cast<int>(as_int64(*revision));
    }

    for (const Json& entry : *calls_json) {
        if (!entry.is_object()) {
            error = "every call has to be a JSON object";
            return false;
        }
        const Json* name = json_member(entry, "name");
        if (name == nullptr || !name->is_string() || as_string(*name).empty()) {
            error = "every call needs a 'name'";
            return false;
        }

        IdlCall call;
        call.name = as_string(*name);
        for (const IdlCall& existing : parsed._calls) {
            if (existing.name == call.name) {
                error = call.name + " appears twice";
                return false;
            }
        }

        if (const Json* returns = json_member(entry, "returns"); returns != nullptr) {
            if (!returns->is_string()) {
                error = call.name + ": 'returns' has to be a type name";
                return false;
            }
            call.returns = as_string(*returns);
        }
        if (const Json* hook = json_member(entry, "hook"); hook != nullptr) {
            if (!hook->is_string()) {
                error = call.name + ": 'hook' has to be a name";
                return false;
            }
            call.hook = as_string(*hook);
            // Named rather than trusted: a hook the generator does not know is a
            // trampoline that would quietly do nothing.
            static const char* const kHooks[] = {"register_callback", "unregister_callback",
                                                 "register_call_result", "unregister_call_result",
                                                 "deliver_events"};
            bool known = false;
            for (const char* candidate : kHooks) {
                known = known || call.hook == candidate;
            }
            if (!known) {
                error = call.name + ": '" + call.hook + "' is not a hook this has";
                return false;
            }
        }
        if (call.returns != "void" && find_type(call.returns) == nullptr) {
            error = call.name + ": unknown return type '" + call.returns + "'";
            return false;
        }

        if (const Json* params = json_member(entry, "params");
            params != nullptr && params->is_array()) {
            for (const Json& entry_param : *params) {
                if (!entry_param.is_object()) {
                    error = call.name + ": every parameter has to be a JSON object";
                    return false;
                }
                IdlParam param;
                const Json* param_name = json_member(entry_param, "name");
                if (param_name == nullptr || !param_name->is_string() ||
                    as_string(*param_name).empty()) {
                    error = call.name + ": every parameter needs a 'name'";
                    return false;
                }
                param.name = as_string(*param_name);

                const Json* param_type = json_member(entry_param, "type");
                if (param_type == nullptr || !param_type->is_string() ||
                    find_type(as_string(*param_type)) == nullptr) {
                    error =
                        call.name + "." + param.name + ": unknown type '" +
                        (param_type != nullptr && param_type->is_string() ? as_string(*param_type)
                                                                          : std::string()) +
                        "'";
                    return false;
                }
                param.type = as_string(*param_type);

                std::string direction = "in";
                if (const Json* dir = json_member(entry_param, "dir");
                    dir != nullptr && dir->is_string()) {
                    direction = as_string(*dir);
                }
                if (direction != "in" && direction != "out") {
                    error = call.name + "." + param.name + ": dir must be 'in' or 'out'";
                    return false;
                }
                param.out = direction == "out";
                if (param.out && (param.type == "cstring" || param.type == "opaque_ptr")) {
                    error = call.name + "." + param.name + ": out parameters of type '" +
                            param.type +
                            "' need a length or an owned buffer - add a dedicated kind when you "
                            "need one";
                    return false;
                }
                call.params.push_back(std::move(param));
            }
        }
        // A hook is spelled with the parameters it needs, and the renderer indexes
        // them by position: a file that named `register_callback` with one
        // parameter would be a crash in the generator rather than a message about
        // the file.
        if (!call.hook.empty()) {
            const std::size_t needed = call.hook == "deliver_events"        ? 0u
                                       : call.hook == "unregister_callback" ? 1u
                                                                            : 2u;
            if (call.params.size() < needed) {
                error = call.name + ": '" + call.hook + "' takes " + std::to_string(needed) +
                        " parameter(s) and has " + std::to_string(call.params.size());
                return false;
            }
        }
        // The calls that can answer themselves: they are handed a version string a
        // game wants an interface for, and the stub has objects of its own for
        // some of them (see bridge/synth.hpp).
        if (const Json* fallback = json_member(entry, "fallback"); fallback != nullptr) {
            const std::string kind = fallback->is_string() ? as_string(*fallback) : std::string();
            const bool factory = kind == "interface";
            const bool lazy = kind == "context";
            if (!factory && !lazy) {
                error = call.name + ": 'fallback' is either absent, 'interface' or 'context'";
                return false;
            }
            // A factory is handed the version string somewhere among its arguments
            // - it is the only string in any of them, wherever it sits - and the
            // lazy accessor is handed the SDK's own blob and nothing else.
            const IdlParam* version = nullptr;
            for (const IdlParam& param : call.params) {
                if (param.type == "cstring" && !param.out) {
                    version = &param;
                }
            }
            const bool shape_is_right =
                factory ? version != nullptr
                        : call.params.size() == 1u && call.params[0].type == "opaque_ptr";
            if (call.returns != "opaque_ptr" || !shape_is_right) {
                error = call.name +
                        ": an 'interface' fallback returns opaque_ptr and is handed the "
                        "version string, and a 'context' one is handed the SDK's blob";
                return false;
            }
            call.fallback = kind;
            if (version != nullptr) {
                call.fallback_param = version->name;
            }
        }
        parsed._calls.push_back(std::move(call));
    }

    std::sort(parsed._calls.begin(), parsed._calls.end(),
              [](const IdlCall& left, const IdlCall& right) { return left.name < right.name; });
    out = std::move(parsed);
    return true;
}

bool Idl::load_file(const std::string& path, Idl& out, std::string& error) {
    std::FILE* file = std::fopen(path.c_str(), "rb");
    if (file == nullptr) {
        error = "cannot read " + path;
        return false;
    }
    std::string text;
    char buffer[4096];
    std::size_t got = 0;
    while ((got = std::fread(buffer, 1, sizeof(buffer), file)) > 0) {
        text.append(buffer, got);
    }
    std::fclose(file);

    Json document;
    if (!parse(text, document)) {
        error = path + " is not valid JSON";
        return false;
    }
    return from_json(document, out, error);
}

// ---------------------------------------------------------------------------
//  The generated files
// ---------------------------------------------------------------------------

std::string render_api_stub(const Idl& idl) {
    bool needs_interfaces = false;
    for (const IdlCall& call : idl.calls()) {
        needs_interfaces = needs_interfaces || call.fallback == "interface";
    }

    std::vector<std::string> out = generated_header(
        idl, kRegenerate,
        "// ============================================================================",
        "// ============================================================================");
    out.push_back("");
    out.push_back("#include \"bridge/call.hpp\"");
    out.push_back("#include \"bridge/export.hpp\"");
    if (needs_interfaces) {
        out.push_back("#include \"bridge/synth.hpp\"");
    }
    out.push_back("");
    out.push_back("namespace {");
    out.push_back("");
    out.push_back("// Handed back for a string-returning call nobody answered, so a game that");
    out.push_back("// skips its null check still reads empty text instead of faulting.");
    out.push_back("const char kEmptyString[] = \"\";");
    out.push_back("");
    out.push_back("}  // namespace");
    out.push_back("");

    for (const IdlCall& call : idl.calls()) {
        out.push_back("// " + call.name);
        out.push_back("STEAMMOCK_EXPORT " + signature(call) + " {");
        render_body(call, out);
        out.push_back("}");
        out.push_back("");
    }
    return joined(out);
}

std::string render_exports_def(const Idl& idl) {
    std::vector<std::string> names;
    names.reserve(idl.calls().size() + kDiagnosticExportCount);
    for (const IdlCall& call : idl.calls()) {
        names.push_back(call.name);
    }
    for (std::size_t index = 0; index < kDiagnosticExportCount; ++index) {
        names.emplace_back(kDiagnosticExports[index]);
    }
    std::sort(names.begin(), names.end());

    std::vector<std::string> out = {
        "; GENERATED FILE - do not edit by hand.",
        kRegenerateDef,
        ";",
        "; A .def rather than __declspec(dllexport) so the exported names are exactly the",
        "; ones games import, with no decoration on x86 and no difference on x64.",
        "EXPORTS",
    };
    for (const std::string& name : names) {
        out.push_back("    " + name);
    }
    return joined(out) + "\n";
}

std::string render_api_surface(const Idl& idl) {
    const std::vector<IdlCall>& calls = idl.calls();
    std::vector<std::string> out = generated_header(
        idl, kRegenerate,
        "// ============================================================================",
        "// ============================================================================");
    out.push_back("");
    out.push_back("#include \"bridge/surface.hpp\"");
    out.push_back("");
    out.push_back("namespace steammock {");
    out.push_back("namespace {");

    for (const IdlCall& call : calls) {
        if (call.params.empty()) {
            continue;
        }
        out.push_back("");
        out.push_back("const SurfaceParam kParams_" + call.name + "[] = {");
        for (const IdlParam& param : call.params) {
            out.push_back("    {\"" + param.name + "\", \"" + param.type + "\", " +
                          (param.out ? "true" : "false") + "},");
        }
        out.push_back("};");
    }

    out.push_back("");
    out.push_back("const SurfaceCall kCalls[] = {");
    if (calls.empty()) {
        // A zero-length array is not valid C++, and an IDL with no calls is a
        // mistake worth still being able to render, so this one is a placeholder
        // with the count below set to zero.
        out.push_back("    {\"\", \"\", nullptr, 0},");
    } else {
        for (const IdlCall& call : calls) {
            out.push_back("    {\"" + call.name + "\", \"" + call.returns + "\", " +
                          (call.params.empty() ? "nullptr" : "kParams_" + call.name) + ", " +
                          std::to_string(call.params.size()) + "},");
        }
    }
    out.push_back("};");
    out.push_back("");
    out.push_back("}  // namespace");
    out.push_back("");
    out.push_back("const char* api_surface_name() noexcept { return " +
                  cpp_string_literal(idl.surface()) + "; }");
    out.push_back("int api_surface_revision() noexcept { return " + std::to_string(idl.revision()) +
                  "; }");
    out.push_back("const SurfaceCall* api_surface_calls(std::size_t& count) noexcept {");
    out.push_back("    count = " + std::to_string(calls.size()) + ";");
    out.push_back("    return kCalls;");
    out.push_back("}");
    out.push_back("");
    out.push_back("}  // namespace steammock");
    return joined(out) + "\n";
}

}  // namespace steammock
