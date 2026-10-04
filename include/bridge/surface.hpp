#pragma once

#include <cstddef>

namespace steammock
{

// ---------------------------------------------------------------------------
//  What the stub exports, as the backend sees it.
// ---------------------------------------------------------------------------
//  This is the hand-written shape; the table itself is generated from
//  gen/steam_api_surface.json into src/generated/api_surface.cpp, so `--list-api`
//  can print the surface without reading a file at run time and without the
//  backend having to parse the IDL.
//
//  The backend does not need this to answer calls: the state machine knows the
//  calls it handles by name, and a scenario names the rest. It exists so a
//  person can ask "what does this stub actually export?" and so a test can prove
//  every call the state machine answers is really in the IDL.

struct SurfaceParam
{
    const char* name;
    const char* type;
    bool out;
};

struct SurfaceCall
{
    const char* name;
    const char* returns;
    // The interface whose slots travel under this name, empty when the name is one of the
    // top-level `SteamAPI_*` entry points rather than a slot of an interface.
    const char* interface_name;
    // How many slots travel under this name. More than one is the case a reader cannot see for
    // themselves: a C function cannot be overloaded, so `GetStat`'s int32 slot and its float slot
    // both answer from the one entry behind the name. An import that resolves each overload to the
    // name the SDK's own flat header gives it leaves every name with one slot, which is the good
    // state and shows no mark at all.
    std::size_t overloads;
    const SurfaceParam* params;
    std::size_t param_count;
};

const char* api_surface_name() noexcept;
const SurfaceCall* api_surface_calls(std::size_t& count) noexcept;

} // namespace steammock
