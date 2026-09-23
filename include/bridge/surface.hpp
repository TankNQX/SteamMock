#pragma once

#include <cstddef>

namespace steammock {

// ---------------------------------------------------------------------------
//  What the stub exports, as the backend sees it.
// ---------------------------------------------------------------------------
//  This is the hand-written shape; the table itself is generated from
//  gen/steam_api.idl.json into src/generated/api_surface.cpp, so `--list-api`
//  can print the surface without reading a file at run time and without the
//  backend having to parse the IDL.
//
//  The backend does not need this to answer calls: the state machine knows the
//  calls it handles by name, and a scenario names the rest. It exists so a
//  person can ask "what does this stub actually export?" and so a test can prove
//  every call the state machine answers is really in the IDL.

struct SurfaceParam {
    const char* name;
    const char* type;
    bool out;
};

struct SurfaceCall {
    const char* name;
    const char* returns;
    const SurfaceParam* params;
    std::size_t param_count;
};

const char* api_surface_name() noexcept;
int api_surface_revision() noexcept;
const SurfaceCall* api_surface_calls(std::size_t& count) noexcept;

}  // namespace steammock
