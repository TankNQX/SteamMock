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
    // How many slots one version of that interface gives this method. More than one is an
    // overload: a C function cannot be overloaded, so `GetStat`'s int32 slot and its float slot
    // travel under the one name and an override on the name covers both. One means the name is
    // the method's own.
    std::size_t overloads;
    const SurfaceParam* params;
    std::size_t param_count;
};

const char* api_surface_name() noexcept;
const SurfaceCall* api_surface_calls(std::size_t& count) noexcept;

} // namespace steammock
