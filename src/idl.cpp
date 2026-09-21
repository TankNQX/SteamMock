#include "bridge/idl.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <string>
#include <utility>
#include <vector>

namespace steambridge {
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
    {"bool", "bool", "arg_bool", "false", "steambridge::reply_bool(reply)",
     "static_cast<bool>(value->as_bool())"},
    {"int32", "std::int32_t", "arg_int", "0",
     "static_cast<std::int32_t>(steambridge::reply_int(reply))",
     "static_cast<std::int32_t>(value->as_int64())"},
    {"uint32", "std::uint32_t", "arg_uint", "0",
     "static_cast<std::uint32_t>(steambridge::reply_uint(reply))",
     "static_cast<std::uint32_t>(value->as_uint64())"},
    {"int64", "std::int64_t", "arg_int", "0", "steambridge::reply_int(reply)", "value->as_int64()"},
    {"uint64", "std::uint64_t", "arg_uint", "0", "steambridge::reply_uint(reply)",
     "value->as_uint64()"},
    {"float", "float", "arg_real", "0.0f", "static_cast<float>(steambridge::reply_real(reply))",
     "static_cast<float>(value->as_double())"},
    {"double", "double", "arg_real", "0.0", "steambridge::reply_real(reply)", "value->as_double()"},
    // A returned string is copied by the stub before the reply dies with the
    // call, so the game gets its own text (see bridge/call.hpp).
    {"cstring", "const char*", "arg_cstring", "kEmptyString",
     "steambridge::reply_cstring(reply, kEmptyString)", nullptr},
    {"opaque_ptr", "void*", "arg_pointer", "nullptr", "steambridge::reply_pointer(reply)", nullptr},
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
const char* const kDiagnosticExports[] = {"SteamBridge_SessionId", "SteamBridge_Stats",
                                          "SteamBridge_Version"};
constexpr std::size_t kDiagnosticExportCount =
    sizeof(kDiagnosticExports) / sizeof(kDiagnosticExports[0]);

constexpr const char* kGeneratedNote = "//  GENERATED FILE - do not edit by hand.";
constexpr const char* kRegenerate = "//  Regenerate: steambridge_codegen";
constexpr const char* kRegenerateDef = "; Regenerate: steambridge_codegen";

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
    return returns + " STEAMBRIDGE_CALL " + call.name + "(" + params + ")";
}

// The body of one trampoline: build the arguments, ask, fall back.
void render_body(const IdlCall& call, std::vector<std::string>& out) {
    const bool returns_void = call.returns == "void";

    if (!returns_void) {
        out.push_back(std::string("    ") + find_type(call.returns)->cpp +
                      " result = " + find_type(call.returns)->return_default + ";");
    }
    out.push_back("    try {");
    out.push_back("        steambridge::Json args = steambridge::Json::object();");
    for (const IdlParam& param : call.params) {
        const TypeInfo& type = *find_type(param.type);
        if (param.out) {
            // Send what the caller passed in, so the backend can see the current
            // value; a null pointer is reported as null and never dereferenced.
            out.push_back("        args.set(\"" + param.name + "\", " + param.name + " != nullptr");
            out.push_back(std::string("                                  ? steambridge::") +
                          type.arg_helper + "(static_cast<" + type.cpp + ">(*" + param.name + "))");
            out.push_back("                                  : steambridge::Json::null());");
        } else if (std::string(param.type) == "cstring") {
            out.push_back("        args.set(\"" + param.name + "\", steambridge::arg_cstring(" +
                          param.name + "));");
        } else {
            out.push_back(std::string("        args.set(\"") + param.name +
                          "\", steambridge::" + type.arg_helper + "(" + param.name + "));");
        }
    }
    out.push_back("");
    out.push_back("        steambridge::Json reply;");
    const std::string invoke = "steambridge::invoke(\"" + call.name + "\", args, reply)";

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
            out.push_back(std::string("                const steambridge::Json* value = "
                                      "steambridge::reply_out(reply, \"") +
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
    const Json* calls_json = document.find("calls");
    if (calls_json == nullptr || !calls_json->is_array()) {
        error = "the IDL needs a 'calls' array";
        return false;
    }

    Idl parsed;
    if (const Json* surface = document.find("surface");
        surface != nullptr && surface->is_string()) {
        parsed._surface = surface->as_string();
    }
    if (const Json* revision = document.find("revision");
        revision != nullptr && revision->is_number()) {
        parsed._revision = static_cast<int>(revision->as_int64());
    }

    for (const Json& entry : calls_json->items()) {
        if (!entry.is_object()) {
            error = "every call has to be a JSON object";
            return false;
        }
        const Json* name = entry.find("name");
        if (name == nullptr || !name->is_string() || name->as_string().empty()) {
            error = "every call needs a 'name'";
            return false;
        }

        IdlCall call;
        call.name = name->as_string();
        for (const IdlCall& existing : parsed._calls) {
            if (existing.name == call.name) {
                error = call.name + " appears twice";
                return false;
            }
        }

        if (const Json* returns = entry.find("returns"); returns != nullptr) {
            if (!returns->is_string()) {
                error = call.name + ": 'returns' has to be a type name";
                return false;
            }
            call.returns = returns->as_string();
        }
        if (call.returns != "void" && find_type(call.returns) == nullptr) {
            error = call.name + ": unknown return type '" + call.returns + "'";
            return false;
        }

        if (const Json* params = entry.find("params"); params != nullptr && params->is_array()) {
            for (const Json& entry_param : params->items()) {
                if (!entry_param.is_object()) {
                    error = call.name + ": every parameter has to be a JSON object";
                    return false;
                }
                IdlParam param;
                const Json* param_name = entry_param.find("name");
                if (param_name == nullptr || !param_name->is_string() ||
                    param_name->as_string().empty()) {
                    error = call.name + ": every parameter needs a 'name'";
                    return false;
                }
                param.name = param_name->as_string();

                const Json* param_type = entry_param.find("type");
                if (param_type == nullptr || !param_type->is_string() ||
                    find_type(param_type->as_string()) == nullptr) {
                    error =
                        call.name + "." + param.name + ": unknown type '" +
                        (param_type != nullptr && param_type->is_string() ? param_type->as_string()
                                                                          : std::string()) +
                        "'";
                    return false;
                }
                param.type = param_type->as_string();

                std::string direction = "in";
                if (const Json* dir = entry_param.find("dir"); dir != nullptr && dir->is_string()) {
                    direction = dir->as_string();
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
    if (!Json::parse(text, document)) {
        error = path + " is not valid JSON";
        return false;
    }
    return from_json(document, out, error);
}

// ---------------------------------------------------------------------------
//  The generated files
// ---------------------------------------------------------------------------

std::string render_api_stub(const Idl& idl) {
    std::vector<std::string> out = generated_header(
        idl, kRegenerate,
        "// ============================================================================",
        "// ============================================================================");
    out.push_back("");
    out.push_back("#include \"bridge/call.hpp\"");
    out.push_back("#include \"bridge/export.hpp\"");
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
        out.push_back("STEAMBRIDGE_EXPORT " + signature(call) + " {");
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
    out.push_back("namespace steambridge {");
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
    out.push_back("}  // namespace steambridge");
    return joined(out) + "\n";
}

}  // namespace steambridge
