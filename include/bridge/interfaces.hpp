#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "bridge/json.hpp"

namespace steambridge {

// ---------------------------------------------------------------------------
//  gen/steam_interfaces.json - the interface layouts the stub hands out.
// ---------------------------------------------------------------------------
//  Where gen/steam_api.idl.json is hand-written (one entry per exported call),
//  this file is hand-maintained data: one entry per interface *version*, with the
//  slots in vtable order and the argument kinds each one carries. It was imported
//  from Steamworks SDK headers when the surface was laid out, and adding a version
//  means editing it - `steambridge_codegen` validates every kind, name and size in
//  it before it will generate anything from it. It is the input to the synthesized
//  objects - see bridge/synth.hpp for what they are and why a generated slot body
//  is one line.
//
//  A version's slots are rows rather than one object per slot, which is what
//  keeps a file of two thousand slots to two thousand lines. The shape of a row
//  is written down where it is read - see src/interfaces.cpp, "Reading a slot" -
//  and the long version is a method, the type it returns, its parameters, and an
//  object of notes for the rare slot that needs one.

struct InterfaceParam {
    std::string name;
    std::string kind;  // the wire kind this argument travels as
    std::string decl;  // for value and struct kinds, the type it names
    std::string cpp;   // the declaration this resolves to, pointer included
    bool out = false;
    bool opaque = false;  // a buffer the wire can report but not carry
};

struct InterfaceSlot {
    std::string method;
    std::string call;  // the name the call travels under on the wire
    std::string returns;
    std::string returns_decl;
    std::string returns_cpp;  // "void", or the declaration the return resolves to
    std::vector<InterfaceParam> params;
    bool destructor = false;

    // Recorded from the file and carried through the generator, but nothing
    // writes a declaration from either of them: they say what the wire cannot
    // carry, which is a fact about the SDK's method rather than about the C++
    // this writes for it. They are kept because the file is the record.
    bool private_api = false;            // STEAM_PRIVATE_API: owns a slot, has no flat name
    bool returns_unmarshalable = false;  // the wire cannot carry what is returned
};

struct InterfaceVersion {
    std::string name;     // the interface's class name, for reporting
    std::string version;  // the string a game asks CreateInterface for
    std::vector<InterfaceSlot> slots;
};

// A structure a declaration needs: the members are what its ABI depends on, and
// the size is what the layouts were imported with. Nothing reads the bytes.
struct InterfaceStructure {
    std::string name;
    std::vector<std::pair<std::string, std::string>> members;
    int size = 0;
};

// A class passed and returned by value - CSteamID, CGameID - which the wire
// carries as the single integer it is.
struct InterfaceValueType {
    std::string name;
    std::string wire;
    std::string member;
    int size = 0;
};

class Interfaces {
public:
    // Parses and validates. `error` describes the first problem found, and a
    // kind or a declaration this cannot write is one of them: the generator
    // refusing is better than a compile error in a generated file.
    static bool load_file(const std::string& path, Interfaces& out, std::string& error);
    static bool from_json(const Json& document, Interfaces& out, std::string& error);

    const std::vector<InterfaceVersion>& versions() const noexcept { return _versions; }
    const std::vector<InterfaceStructure>& structures() const noexcept { return _structures; }
    const std::vector<InterfaceValueType>& value_types() const noexcept { return _value_types; }

private:
    std::vector<InterfaceVersion> _versions;
    std::vector<InterfaceStructure> _structures;
    std::vector<InterfaceValueType> _value_types;
};

std::string render_api_interfaces(const Interfaces& interfaces);

}  // namespace steambridge
