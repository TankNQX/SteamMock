#include "bridge/interfaces.hpp"

#include <cstddef>
#include <cstdio>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace steambridge {
namespace {

// ---------------------------------------------------------------------------
//  The type table.
// ---------------------------------------------------------------------------
//  A wire kind is what a layout carries; the declaration type is what
//  this writes for it. The two differ where an SDK's spelling is not the ABI's:
//  an enum is an int, and the pointer-sized integers are whatever the target's
//  pointer is - which is why those two are kinds of their own rather than being
//  folded into int32 and int64.

struct KindInfo {
    const char* kind;
    const char* cpp;
};

constexpr KindInfo kKinds[] = {
    {"bool", "bool"},
    {"int8", "std::int8_t"},
    {"uint8", "std::uint8_t"},
    {"int16", "std::int16_t"},
    {"uint16", "std::uint16_t"},
    {"int32", "std::int32_t"},
    {"uint32", "std::uint32_t"},
    {"int64", "std::int64_t"},
    {"uint64", "std::uint64_t"},
    {"float", "float"},
    {"double", "double"},
    {"intptr", "std::intptr_t"},
    {"uintptr", "std::size_t"},
    {"enum", "std::int32_t"},
    {"cstring", "const char*"},
    {"opaque_ptr", "void*"},
};

const KindInfo* find_kind(const std::string& name) noexcept {
    for (const KindInfo& kind : kKinds) {
        if (name == kind.kind) {
            return &kind;
        }
    }
    return nullptr;
}

// A kind with a type of its own behind it - a value class, a structure - is named
// rather than mapped, and the file has to say which.
bool named_kind(const std::string& kind) noexcept { return kind == "value" || kind == "struct"; }

// The C++ type a member of a structure is declared as. The layouts name members
// the way the SDK headers do, which is a scalar, or an enum - and an enum is an
// int, which is the rule the sizes in the file follow.
bool member_type(const std::string& declared, std::string& out, std::string& error) {
    static const KindInfo kMembers[] = {
        {"bool", "bool"},
        {"int8", "std::int8_t"},
        {"uint8", "std::uint8_t"},
        {"int16", "std::int16_t"},
        {"uint16", "std::uint16_t"},
        {"int32", "std::int32_t"},
        {"uint32", "std::uint32_t"},
        {"int64", "std::int64_t"},
        {"uint64", "std::uint64_t"},
        {"float", "float"},
        {"double", "double"},
    };
    for (const KindInfo& kind : kMembers) {
        if (declared == kind.kind) {
            out = kind.cpp;
            return true;
        }
    }
    if (declared.size() > 1 && declared[0] == 'E' && declared[1] >= 'A' && declared[1] <= 'Z') {
        out = "std::int32_t";
        return true;
    }
    error = "the member type '" + declared + "' has no declaration - the file has to record " +
            "the wire kind a member lands on";
    return false;
}

std::string number(int value) { return std::to_string(value); }

// A version string as an identifier: they are alphanumeric today, and this keeps
// a file that grew one punctuation mark from producing C++ that does not parse.
std::string identified(const std::string& version) {
    std::string out;
    out.reserve(version.size());
    for (const char ch : version) {
        const bool alphanumeric = (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') ||
                                  (ch >= 'A' && ch <= 'Z') || ch == '_';
        out += alphanumeric ? ch : '_';
    }
    return out;
}

std::string literal(const std::string& text) {
    std::string out = "\"";
    for (const char ch : text) {
        if (ch == '\\' || ch == '"') {
            out += '\\';
        }
        out += ch;
    }
    out += '"';
    return out;
}

// The declaration type of a parameter, and of a return value.
bool declared_type(const std::string& kind, const std::string& decl, std::string& out,
                   std::string& error, const std::string& where) {
    if (named_kind(kind)) {
        if (decl.empty()) {
            error = where + ": a '" + kind + "' needs a 'decl' to name its type";
            return false;
        }
        out = decl;
        return true;
    }
    const KindInfo* info = find_kind(kind);
    if (info == nullptr) {
        error = where + ": the kind '" + kind + "' has no declaration";
        return false;
    }
    out = info->cpp;
    return true;
}

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

// The parameter that names the interface a slot hands back, or empty when the
// slot is not one of those.
//
// `ISteamClient::GetISteamUser( hSteamUser, hSteamPipe, pchVersion )` answers the
// same question `SteamInternal_CreateInterface` was asked, so a declined one can
// be answered with the object of ours for that string - which is how a game built
// against an older SDK reaches an interface at all, since its accessors go
// through ISteamClient rather than asking for the interface themselves. What
// identifies one is a return of an interface pointer *and* a string parameter:
// the other interface-pointer returns take no version string, and the version is
// the last string in every one of them.
std::string interface_factory_parameter(const InterfaceSlot& slot) {
    if (slot.returns != "opaque_ptr") {
        return std::string();
    }
    std::string name;
    for (const InterfaceParam& param : slot.params) {
        if (param.kind == "cstring") {
            name = param.name;
        }
    }
    return name;
}

// ---------------------------------------------------------------------------
//  Reading the layouts
// ---------------------------------------------------------------------------

const Json* member(const Json& object, const char* name) noexcept { return object.find(name); }

bool read_string(const Json& object, const char* name, bool required, std::string& out,
                 std::string& error, const std::string& where) {
    const Json* value = member(object, name);
    if (value == nullptr || value->is_null()) {
        if (required) {
            error = where + ": '" + name + "' is missing";
            return false;
        }
        return true;
    }
    if (!value->is_string()) {
        error = where + ": '" + name + "' has to be a string";
        return false;
    }
    out = value->as_string();
    return true;
}

bool read_flag(const Json& object, const char* name, bool& out, std::string& error,
               const std::string& where) {
    const Json* value = member(object, name);
    if (value == nullptr) {
        return true;
    }
    if (value->kind() != Json::Kind::boolean) {
        error = where + ": '" + name + "' has to be true or false";
        return false;
    }
    out = value->as_bool();
    return true;
}

// A structure's size, which the ABI turns on: null means the size was never
// established for it, and a declaration without a size is not something this can
// write - so it is an error rather than a zero.
bool read_size(const Json& object, int& out, std::string& error, const std::string& where) {
    const Json* value = member(object, "size");
    if (value == nullptr || !value->is_number()) {
        error = where + ": no size - the file has to carry one before a declaration can be written";
        return false;
    }
    out = static_cast<int>(value->as_int64());
    return true;
}

}  // namespace

bool Interfaces::from_json(const Json& document, Interfaces& out, std::string& error) {
    if (!document.is_object()) {
        error = "the layouts have to be a JSON object";
        return false;
    }
    const Json* versions = member(document, "interfaces");
    if (versions == nullptr || !versions->is_array()) {
        error = "the layouts need an 'interfaces' array";
        return false;
    }

    Interfaces parsed;

    if (const Json* values = member(document, "value_types");
        values != nullptr && values->is_array()) {
        for (const Json& entry : values->items()) {
            InterfaceValueType value;
            const std::string where =
                "value_types[" + std::to_string(parsed._value_types.size()) + "]";
            if (!read_string(entry, "name", true, value.name, error, where) ||
                !read_string(entry, "wire", true, value.wire, error, where) ||
                !read_string(entry, "member", true, value.member, error, where) ||
                !read_size(entry, value.size, error, where)) {
                return false;
            }
            if (value.wire != "uint64") {
                error = where + ": the wire for a value class is a 64-bit integer, not '" +
                        value.wire + "'";
                return false;
            }
            parsed._value_types.push_back(std::move(value));
        }
    }

    if (const Json* structures = member(document, "structures");
        structures != nullptr && structures->is_array()) {
        for (const Json& entry : structures->items()) {
            InterfaceStructure structure;
            const std::string where =
                "structures[" + std::to_string(parsed._structures.size()) + "]";
            if (!read_string(entry, "name", true, structure.name, error, where) ||
                !read_size(entry, structure.size, error, where)) {
                return false;
            }
            const Json* members = member(entry, "members");
            if (members == nullptr || !members->is_array() || members->items().empty()) {
                error = where + ": a structure needs its members - that is what its ABI is";
                return false;
            }
            for (const Json& declared : members->items()) {
                if (!declared.is_array() || declared.items().size() != 2u) {
                    error = where + ": every member is a pair of a type and a name";
                    return false;
                }
                const std::string type = declared.items()[0].as_string();
                std::string cpp;
                if (!member_type(type, cpp, error)) {
                    error = where + " (" + structure.name + "): " + error;
                    return false;
                }
                structure.members.emplace_back(cpp, declared.items()[1].as_string());
            }
            parsed._structures.push_back(std::move(structure));
        }
    }

    for (const Json& entry : versions->items()) {
        InterfaceVersion version;
        const std::string where = "interfaces[" + std::to_string(parsed._versions.size()) + "]";
        if (!read_string(entry, "name", true, version.name, error, where) ||
            !read_string(entry, "version", true, version.version, error, where)) {
            return false;
        }
        const Json* slots = member(entry, "slots");
        if (slots == nullptr || !slots->is_array() || slots->items().empty()) {
            error = where + " (" + version.version + "): a version needs its slots";
            return false;
        }

        for (const Json& declared : slots->items()) {
            InterfaceSlot slot;
            const std::string slot_where = where + " (" + version.version + ")";
            if (!read_string(declared, "method", true, slot.method, error, slot_where) ||
                !read_string(declared, "returns", true, slot.returns, error, slot_where) ||
                !read_string(declared, "call", false, slot.call, error, slot_where) ||
                !read_string(declared, "returns_decl", false, slot.returns_decl, error,
                             slot_where) ||
                !read_flag(declared, "destructor", slot.destructor, error, slot_where)) {
                return false;
            }

            slot.returns_cpp = "void";
            if (slot.returns != "void" &&
                !declared_type(slot.returns, slot.returns_decl, slot.returns_cpp, error,
                               slot_where + "." + slot.method)) {
                return false;
            }

            if (const Json* params = member(declared, "params");
                params != nullptr && params->is_array()) {
                for (const Json& declared_param : params->items()) {
                    InterfaceParam param;
                    const std::string param_where = slot_where + "." + slot.method;
                    if (!read_string(declared_param, "name", true, param.name, error,
                                     param_where) ||
                        !read_string(declared_param, "kind", true, param.kind, error,
                                     param_where) ||
                        !read_string(declared_param, "decl", false, param.decl, error,
                                     param_where) ||
                        !read_flag(declared_param, "unmarshalable", param.opaque, error,
                                   param_where)) {
                        return false;
                    }
                    if (const Json* direction = member(declared_param, "dir");
                        direction != nullptr) {
                        if (!direction->is_string() || direction->as_string() != "out") {
                            error = param_where + "." + param.name +
                                    ": 'dir' is either absent or 'out'";
                            return false;
                        }
                        param.out = true;
                    }
                    if (!declared_type(param.kind, param.decl, param.cpp, error,
                                       param_where + "." + param.name)) {
                        return false;
                    }
                    if (param.out) {
                        if (param.kind == "cstring" || param.kind == "opaque_ptr") {
                            error = param_where + "." + param.name +
                                    ": an out parameter of kind '" + param.kind +
                                    "' is a buffer the wire cannot carry";
                            return false;
                        }
                        param.cpp += "*";
                    }
                    slot.params.push_back(std::move(param));
                }
            }
            version.slots.push_back(std::move(slot));
        }

        for (const InterfaceVersion& existing : parsed._versions) {
            if (existing.version == version.version) {
                error = version.version + " appears twice";
                return false;
            }
        }
        parsed._versions.push_back(std::move(version));
    }

    out = std::move(parsed);
    return true;
}

bool Interfaces::load_file(const std::string& path, Interfaces& out, std::string& error) {
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
//  The generated file
// ---------------------------------------------------------------------------
//  A slot body is one line because the marshalling lives in bridge/synth.hpp and
//  the names are data: what is written here per slot is its signature, the index
//  its descriptor sits at, and its argument names. The compiler still lays out
//  the vtable and the calling convention, which is the part that has to be
//  exactly right.

std::string render_api_interfaces(const Interfaces& interfaces) {
    std::size_t slot_count = 0;
    for (const InterfaceVersion& version : interfaces.versions()) {
        slot_count += version.slots.size();
    }

    std::vector<std::string> out = {
        "// ============================================================================",
        "//  GENERATED FILE - do not edit by hand.",
        "//",
        "//  Source:   gen/steam_interfaces.json (" +
            number(static_cast<int>(interfaces.versions().size())) + " interface versions, " +
            number(static_cast<int>(slot_count)) + " slots)",
        "//  Regenerate: steambridge_codegen",
        "// ============================================================================",
        "",
        "#include \"bridge/synth.hpp\"",
        "",
        "#include <cstring>",
        "",
        "namespace steambridge {",
        "namespace {",
        "",
        "// ---------------------------------------------------------------------------",
        "//  The types the declarations name.",
        "// ---------------------------------------------------------------------------",
        "//  None of them comes from an SDK. The ABI is a size and a shape, so a value",
        "//  class is the one integer the wire carries it as, and a structure that only",
        "//  ever travels by value is its members. The assertions check both against the",
        "//  sizes the layouts were imported with, so getting one wrong is a build error",
        "//  rather than a call the game reads the wrong way.",
        "",
        "#pragma pack(push, 4)",
    };

    for (const InterfaceValueType& value : interfaces.value_types()) {
        out.push_back("struct " + value.name + " {");
        out.push_back("    std::uint64_t " + value.member + ";");
        out.push_back("};");
        out.push_back("static_assert(sizeof(" + value.name + ") == " + number(value.size) + ", \"" +
                      value.name + " has to be the size the wire carries\");");
    }

    if (!interfaces.value_types().empty()) {
        out.push_back("");
    }

    for (const InterfaceStructure& structure : interfaces.structures()) {
        out.push_back("struct " + structure.name + " {");
        for (const auto& declared_member : structure.members) {
            out.push_back("    " + declared_member.first + " " + declared_member.second + ";");
        }
        out.push_back("};");
        out.push_back("static_assert(sizeof(" + structure.name + ") == " + number(structure.size) +
                      ", \"" + structure.name + " has to be the size the ABI passes\");");
    }

    out.push_back("#pragma pack(pop)");
    out.push_back("");
    out.push_back("}  // namespace");
    out.push_back("");

    if (!interfaces.value_types().empty() || !interfaces.structures().empty()) {
        out.push_back("// The kinds for the types above: a value class is the one integer the");
        out.push_back(
            "// wire carries it as, and a structure the wire cannot carry is a value the");
        out.push_back("// backend is told about but cannot set - so the call is reported and the");
        out.push_back("// caller gets a default, which is what a game sees with Steam absent.");
        out.push_back("");
        for (const InterfaceValueType& value : interfaces.value_types()) {
            out.push_back("template <> struct Kind<" + value.name + "> {");
            out.push_back("    static constexpr bool out() noexcept { return false; }");
            out.push_back("");
            out.push_back("    static Json in(" + value.name + " value) noexcept {");
            out.push_back("        return arg_uint(value." + value.member + ");");
            out.push_back("    }");
            out.push_back("    static " + value.name + " from(const Json& reply) noexcept {");
            out.push_back("        return " + value.name + "{reply_uint(reply)};");
            out.push_back("    }");
            out.push_back("    static void store(" + value.name +
                          "* target, const Json& value) noexcept {");
            out.push_back("        if (target != nullptr) {");
            out.push_back("            target->" + value.member + " = value.as_uint64();");
            out.push_back("        }");
            out.push_back("    }");
            out.push_back("    static " + value.name + " fallback() noexcept { return " +
                          value.name + "{0}; }");
            out.push_back("};");
            out.push_back("");
        }
        for (const InterfaceStructure& structure : interfaces.structures()) {
            out.push_back("template <> struct Kind<" + structure.name + "> {");
            out.push_back("    static constexpr bool out() noexcept { return false; }");
            out.push_back("");
            out.push_back("    static Json in(const " + structure.name + "&) noexcept {");
            out.push_back("        return Json::null();");
            out.push_back("    }");
            out.push_back("    static " + structure.name + " from(const Json&) noexcept {");
            out.push_back("        return " + structure.name + "{};");
            out.push_back("    }");
            out.push_back("    static " + structure.name + " fallback() noexcept { return " +
                          structure.name + "{}; }");
            out.push_back("};");
            out.push_back("");
        }
    }

    out.push_back("namespace {");
    out.push_back("");

    for (const InterfaceVersion& version : interfaces.versions()) {
        const std::string id = identified(version.version);

        // The argument names the backend sees, one array per slot that has any.
        for (std::size_t index = 0; index < version.slots.size(); ++index) {
            const InterfaceSlot& slot = version.slots[index];
            if (slot.params.empty()) {
                continue;
            }
            out.push_back("const char* const kParams_" + id + "_" +
                          number(static_cast<int>(index)) + "[] = {");
            for (const InterfaceParam& param : slot.params) {
                out.push_back("    " + literal(param.name) + ",");
            }
            out.push_back("};");
        }

        out.push_back("// " + version.name + " " + version.version);
        out.push_back("const steambridge::SlotInfo kSlots_" + id + "[] = {");
        for (std::size_t index = 0; index < version.slots.size(); ++index) {
            const InterfaceSlot& slot = version.slots[index];
            if (slot.destructor) {
                out.push_back("    {nullptr, nullptr},  // the virtual destructor");
                continue;
            }
            out.push_back("    {" + literal(slot.call) + ", " +
                          (slot.params.empty()
                               ? "nullptr"
                               : "kParams_" + id + "_" + number(static_cast<int>(index))) +
                          "},");
        }
        out.push_back("};");
        out.push_back("");

        out.push_back("class Version_" + id + " {");
        out.push_back("public:");
        for (std::size_t index = 0; index < version.slots.size(); ++index) {
            const InterfaceSlot& slot = version.slots[index];
            const std::string at = "kSlots_" + id + "[" + number(static_cast<int>(index)) + "]";
            if (slot.destructor) {
                out.push_back("    virtual ~Version_" + id + "() {}");
                continue;
            }

            std::string parameters;
            std::string arguments;
            for (const InterfaceParam& param : slot.params) {
                if (!parameters.empty()) {
                    parameters += ", ";
                    arguments += ", ";
                }
                parameters += param.cpp + " " + param.name;
                arguments += param.name;
            }

            const std::string call = at + (arguments.empty() ? "" : ", " + arguments);
            const std::string factory = interface_factory_parameter(slot);
            out.push_back("    virtual " + slot.returns_cpp + " " + slot.method + "(" + parameters +
                          ") {");
            if (slot.returns == "void") {
                out.push_back("        steambridge::slot<void>(" + call + ");");
            } else if (factory.empty()) {
                out.push_back("        return steambridge::slot<" + slot.returns_cpp + ">(" + call +
                              ");");
            } else {
                // The same fallback the factory call in api_stub.cpp has: the
                // backend answered, or this is the object of ours for the string
                // the game named.
                out.push_back("        void* result = steambridge::slot<void*>(" + call + ");");
                out.push_back("        if (result == nullptr) {");
                out.push_back("            result = steambridge::interface_object(" + factory +
                              ");");
                out.push_back("        }");
                out.push_back("        return result;");
            }
            out.push_back("    }");
        }
        out.push_back("};");
        out.push_back("");
        out.push_back("Version_" + id + " g_" + id + ";");
        out.push_back("");
    }

    out.push_back("const steambridge::InterfaceVersion kVersions[] = {");
    for (const InterfaceVersion& version : interfaces.versions()) {
        out.push_back("    {" + literal(version.version) + ", &g_" + identified(version.version) +
                      "},");
    }
    out.push_back("};");
    out.push_back("");
    out.push_back("}  // namespace");
    out.push_back("");
    out.push_back("// The object to hand back for a version string, or null when this stub has");
    out.push_back("// none - a game that asked for something newer than any SDK this knows.");
    out.push_back("void* interface_object(const char* version) noexcept {");
    out.push_back("    if (version == nullptr) {");
    out.push_back("        return nullptr;");
    out.push_back("    }");
    out.push_back("    for (const InterfaceVersion& entry : kVersions) {");
    out.push_back("        if (std::strcmp(entry.version, version) == 0) {");
    out.push_back("            return entry.object;");
    out.push_back("        }");
    out.push_back("    }");
    out.push_back("    return nullptr;");
    out.push_back("}");
    out.push_back("");
    out.push_back("const InterfaceVersion* interface_versions(std::size_t& count) noexcept {");
    out.push_back("    count = sizeof(kVersions) / sizeof(kVersions[0]);");
    out.push_back("    return kVersions;");
    out.push_back("}");
    out.push_back("");
    out.push_back("}  // namespace steambridge");
    return joined(out) + "\n";
}

}  // namespace steambridge
