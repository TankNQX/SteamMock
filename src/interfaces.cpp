#include "bridge/interfaces.hpp"

#include <climits>
#include <cstddef>
#include <cstdio>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace steammock {
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
    {"bytes", "const void*"},
    {"out_bytes", "void*"},
    // A text buffer the caller owns, which the wire carries as the text itself rather than
    // as hex - see bridge/synth.hpp. Like the byte buffer above, it is a shape with two
    // halves, so a row that names it also names the parameter carrying its length.
    {"out_text", "char*"},
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
        switch (ch) {
            case '\\': out += "\\\\"; break;
            case '"': out += "\\\""; break;
            case '\n': out += "\\n"; break;
            case '\t': out += "\\t"; break;
            case '\r': out += "\\r"; break;
            default:
                // Anything else that is not printable, as an octal escape. A raw control
                // byte inside a "..." literal is either a literal the compiler refuses or
                // one that ends early and takes the rest of the line with it - and these
                // strings are method, call, event and version names, which a layouts file
                // is not supposed to be able to smuggle a newline into at all.
                if (static_cast<unsigned char>(ch) < 0x20u ||
                    static_cast<unsigned char>(ch) == 0x7Fu) {
                    char escape[5] = {};
                    std::snprintf(escape, sizeof(escape), "\\%03o", static_cast<unsigned char>(ch));
                    out += escape;
                } else {
                    out += ch;
                }
        }
    }
    out += '"';
    return out;
}

// A string for a comment rather than for a literal: nothing here escapes, so a newline or
// a control byte would end the comment and leave the rest of it as source. Version names
// and strings go above every class the file writes.
std::string commented(const std::string& text) {
    std::string out;
    out.reserve(text.size());
    for (const char ch : text) {
        const unsigned char byte = static_cast<unsigned char>(ch);
        out += (byte < 0x20u || byte == 0x7Fu) ? ' ' : ch;
    }
    return out;
}

// One member of a structure, written from the field of that name - and one expression for the
// two places that need it, because it is the same question twice: a field of this kind, into a
// member of that declaration. A payload being filled (`value.m_x`, from a `fields` object) and
// a structure a call fills in through the caller's pointer (`target->m_x`, from `value`) differ
// only in the two expressions, which is why they are parameters.
//
// False means there was nothing to write: a member the wire cannot carry in one field, an
// array, is left as the caller had it, and the caller is told so.
bool push_member_write(const std::string& cpp, const std::string& member, const std::string& fields,
                       const std::string& target, std::vector<std::string>& out) {
    if (member.find('[') != std::string::npos) {
        return false;
    }
    std::string read = "as_int64";
    if (cpp == "bool") {
        read = "as_bool";
    } else if (cpp == "float" || cpp == "double") {
        read = "as_double";
    } else if (cpp.compare(0, 9, "std::uint") == 0 || cpp == "std::size_t") {
        read = "as_uint64";
    }
    out.push_back("    if (const Json* field = steammock::json_member(" + fields + ", " +
                  literal(member) + ")) {");
    out.push_back("        " + target + member + " = static_cast<" + cpp + ">(steammock::" + read +
                  "(*field));");
    out.push_back("    }");
    return true;
}

