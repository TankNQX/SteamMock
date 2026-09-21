#pragma once

#include <string>
#include <vector>

#include "bridge/json.hpp"

namespace steambridge {

// ---------------------------------------------------------------------------
//  gen/steam_api.idl.json - the one file to edit to add a call.
// ---------------------------------------------------------------------------
//  This is the generator, in C++ so the build needs one toolchain rather than
//  two. It turns the IDL into the stub's trampolines, the export list, and the
//  surface table the backend prints with --list-api.
//
//  The rendering is a pure function of the IDL, deliberately: a test can render
//  everything and compare it with what is committed, which is what catches an
//  IDL edit that was never regenerated.

struct IdlParam {
    std::string name;
    std::string type;
    bool out = false;
};

struct IdlCall {
    std::string name;
    std::string returns = "void";
    std::vector<IdlParam> params;
};

class Idl {
public:
    // Parses and validates. `error` describes the first problem found.
    static bool load_file(const std::string& path, Idl& out, std::string& error);
    static bool from_json(const Json& document, Idl& out, std::string& error);

    const std::string& surface() const noexcept { return _surface; }
    int revision() const noexcept { return _revision; }

    // Sorted by name: the order every generated file is written in.
    const std::vector<IdlCall>& calls() const noexcept { return _calls; }

private:
    std::string _surface = "?";
    int _revision = 0;
    std::vector<IdlCall> _calls;
};

std::string render_api_stub(const Idl& idl);
std::string render_exports_def(const Idl& idl);
std::string render_api_surface(const Idl& idl);

}  // namespace steambridge