// The structures some call hands back through a pointer - the only ones the generated file can
// *write*, and the reason a structure's trait has a store or does not. A structure is otherwise
// a shape the wire has no answer for: it can be reported and defaulted, and not set.
//
// Which ones those are is the layouts' answer, not the declaration's: the layouts come from an
// SDK, the SDK declares most of these as `void*` (see STRUCT_OUT in the importer for the ones
// that are a single structure and not a list), and a parameter's kind is what says it.
std::set<std::string> written_structures(const Interfaces& interfaces) {
    std::set<std::string> names;
    for (const InterfaceVersion& version : interfaces.versions()) {
        for (const InterfaceSlot& slot : version.slots) {
            for (const InterfaceParam& param : slot.params) {
                if (param.out && param.kind == "struct") {
                    names.insert(param.decl);
                }
            }
        }
    }
    return names;
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

// Which of a call's length parameters are the caller's own pointer - the two-call shape, where
// a game asks how much it needs by passing a null buffer and a count of zero and then
// allocates what came back. Those are the ones the generated call wraps: a length passed by
// value has nowhere to write an answer, so it travels as the value it is and nothing stores it
// (which is what `Kind<int32>::out()` being false already says).
std::set<std::string> pointer_lengths(const InterfaceSlot& slot) {
    std::set<std::string> names;
    for (const InterfaceParam& param : slot.params) {
        if (param.length.empty()) {
            continue;
        }
        for (const InterfaceParam& other : slot.params) {
            if (other.name == param.length && !other.cpp.empty() && other.cpp.back() == '*') {
                names.insert(param.length);
            }
        }
    }
    return names;
}

// What one parameter travels as. Most are the parameter itself; a buffer is the value and the
// thing that says how long it is, together, and those are the wrappers in bridge/synth.hpp.
std::string argument_of(const InterfaceParam& param, const std::set<std::string>& lengths) {
    if (lengths.count(param.name) != 0u) {
        return "steammock::BufferLength{" + param.name + "}";
    }
    if (param.kind == "bytes") {
        // The buffer and how long it is travel together, which is what the layouts name the
        // length parameter for. Braces rather than a call: one of these is an aggregate, and
        // a function-style cast of an aggregate is only spelled that way from C++20 on.
        return "steammock::Bytes{" + param.name + ", " + param.length + "}";
    }
    if (param.kind == "out_bytes") {
        return "steammock::BytesOut{" + param.name + ", " + param.length + "}";
    }
    if (param.kind == "out_text") {
        return "steammock::TextOut{" + param.name + ", " + param.length + "}";
    }
    // A named type that is out *and* names a length is that many of them: one structure is
    // what `out` alone says, and a list of them is what the length adds.
    if (param.out && named_kind(param.kind) && !param.length.empty()) {
        return "steammock::ArrayOut<" + param.decl + ">{" + param.name + ", " + param.length + "}";
    }
    return param.name;
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
//  `member` is the one in bridge/json_read.hpp: this file used to carry its own
//  overload for a `const char*` name, which is what std::string_view is for.

bool read_string(const Json& object, const char* name, bool required, std::string& out,
                 std::string& error, const std::string& where) {
    const Json* value = json_member(object, name);
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
    out = as_string(*value);
    return true;
}

bool read_flag(const Json& object, const char* name, bool& out, std::string& error,
               const std::string& where) {
    const Json* value = json_member(object, name);
    if (value == nullptr) {
        return true;
    }
    if (!value->is_boolean()) {
        error = where + ": '" + name + "' has to be true or false";
        return false;
    }
    out = as_bool(*value);
    return true;
}

// A structure's size, which the ABI turns on: null means the size was never
// established for it, and a declaration without a size is not something this can
// write - so it is an error rather than a zero.
bool read_size(const Json& object, int& out, std::string& error, const std::string& where) {
    const Json* value = json_member(object, "size");
    if (value == nullptr || !value->is_number()) {
        error = where + ": no size - the file has to carry one before a declaration can be written";
        return false;
    }
    // A size is an int in every signature this writes, and this used to narrow without
    // looking: a negative or oversized one became whatever the cast made of it and the
    // generated file carried a structure whose size no longer matched the file's own
    // declaration of it - which is exactly the thing these layouts are read for.
    const std::int64_t size = as_int64(*value);
    if (size <= 0 || size > static_cast<std::int64_t>(INT_MAX)) {
        error = where + ": size " + std::to_string(size) + " is out of range";
        return false;
    }
    out = static_cast<int>(size);
    return true;
}

// ---------------------------------------------------------------------------
//  Reading a slot
// ---------------------------------------------------------------------------
//  A version's slots are rows, one per slot, in vtable order:
//
//    ["GetSteamID", "CSteamID"]
//    ["GetInstalledApps", "uint32", [["pvecAppID", "uint32", "out"], ["unMaxAppIDs", "uint32"]]]
//    ["GetStat", "bool", [["steamIDUser", "CSteamID"], ["pchName", "cstring"]],
//     {"call": "SteamAPI_ISteamUserStats_GetStat"}]
//    ["~"]                                  the vtable's destructor slot
//    ["~", {"call": "..."}]                 the same, with the name the SDK gives it
//
//  A type is one name - a kind from the table above, or a value class or a
//  structure this file declares - so the file says "CSteamID" rather than a kind
//  and a type behind it. A parameter's trailing strings are `out` and
//  `unmarshalable`, and a fourth row element is the rare slot that needs more:
//  the call name when it is not the flat name the method implies, or what the
//  wire cannot carry.
//
//  One more string a parameter can carry is the name of the parameter that says how long
//  it is, which is the other half of every buffer - the bytes, the text, and a list of a
//  named type (a structure named with `out` *and* a length is that many of them rather than
//  one). See the kinds in bridge/synth.hpp for what a length that is the caller's own
//  pointer means.
//
//  `named` is the value classes and structures read above, which is what tells a
//  type name from a kind.

bool resolve_type(const std::string& name,
                  const std::vector<std::pair<std::string, std::string>>& named, std::string& kind,
                  std::string& decl, std::string& error, const std::string& where) {
    if (name == "void") {
        kind = "void";
        decl.clear();
        return true;
    }
    if (find_kind(name) != nullptr) {
        kind = name;
        decl.clear();
        return true;
    }
    for (const auto& [declared_name, declared_kind] : named) {
        if (declared_name == name) {
            kind = declared_kind;  // "value" or "struct"
            decl = name;
            return true;
        }
    }
    error = where + ": '" + name + "' is neither a wire kind nor a type this file declares";
    return false;
}

bool read_param(const Json& row, const std::vector<std::pair<std::string, std::string>>& named,
                InterfaceParam& out, std::string& error, const std::string& where) {
    if (!row.is_array() || row.size() < 2u || !row[0].is_string() || !row[1].is_string()) {
        error = where + ": a parameter is [name, type], and 'out' or 'unmarshalable' after it";
        return false;
    }
    out.name = as_string(row[0]);
    const std::string param_where = where + "." + out.name;
    if (!resolve_type(as_string(row[1]), named, out.kind, out.decl, error, param_where)) {
        return false;
    }
    for (std::size_t index = 2; index < row.size(); ++index) {
        const Json& flag = row[index];
        if (!flag.is_string()) {
            error = param_where + ": 'out' and 'unmarshalable' are written as strings";
            return false;
        }
        const std::string text = as_string(flag);
        if (text == "out") {
            out.out = true;
        } else if (text == "unmarshalable") {
            out.opaque = true;
        } else if (out.kind == "bytes" || out.kind == "out_bytes" || out.kind == "out_text" ||
                   named_kind(out.kind)) {
            // A buffer is two things on the wire, the value and how long it is, and the wire
            // has one value per parameter - so the file has to name the parameter that
            // carries the length rather than the size being guessed at.
            //
            // A *named* type is a buffer of one kind of its own: one structure, or - when a
            // length is named - that many of them, which is what an SDK's "give me the list
            // through my own array and count" call is. `out` is checked below, because a row
            // may write the length before the flag.
            if (!out.length.empty()) {
                error = param_where + ": a buffer names one length parameter";
                return false;
            }
            out.length = text;
        } else {
            error = param_where + ": '" + text + "' is neither 'out' nor 'unmarshalable'";
            return false;
        }
    }
    if (out.out && (out.kind == "cstring" || out.kind == "opaque_ptr")) {
        error = param_where + ": an out parameter of kind '" + out.kind +
                "' is a buffer the wire cannot carry";
        return false;
    }
    if (!out.length.empty() && named_kind(out.kind) && !out.out) {
        // One of a named type is what `out` alone says. A length on one is a list of them,
        // and a list is written through the caller's pointer or not at all.
        error = param_where + ": a named type with a length is a list of them, which is an "
                              "'out' parameter";
        return false;
    }
    if ((out.kind == "bytes" || out.kind == "out_bytes" || out.kind == "out_text") &&
        out.length.empty()) {
        // A buffer is two things on the wire, the value and how long it is, and the
        // length parameter is how the row says which of its siblings carries the second -
        // so a row without one cannot be written: the generator emitted
        // `steammock::Bytes{name, }` and the compiler said so, with a message about
        // generated code rather than about the row that produced it.
        error =
            param_where + ": a '" + out.kind + "' buffer names the parameter carrying its length";
        return false;
    }
    if (!declared_type(out.kind, out.decl, out.cpp, error, param_where)) {
        return false;
    }
    if (out.out) {
        out.cpp += "*";
    }
    return true;
}

// What a row says beyond its signature: the call name when it is not the one the
// method implies, whether the SDK keeps the method private, and whether the wire
// cannot carry what is returned. A destructor's row may carry the same object,
// which is how a version that has a flat name for its destructor records it.
bool read_notes(const Json& notes, InterfaceSlot& out, std::string& error,
                const std::string& where) {
    if (!notes.is_object()) {
        error = where + ": what a row says beyond its signature is an object of notes";
        return false;
    }
    return read_string(notes, "call", false, out.call, error, where) &&
           read_flag(notes, "private", out.private_api, error, where) &&
           read_flag(notes, "unmarshalable", out.returns_unmarshalable, error, where);
}

// One slot row. `interface_name` is the version's own name, which is what a call
// name is derived from when the row does not record one.
bool read_slot(const Json& row, const std::string& interface_name,
               const std::vector<std::pair<std::string, std::string>>& named, InterfaceSlot& out,
               std::string& error, const std::string& where) {
    if (!row.is_array() || row.empty() || !row[0].is_string()) {
        error = where + ": a slot is a row - [method, returns] or [method, returns, params], " +
                "or [\"~\"] for the destructor";
        return false;
    }
    const Json& items = row;
    const std::string head = as_string(items[0]);

    if (head == "~") {
        // The destructor owns a slot and names no call of its own. Where it sits
        // is what the row's position says, which is first in both versions that
        // have one.
        out.destructor = true;
        out.returns = "void";
        out.returns_cpp = "void";
        if (items.size() > 2u) {
            error = where + ": the destructor row is [\"~\"] and at most one object of notes";
            return false;
        }
        return items.size() == 1u || read_notes(items[1], out, error, where);
    }

    const std::string slot_where = where + " (" + head + ")";
    if (items.size() < 2u || !items[1].is_string()) {
        error = slot_where + ": a slot needs the type it returns";
        return false;
    }
    out.method = head;
    if (!resolve_type(as_string(items[1]), named, out.returns, out.returns_decl, error,
                      slot_where)) {
        return false;
    }
    out.returns_cpp = "void";
    if (out.returns != "void" &&
        !declared_type(out.returns, out.returns_decl, out.returns_cpp, error, slot_where)) {
        return false;
    }

    if (items.size() > 2u && !items[2].is_null()) {
        if (!items[2].is_array()) {
            error = slot_where + ": a slot's parameters are a list of rows";
            return false;
        }
        for (const Json& declared : items[2]) {
            InterfaceParam param;
            if (!read_param(declared, named, param, error, slot_where)) {
                return false;
            }
            out.params.push_back(std::move(param));
        }
    }

    if (items.size() > 3u && !read_notes(items[3], out, error, slot_where)) {
        return false;
    }
    if (items.size() > 4u) {
        error = slot_where + ": a slot row carries a method, a return type, its parameters " +
                "and at most one object of notes";
        return false;
    }

    // A buffer's length parameter has to be one of this slot's own: the generated
    // signature reads it by name, so a length that names nothing is `Bytes{pv, cbLength}`
    // with no `cbLength` in scope. That can only be checked now, once every parameter of
    // the row has been read.
    for (const InterfaceParam& param : out.params) {
        if (param.length.empty()) {
            continue;
        }
        bool found = false;
        bool pointer = false;
        for (const InterfaceParam& other : out.params) {
            if (other.name == param.length) {
                found = true;
                pointer = !other.cpp.empty() && other.cpp.back() == '*';
                break;
            }
        }
        if (!found) {
            error = slot_where + "." + param.name + ": '" + param.length +
                    "' is not a parameter of this call, so there is no length to pass";
            return false;
        }
        // Only a buffer the *caller* owns can be told what it got: what the call reads is
        // already in the caller's memory, so a length that is a pointer would be a room
        // nothing could report against - and `Bytes` has no place to put the number.
        if (param.kind == "bytes" && pointer) {
            error = slot_where + "." + param.name + ": '" + param.length +
                    "' is a pointer, and a buffer the call reads has no length to report";
            return false;
        }
    }

    // The flat name the call travels under, which is the method's own unless the
    // row recorded the one an overload or an implemented-in-terms-of threw away.
    if (out.call.empty()) {
        out.call = "SteamAPI_" + interface_name + "_" + out.method;
    }
    return true;
}

// The structures and the events are read the same way - a name, the size the
// layouts were imported with, and the members that size is made of - so one
// reader serves both. `what` is only there to name the thing in an error.
bool read_layouts(const Json& document, const char* key, const char* what,
                  std::vector<InterfaceStructure>& out, std::string& error) {
    const Json* entries = json_member(document, key);
    if (entries == nullptr || !entries->is_array()) {
        return true;
    }
    for (const Json& entry : *entries) {
        InterfaceStructure layout;
        const std::string where = std::string(key) + "[" + std::to_string(out.size()) + "]";
        if (!read_string(entry, "name", true, layout.name, error, where) ||
            !read_size(entry, layout.size, error, where)) {
            return false;
        }
        const Json* members = json_member(entry, "members");
        if (members == nullptr || !members->is_array()) {
            error = where + ": a " + what + " needs its members - that is what its ABI is";
            return false;
        }
        // A payload with no fields is a real shape: plenty of the SDK's callbacks are
        // notifications with nothing in them. A structure with no members is a
        // declaration that says nothing at all, so only the empty event is allowed.
        if (members->empty() && std::string(what) != "event") {
            error = where + ": a " + what + " with no members says nothing about its ABI";
            return false;
        }
        for (const Json& declared : *members) {
            if (!declared.is_array() || declared.size() != 2u) {
                error = where + ": every member is a pair of a type and a name";
                return false;
            }
            const std::string type = as_string(declared[0]);
            std::string cpp;
            if (!member_type(type, cpp, error)) {
                error = where + " (" + layout.name + "): " + error;
                return false;
            }
            layout.members.emplace_back(cpp, as_string(declared[1]));
        }
        out.push_back(std::move(layout));
    }
    return true;
}

}  // namespace

bool Interfaces::from_json(const Json& document, Interfaces& out, std::string& error) {
    if (!document.is_object()) {
        error = "the layouts have to be a JSON object";
        return false;
    }
    const Json* versions = json_member(document, "interfaces");
    if (versions == nullptr || !versions->is_array()) {
        error = "the layouts need an 'interfaces' array";
        return false;
    }

    Interfaces parsed;

    if (const Json* values = json_member(document, "value_types");
        values != nullptr && values->is_array()) {
        for (const Json& entry : *values) {
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

    if (!read_layouts(document, "structures", "structure", parsed._structures, error) ||
        !read_layouts(document, "events", "event", parsed._events, error)) {
        return false;
    }

    // Every payload carries the SDK's id for its callback as well as its size, and
    // it has to: a payload that completes no call can only be handed over to the
    // object a game registered under that id, so an event without one could be
    // written but never delivered.
    //
    // There used to be a check here comparing this array's length against what read_layouts
    // made of it, to catch "the array changed while it was being read". It cannot change:
    // both reads are of the same in-memory Json, and read_layouts has already answered with
    // an error for anything it would not read - so nothing reached the comparison that could
    // have failed it.
    if (const Json* events = json_member(document, "events");
        events != nullptr && events->is_array()) {
        for (std::size_t index = 0; index < parsed._events.size(); ++index) {
            const std::string where = "events[" + std::to_string(index) + "]";
            const Json* callback = json_member((*events)[index], "callback");
            if (callback == nullptr || !callback->is_number()) {
                error = where + ": an event needs the callback id a game registers it under";
                return false;
            }
            parsed._events[index].callback = static_cast<std::int32_t>(as_int64(*callback));
        }
    }

    // The names a slot can write where a kind would go. Both lists are read by
    // now, which is what lets a type name be told from a kind.
    std::vector<std::pair<std::string, std::string>> named;
    named.reserve(parsed._value_types.size() + parsed._structures.size());
    for (const InterfaceValueType& value : parsed._value_types) {
        named.emplace_back(value.name, "value");
    }
    for (const InterfaceStructure& structure : parsed._structures) {
        named.emplace_back(structure.name, "struct");
    }

    // Which version strings have been read already. A layouts file is dozens of versions
    // and hundreds of slots, and comparing every new version against every earlier one is
    // a scan of the whole file per version.
    std::set<std::string> seen_versions;
    // ...and which *identifiers* they made, which is not the same question: `identified`
    // turns every character that is not alphanumeric into an underscore, so "1.0" and
    // "1_0" are two version strings and one identifier. Two versions with one identifier
    // are two classes, two object arrays and two table rows with the same name - which the
    // compiler refuses, with a message about the generated file rather than about the
    // layouts that produced it.
    std::map<std::string, std::string> seen_identifiers;

    for (const Json& entry : *versions) {
        InterfaceVersion version;
        const std::string where = "interfaces[" + std::to_string(parsed._versions.size()) + "]";
        if (!read_string(entry, "name", true, version.name, error, where) ||
            !read_string(entry, "version", true, version.version, error, where)) {
            return false;
        }
        const Json* slots = json_member(entry, "slots");
        if (slots == nullptr || !slots->is_array() || slots->empty()) {
            error = where + " (" + version.version + "): a version needs its slots";
            return false;
        }

        for (const Json& declared : *slots) {
            InterfaceSlot slot;
            if (!read_slot(declared, version.name, named, slot, error,
                           where + " (" + version.version + ")")) {
                return false;
            }
            version.slots.push_back(std::move(slot));
        }

        if (!seen_versions.insert(version.version).second) {
            error = version.version + " appears twice";
            return false;
        }
        const std::string identifier = identified(version.version);
        const auto clash = seen_identifiers.find(identifier);
        if (clash != seen_identifiers.end()) {
            error = "'" + version.version + "' and '" + clash->second +
                    "' are the same identifier ('" + identifier +
                    "'): the classes and tables this writes are named after it";
            return false;
        }
        seen_identifiers.emplace(identifier, version.version);
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
    if (!parse(text, document)) {
        error = path + " is not valid JSON";
        return false;
    }
    return from_json(document, out, error);
}

// ---------------------------------------------------------------------------
//  The generated file
// ---------------------------------------------------------------------------
//  What is written per slot is its signature and the pooled call it names - one
//  line where that fits. What is *not* written per slot is anything two versions
//  share: the argument names a call carries and the call itself are interned
//  here first, because the same call with the same arguments declared in six
//  versions is one call, and writing it out six times is how this file came to
//  be larger than everything it describes.
//
//  The compiler still lays out the vtable and the calling convention, which is
//  the part that has to be exactly right.

std::string render_api_interfaces(const Interfaces& interfaces) {
    // Which structures this file has to be able to *write*, asked once: a call that fills one in
    // through a pointer needs the trait below to have a store, and one that is only ever reported
    // must not have one (an unused member write is a warning here, and a warning is an error).
    const std::set<std::string> written = written_structures(interfaces);

    // -----------------------------------------------------------------------
    //  The pools.
    // -----------------------------------------------------------------------
    //  Both are filled in the order a version first declares them, so the number
    //  a call gets is a property of the layout file rather than of the run - a
    //  reordering that changed the numbers would show up as a regenerated file
    //  rather than as a mystery in a diff.
    std::vector<std::vector<std::string>> param_lists;
    std::map<std::string, int> param_index;
    std::vector<std::pair<std::string, int>> calls;  // the call, and its names (-1: none)
    std::map<std::string, int> call_index;

    // A call with no arguments has no names to pool, and `nullptr` is what the
    // descriptor says instead - which is also what a slot with no names has
    // always carried.
    const auto intern_names = [&](const InterfaceSlot& slot) {
        std::vector<std::string> names;
        names.reserve(slot.params.size());
        for (const InterfaceParam& param : slot.params) {
            names.push_back(param.name);
        }
        if (names.empty()) {
            return -1;
        }

        // \x1f because a name is text: it cannot appear in one, so no two lists
        // can run together into the same key.
        std::string key;
        for (const std::string& name : names) {
            key += name;
            key += '\x1f';
        }

        const auto found = param_index.find(key);
        if (found != param_index.end()) {
            return found->second;
        }
        const int index = static_cast<int>(param_lists.size());
        param_index.emplace(std::move(key), index);
        param_lists.push_back(std::move(names));
        return index;
    };

    std::size_t slot_count = 0;
    std::vector<std::vector<int>> slot_call(interfaces.versions().size());
    for (std::size_t v = 0; v < interfaces.versions().size(); ++v) {
        const InterfaceVersion& version = interfaces.versions()[v];
        slot_count += version.slots.size();
        for (const InterfaceSlot& slot : version.slots) {
            if (slot.destructor) {
                // The destructor owns a slot and names no call.
                slot_call[v].push_back(-1);
                continue;
            }
            const int names = intern_names(slot);
            const std::string key = slot.call + '\x1f' + number(names);
            const auto found = call_index.find(key);
            if (found != call_index.end()) {
                slot_call[v].push_back(found->second);
                continue;
            }
            const int index = static_cast<int>(calls.size());
            call_index.emplace(key, index);
            calls.emplace_back(slot.call, names);
            slot_call[v].push_back(index);
        }
    }

    std::vector<std::string> out = {
        "// ============================================================================",
        "//  GENERATED FILE - do not edit by hand.",
        "//",
        "//  Source:   gen/steam_interfaces.json (" +
            number(static_cast<int>(interfaces.versions().size())) + " interface versions, " +
            number(static_cast<int>(slot_count)) + " slots,",
        "//            " + number(static_cast<int>(calls.size())) + " distinct calls)",
        "//  Regenerate: steammock_codegen",
        "// ============================================================================",
        "",
        "#include \"bridge/synth.hpp\"",
        "",
        "#include <atomic>",
        "#include <cstring>",
        "",
        "namespace steammock {",
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
        "//",
        "//  The pack is the SDK's and not ours. A callback struct is read by a game that",
        "//  has already decided where its members are, and the pack is what decides it:",
        "//  at four, Spacewar read a lobby id out of the padding and got half of it.",
        "",
        "#pragma pack(push, 8)",
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

    // A payload a call can be completed with is the SDK's own struct, so it is
    // declared in the same packed block - and asserted the same way, because a
    // size that is wrong here is a game reading past what the stub wrote.
    for (const InterfaceEvent& event : interfaces.events()) {
        out.push_back("struct " + event.name + " {");
        for (const auto& declared_member : event.members) {
            out.push_back("    " + declared_member.first + " " + declared_member.second + ";");
        }
        out.push_back("};");
        out.push_back("static_assert(sizeof(" + event.name + ") == " + number(event.size) + ", \"" +
                      event.name + " has to be the size the SDK's callback pack gives it\");");
        // The delivery buffer is bounded rather than sized from the file, so a file
        // that declares a payload bigger than it is a build error here instead of a
        // write past the end of a stack buffer inside a game's callback.
        out.push_back("static_assert(sizeof(" + event.name + ") <= steammock::kMaxEventBytes, \"" +
                      event.name + " has to fit the buffer a payload is delivered in\");");
    }

    out.push_back("#pragma pack(pop)");
    out.push_back("");

    // The structures a call fills in through a pointer the caller owns. A payload is written
    // into a buffer this stub borrowed; this is written into the game's own memory, which is why
    // it is checked against null and why a member the wire cannot carry is left as the caller had
    // it rather than zeroed. One function per structure rather than the work inline in the trait,
    // because the trait is a template specialisation and this is where the type is complete.
    for (const InterfaceStructure& structure : interfaces.structures()) {
        if (written.find(structure.name) == written.end()) {
            continue;
        }
        out.push_back("// " + structure.name + ", as a call fills it in: the fields the layouts");
        out.push_back(
            "// declare and nothing else - a member the wire cannot carry is left alone.");
        // The body is written first so the signature can leave the parameter unnamed when
        // nothing in it reads the object: a structure whose members are all arrays says nothing
        // about itself, and an unused parameter is a warning this build treats as an error. A
        // commented-out name is how a parameter that exists for the signature's sake says so.
        std::vector<std::string> body;
        bool writes = false;
        for (const auto& declared_member : structure.members) {
            writes = push_member_write(declared_member.first, declared_member.second, "fields",
                                       "target->", body) ||
                     writes;
        }
        out.push_back("void store_" + structure.name + "(" + structure.name +
                      "* target, const Json& " + (writes ? "fields" : "/*fields*/") +
                      ") noexcept {");
        out.push_back("    if (target == nullptr) {");
        out.push_back("        return;");
        out.push_back("    }");
        for (const std::string& line : body) {
            out.push_back(line);
        }
        out.push_back("}");
        out.push_back("");
    }

    out.push_back("}  // namespace");
    out.push_back("");

    if (!interfaces.value_types().empty() || !interfaces.structures().empty()) {
        out.push_back("// The kinds for the types above: a value class is the one integer the");
        out.push_back(
            "// wire carries it as, and a structure the wire cannot carry is a value the");
        out.push_back("// backend is told about but cannot set - so the call is reported and the");
        out.push_back("// caller gets a default, which is what a game sees with Steam absent.");
        out.push_back("//");
        out.push_back(
            "// ...except a structure some call hands back through a pointer, which has a");
        out.push_back(
            "// store and is written field by field from the answer: the SDK declares most");
        out.push_back("// of those as `void*`, so the layouts are what say it - see STRUCT_OUT in");
        out.push_back("// tools/steamworks_sdk_import.py for the ones that are a single structure");
        out.push_back("// and not a list of them.");
        out.push_back("");
        for (const InterfaceValueType& value : interfaces.value_types()) {
            out.push_back("template <> struct Kind<" + value.name + "> {");
            out.push_back("    static constexpr bool out() noexcept { return false; }");
            out.push_back("");
            out.push_back("    static Arg arg(" + value.name + " value) noexcept {");
            out.push_back("        return wire_uint(value." + value.member + ");");
            out.push_back("    }");
            out.push_back("    static " + value.name + " from(const Json& reply) noexcept {");
            out.push_back("        return " + value.name + "{reply_uint(reply)};");
            out.push_back("    }");
            out.push_back("    static void store(" + value.name +
                          "* target, const Json& value) noexcept {");
            out.push_back("        if (target != nullptr) {");
            out.push_back("            target->" + value.member +
                          " = steammock::as_uint64(value);");
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
            out.push_back("    static Arg arg(const " + structure.name + "&) noexcept {");
            out.push_back("        return wire_null();");
            out.push_back("    }");
            out.push_back("    static " + structure.name + " from(const Json&) noexcept {");
            out.push_back("        return " + structure.name + "{};");
            out.push_back("    }");
            if (written.find(structure.name) != written.end()) {
                // The structure a call fills in: the pointer is sent as the address it is, and
                // the answer's fields are written through it - which is the whole of what makes
                // an `out` structure different from one that is only ever reported.
                out.push_back("    static void store(" + structure.name +
                              "* target, const Json& value) noexcept {");
                out.push_back("        store_" + structure.name + "(target, value);");
                out.push_back("    }");
            }
            out.push_back("    static " + structure.name + " fallback() noexcept { return " +
                          structure.name + "{}; }");
            out.push_back("};");
            out.push_back("");
        }
    }

    out.push_back("namespace {");
    out.push_back("");

    // -----------------------------------------------------------------------
    //  The argument names, pooled.
    // -----------------------------------------------------------------------
    out.push_back("// ---------------------------------------------------------------------------");
    out.push_back("//  The argument names the calls carry.");
    out.push_back("// ---------------------------------------------------------------------------");
    out.push_back(
        "//  One list per distinct set of names, so a call declared in six versions names");
    out.push_back("//  its arguments once and all six point here. A call that takes none has no");
    out.push_back("//  list, which is what the null in its descriptor says.");
    out.push_back("");

    for (std::size_t index = 0; index < param_lists.size(); ++index) {
        const std::vector<std::string>& names = param_lists[index];
        const std::string head =
            "const char* const kParams_" + number(static_cast<int>(index)) + "[] = {";

        std::string one = head;
        for (std::size_t at = 0; at < names.size(); ++at) {
            if (at != 0u) {
                one += " ";
            }
            one += literal(names[at]) + (at + 1u == names.size() ? "};" : ",");
        }
        if (one.size() <= 100u) {
            out.push_back(one);
            continue;
        }

        out.push_back(head);
        for (const std::string& name : names) {
            out.push_back("    " + literal(name) + ",");
        }
        out.push_back("};");
    }
    out.push_back("");

    // -----------------------------------------------------------------------
    //  The calls, pooled.
    // -----------------------------------------------------------------------
    out.push_back("// ---------------------------------------------------------------------------");
    out.push_back("//  The calls themselves.");
    out.push_back("// ---------------------------------------------------------------------------");
    out.push_back("//  One entry per distinct call: the name it travels under, and the argument");
    out.push_back("//  names it carries. Two versions that declare the same call with the same");
    out.push_back("//  arguments are the same call, and each slot body below names one of these -");
    out.push_back("//  which is the whole of what a slot knows about itself.");
    out.push_back("");

    for (std::size_t index = 0; index < calls.size(); ++index) {
        const int names = calls[index].second;
        out.push_back("const steammock::SlotInfo kCall_" + number(static_cast<int>(index)) +
                      " = {" + literal(calls[index].first) + ", " +
                      (names < 0 ? "nullptr" : "kParams_" + number(names)) + "};");
    }
    out.push_back("");

    // -----------------------------------------------------------------------
    //  One class per version, whose virtuals mirror its slots in order.
    // -----------------------------------------------------------------------
    for (std::size_t v = 0; v < interfaces.versions().size(); ++v) {
        const InterfaceVersion& version = interfaces.versions()[v];
        const std::string id = identified(version.version);

        // Through `commented`: a newline in either of these would end the comment and leave
        // the rest of it as source, in a file whose whole point is that it is generated.
        out.push_back("// " + commented(version.name) + " " + commented(version.version));
        out.push_back("class Version_" + id + " {");
        out.push_back("public:");
        // Which user handle this object was handed out for. One version string asked
        // for under two handles is two objects, and this is what lets a call made
        // through one of them say who made it. The underscore because the accessors
        // that take a handle name their parameter hSteamUser, and a member of the same
        // name would be hidden by it in every one of them.
        //
        // Atomic: the hand-out writes it on whichever thread asked, and a call through
        // this object reads it on whatever thread the game makes that call from. See
        // InterfaceVersion in bridge/synth.hpp.
        out.push_back("    std::atomic<std::int32_t> _hSteamUser{0};");
        // Whether this object has been handed to anybody at all. `_hSteamUser` cannot say:
        // zero there is a real answer - the caller did not say which handle - so a claim
        // that stored zero would leave the object looking unclaimed and hand it out again.
        out.push_back("    std::atomic<bool> _claimed{false};");
        for (std::size_t index = 0; index < version.slots.size(); ++index) {
            const InterfaceSlot& slot = version.slots[index];
            if (slot.destructor) {
                out.push_back("    virtual ~Version_" + id + "() {}");
                continue;
            }

            // The handle travels in front of the call name, which is what the two
            // overloads of slot() tell apart. Whether it reaches the wire is the
            // marshaller's business - see needs_user_handle. A relaxed load: the value
            // is written once, before the object is handed to anyone, and what makes
            // the write visible to another thread is that thread being given the
            // pointer in the first place.
            const std::string at =
                "_hSteamUser.load(std::memory_order_relaxed), kCall_" + number(slot_call[v][index]);
            // Which of this call's length parameters is the caller's own pointer, asked once:
            // that one is sent as the value it points at rather than stored, because the buffer
            // has already written the number that belongs there.
            const std::set<std::string> lengths = pointer_lengths(slot);
            std::string parameters;
            std::string arguments;
            for (const InterfaceParam& param : slot.params) {
                if (!parameters.empty()) {
                    parameters += ", ";
                    arguments += ", ";
                }
                parameters += param.cpp + " " + param.name;
                arguments += argument_of(param, lengths);
            }

            const std::string passed = at + (arguments.empty() ? "" : ", " + arguments);
            const std::string factory = interface_factory_parameter(slot);
            // The user handle the game asked this object for, when the call says which
            // user it is for. A customer's ISteamNetworking and a game server's are the
            // same version string under different handles - `hSteamUser` in the SDK's
            // spelling, `hSteamuser` in a few of its declarations - and at the hand-out
            // this is the only thing that can tell the two of them apart.
            std::string factory_user = "0";
            for (const InterfaceParam& param : slot.params) {
                if (param.name == "hSteamUser" || param.name == "hSteamuser") {
                    factory_user = param.name;
                }
            }

            // A slot that only forwards fits on one line, which is what most of
            // them are. The ones that answer with an object of ours keep the
            // fallback under the signature, and so do the signatures that a
            // single line cannot hold.
            if (factory.empty()) {
                const std::string one =
                    "    virtual " + slot.returns_cpp + " " + slot.method + "(" + parameters +
                    ") { " +
                    (slot.returns == "void"
                         ? "steammock::slot<void>(" + passed + "); }"
                         : "return steammock::slot<" + slot.returns_cpp + ">(" + passed + "); }");
                if (one.size() <= 100u) {
                    out.push_back(one);
                    continue;
                }
            }

            out.push_back("    virtual " + slot.returns_cpp + " " + slot.method + "(" + parameters +
                          ") {");
            if (slot.returns == "void") {
                out.push_back("        steammock::slot<void>(" + passed + ");");
            } else if (factory.empty()) {
                out.push_back("        return steammock::slot<" + slot.returns_cpp + ">(" + passed +
                              ");");
            } else {
                // The same fallback the factory call in api_stub.cpp has: the
                // backend answered, or this is the object of ours for the string
                // the game named.
                out.push_back("        void* result = steammock::slot<void*>(" + passed + ");");
                out.push_back("        if (result == nullptr) {");
                out.push_back("            result = steammock::interface_object(" + factory + ", " +
                              factory_user + ");");
                out.push_back("        }");
                out.push_back("        return result;");
            }
            out.push_back("    }");
        }
        out.push_back("};");
        out.push_back("");
        // One object per user handle this version can be handed out under, so that a
        // call made through one of them can say which handle it was made under.
        out.push_back("Version_" + id + " g_" + id + "[steammock::kInterfaceEndpoints];");
        out.push_back("");
    }

    // -----------------------------------------------------------------------
    //  The payloads a call can be completed with.
    // -----------------------------------------------------------------------
    //  Written off the wire and into the game's own callback object, so the bytes
    //  have to be the game's layout - which is what the assertions above are for.
    //  A member the wire cannot carry in one field, an array, is left zeroed: a
    //  payload that needs one needs a rule this does not have yet.
    if (!interfaces.events().empty()) {
        out.push_back(
            "// ---------------------------------------------------------------------------");
        out.push_back("//  The payloads a call can be completed with.");
        out.push_back(
            "// ---------------------------------------------------------------------------");
        out.push_back(
            "//  Written into the game's own callback object, so the bytes are the ones the");
        out.push_back(
            "//  SDK's struct has - which is what the assertions above check. A member the");
        out.push_back("//  wire cannot carry in one field is left zeroed.");
        out.push_back("");

        for (const InterfaceEvent& event : interfaces.events()) {
            // The payload is written first for the same reason a structure's body is: a
            // notification with no member of its own reads nothing, and the signature is what
            // has to say so. The memcpy that hands the object over stays last either way.
            std::vector<std::string> body;
            bool reads = false;
            for (const auto& declared_member : event.members) {
                reads = push_member_write(declared_member.first, declared_member.second, "fields",
                                          "value.", body) ||
                        reads;
            }
            out.push_back("void fill_" + event.name + "(const Json& " +
                          (reads ? "fields" : "/*fields*/") + ", void* buffer) noexcept {");
            out.push_back("    " + event.name + " value{};");
            for (const std::string& line : body) {
                out.push_back(line);
            }
            out.push_back("    std::memcpy(buffer, &value, sizeof(value));");
            out.push_back("}");
            out.push_back("");
        }

        out.push_back("const steammock::EventInfo kEvents[] = {");
        for (const InterfaceEvent& event : interfaces.events()) {
            out.push_back("    {" + literal(event.name) + ",");
            out.push_back("     sizeof(" + event.name + "),");
            out.push_back("     " + number(event.callback) + ",");
            out.push_back("     &fill_" + event.name + "},");
        }
        out.push_back("};");
    } else {
        // The payloads are declared in the layouts file, so a build with none has none.
        // A table with nothing in it is not C++, so it is a null one - and the count and
        // the lookup below are the same either way, because a constant nobody reads is a
        // warning the clang job turns into an error.
        out.push_back("const steammock::EventInfo* const kEvents = nullptr;");
    }
    out.push_back("const std::size_t kEventCount = " +
                  number(static_cast<int>(interfaces.events().size())) + ";");
    out.push_back("");
    // Closed and reopened around this one definition: the marshalling in synth.cpp is
    // another translation unit, so the lookup needs external linkage - everything above
    // it in this file is internal on purpose.
    out.push_back("}  // namespace");
    out.push_back("");
    out.push_back("const EventInfo* find_event(const char* name) noexcept {");
    out.push_back("    for (std::size_t index = 0; index < kEventCount; ++index) {");
    out.push_back("        if (std::strcmp(kEvents[index].name, name) == 0) {");
    out.push_back("            return &kEvents[index];");
    out.push_back("        }");
    out.push_back("    }");
    out.push_back("    return nullptr;");
    out.push_back("}");
    out.push_back("");
    out.push_back("namespace {");
    out.push_back("");

    // A table with nothing in it is not C++, and a build with no layouts - which is
    // what a fresh clone has, since Valve's layouts are not part of the checkout -
    // has no versions at all. So the table exists when there is one, and the count
    // is what the two lookups below go by.
    if (!interfaces.versions().empty()) {
        out.push_back("const steammock::InterfaceVersion kVersions[] = {");
        for (const InterfaceVersion& version : interfaces.versions()) {
            // One row per version, carrying every object that version is handed out as
            // and the place each of them keeps its user handle in. Both, because a
            // version asked for under two handles is two objects, and the object is the
            // only thing that can say which handle a call through it was made under.
            out.push_back("    {" + literal(version.version) + ", {&g_" +
                          identified(version.version) + "[0], &g_" + identified(version.version) +
                          "[1]}, {&g_" + identified(version.version) + "[0]._hSteamUser, &g_" +
                          identified(version.version) + "[1]._hSteamUser}, {&g_" +
                          identified(version.version) + "[0]._claimed, &g_" +
                          identified(version.version) + "[1]._claimed}},");
        }
        out.push_back("};");
    } else {
        out.push_back("const steammock::InterfaceVersion* const kVersions = nullptr;");
    }
    out.push_back("const std::size_t kVersionCount = " +
                  number(static_cast<int>(interfaces.versions().size())) + ";");
    out.push_back("");
    out.push_back("}  // namespace");
    out.push_back("");
    out.push_back("// The object to hand back for a version string, or null when this stub has");
    out.push_back("// none - a game that asked for something newer than any SDK this knows. The");
    out.push_back("// handle is the user it is asked for under, or 0 when the caller did not say");
    out.push_back("// which: the object already holding that handle comes back, otherwise one");
    out.push_back("// nobody has claimed, and the object remembers the handle from then on.");
    out.push_back("//");
    out.push_back(
        "// Claiming an endpoint is a compare-exchange on whether it has been handed out");
    out.push_back(
        "// at all, and not on the handle it holds. The handle cannot be the test: 0 is a");
    out.push_back("// real answer there - the caller did not say which - so a claim that stored 0");
    out.push_back(
        "// would leave the endpoint still looking unclaimed, and two threads of one game");
    out.push_back("// asking for the same version with no handle would both succeed on the first");
    out.push_back(
        "// endpoint and be handed the same object, which is the opposite of what this is");
    out.push_back("// for.");
    out.push_back(
        "void* interface_object(const char* version, std::int32_t hSteamUser) noexcept {");
    out.push_back("    if (version == nullptr) {");
    out.push_back("        return nullptr;");
    out.push_back("    }");
    out.push_back("    for (std::size_t index = 0; index < kVersionCount; ++index) {");
    out.push_back("        if (std::strcmp(kVersions[index].version, version) != 0) {");
    out.push_back("            continue;");
    out.push_back("        }");
    out.push_back(
        "        for (std::size_t endpoint = 0; endpoint < steammock::kInterfaceEndpoints;");
    out.push_back("             ++endpoint) {");
    out.push_back("            if (hSteamUser != 0 && kVersions[index].user[endpoint]->load() == "
                  "hSteamUser) {");
    out.push_back("                return kVersions[index].object[endpoint];");
    out.push_back("            }");
    out.push_back("        }");
    out.push_back(
        "        for (std::size_t endpoint = 0; endpoint < steammock::kInterfaceEndpoints;");
    out.push_back("             ++endpoint) {");
    out.push_back("            bool unclaimed = false;");
    out.push_back("            if (!kVersions[index].claimed[endpoint]->compare_exchange_strong(");
    out.push_back("                    unclaimed, true)) {");
    out.push_back("                continue;");
    out.push_back("            }");
    out.push_back("            kVersions[index].user[endpoint]->store(hSteamUser);");
    out.push_back("            return kVersions[index].object[endpoint];");
    out.push_back("        }");
    out.push_back("        // Both are spoken for, which takes two game servers in one process to");
    out.push_back("        // reach. A stub has nothing better to say than the first of them.");
    out.push_back("        return kVersions[index].object[0];");
    out.push_back("    }");
    out.push_back("    return nullptr;");
    out.push_back("}");
    out.push_back("");
    out.push_back("}  // namespace steammock");
    return joined(out) + "\n";
}

}  // namespace steammock
