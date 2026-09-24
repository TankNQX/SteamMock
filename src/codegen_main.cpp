// ---------------------------------------------------------------------------
//  steammock_codegen - turn the two data files into the generated files.
// ---------------------------------------------------------------------------
//  gen/steam_api.idl.json is the source of truth for what the stub exports, and
//  gen/steam_interfaces.json for the interface versions it hands out. Adding a
//  call means adding one entry there, and adding an interface version means the
//  same in the other file; either way, then:
//
//    steammock_codegen            # rewrite the generated files
//    steammock_codegen --check    # fail if they are stale (used by CI)
//
//  Outputs, all under <root>/src/generated/:
//    api_stub.cpp             one trampoline per call
//    steam_api_exports.def    the export list, so names stay undecorated
//    api_surface.cpp          the table the backend prints with --list-api
//    api_interfaces.cpp       the objects the stub hands out for a version string
//
//  Written as bytes and compared as bytes, because the files are committed and
//  line endings must not depend on the platform that ran the generator.

#include <cstdio>
#include <new>
#include <string>
#include <utility>
#include <vector>

#include "bridge/idl.hpp"
#include "bridge/interfaces.hpp"

namespace {

const char* const kDefaultIdl = "gen/steam_api.idl.json";
const char* const kDefaultInterfaces = "gen/steam_interfaces.json";

bool read_file_bytes(const std::string& path, std::string& out) {
    std::FILE* file = std::fopen(path.c_str(), "rb");
    if (file == nullptr) {
        return false;
    }
    char buffer[4096];
    std::size_t got = 0;
    while ((got = std::fread(buffer, 1, sizeof(buffer), file)) > 0) {
        out.append(buffer, got);
    }
    std::fclose(file);
    return true;
}

bool file_exists(const std::string& path) {
    std::FILE* file = std::fopen(path.c_str(), "rb");
    if (file == nullptr) {
        return false;
    }
    std::fclose(file);
    return true;
}

bool write_file_bytes(const std::string& path, const std::string& text) {
    std::FILE* file = std::fopen(path.c_str(), "wb");
    if (file == nullptr) {
        return false;
    }
    const std::size_t written = text.empty() ? 0u : std::fwrite(text.data(), 1, text.size(), file);
    std::fclose(file);
    return written == text.size();
}

std::string parent_of(const std::string& path) {
    const std::size_t slash = path.find_last_of("\\/");
    return slash == std::string::npos ? std::string() : path.substr(0, slash);
}

std::string join_path(const std::string& directory, const std::string& name) {
    if (directory.empty()) {
        return name;
    }
    const char last = directory.back();
    return directory + (last == '/' || last == '\\' ? "" : "/") + name;
}

void print_usage(std::FILE* out) {
    std::fprintf(out,
                 "steammock_codegen - regenerate the stub's trampolines, export list and "
                 "interfaces\n\n"
                 "usage: steammock_codegen [--idl FILE] [--interfaces FILE] [--root DIR] "
                 "[--check]\n\n"
                 "  --idl FILE         the API surface to read (default %s)\n"
                 "  --interfaces FILE  the interface layouts to read (default %s)\n"
                 "  --root DIR         the checkout the outputs belong to (default: the parent of\n"
                 "                     the IDL's directory)\n"
                 "  --check            do not write; fail if the generated files are out of date\n",
                 kDefaultIdl, kDefaultInterfaces);
}

}  // namespace

int run(int argc, char** argv) {
    std::string idl_path = kDefaultIdl;
    std::string interfaces_path = kDefaultInterfaces;
    std::string root;
    bool root_given = false;
    bool check = false;

    for (int index = 1; index < argc; ++index) {
        const std::string argument = argv[index];
        const auto take = [&](std::string& target) {
            if (index + 1 >= argc) {
                return false;
            }
            target = argv[++index];
            return true;
        };

        if (argument == "-h" || argument == "--help") {
            print_usage(stdout);
            return 0;
        }
        if (argument == "--check") {
            check = true;
            continue;
        }
        if (argument == "--idl") {
            if (!take(idl_path)) {
                std::fprintf(stderr, "steammock_codegen: --idl needs a value\n");
                return 2;
            }
            continue;
        }
        if (argument == "--interfaces") {
            if (!take(interfaces_path)) {
                std::fprintf(stderr, "steammock_codegen: --interfaces needs a value\n");
                return 2;
            }
            continue;
        }
        if (argument == "--root") {
            if (!take(root)) {
                std::fprintf(stderr, "steammock_codegen: --root needs a value\n");
                return 2;
            }
            root_given = true;
            continue;
        }
        std::fprintf(stderr, "steammock_codegen: unknown option '%s'\n", argument.c_str());
        print_usage(stderr);
        return 2;
    }

    if (!root_given) {
        // "gen/steam_api.idl.json" means the checkout is one level above gen/,
        // which is how the tool is run from the repository root.
        root = parent_of(parent_of(idl_path));
    }

    steammock::Idl idl;
    std::string error;
    if (!steammock::Idl::load_file(idl_path, idl, error)) {
        std::fprintf(stderr, "idl error: %s\n", error.c_str());
        return 2;
    }

    steammock::Interfaces interfaces;
    if (!file_exists(interfaces_path)) {
        // The layouts are Valve's own data and are deliberately not part of the
        // checkout: whoever builds this imports theirs from an SDK they have (see
        // tools/steamworks_sdk_import.py). Without one there is no version string
        // to hand an object out for, which is the same answer a game gets from a
        // stub that does not know the version - so this is a note, not a failure.
        std::fprintf(stderr,
                     "steammock_codegen: no interface layouts at %s\n"
                     "  the stub will hand out no interface objects; import yours with\n"
                     "  python tools/steamworks_sdk_import.py --sdk <sdk>/public/steam "
                     "--out %s\n",
                     interfaces_path.c_str(), interfaces_path.c_str());
    } else if (!steammock::Interfaces::load_file(interfaces_path, interfaces, error)) {
        std::fprintf(stderr, "interfaces error: %s\n", error.c_str());
        return 2;
    }

    const std::vector<std::pair<std::string, std::string>> outputs = {
        {join_path(root, "src/generated/api_stub.cpp"), steammock::render_api_stub(idl)},
        {join_path(root, "src/generated/steam_api_exports.def"),
         steammock::render_exports_def(idl)},
        {join_path(root, "src/generated/api_surface.cpp"), steammock::render_api_surface(idl)},
        {join_path(root, "src/generated/api_interfaces.cpp"),
         steammock::render_api_interfaces(interfaces)},
    };

    std::vector<std::string> stale;
    for (const auto& output : outputs) {
        const std::string& path = output.first;
        if (check) {
            std::string current;
            if (!read_file_bytes(path, current) || current != output.second) {
                stale.push_back(path);
            }
            continue;
        }
        if (!write_file_bytes(path, output.second)) {
            std::fprintf(stderr, "cannot write %s\n", path.c_str());
            return 2;
        }
        std::printf("wrote %s\n", path.c_str());
    }

    if (check) {
        if (!stale.empty()) {
            std::fprintf(stderr, "stale generated files (run: steammock_codegen):\n");
            for (const std::string& path : stale) {
                std::fprintf(stderr, "  %s\n", path.c_str());
            }
            return 1;
        }
        std::printf("generated files are up to date\n");
    }
    return 0;
}

// As in backend_main: an exception escaping main would terminate silently, and
// a failed allocation is the only realistic way to get one.
int main(int argc, char** argv) {
    try {
        return run(argc, argv);
    } catch (const std::bad_alloc&) {
        std::fprintf(stderr, "steammock_codegen: out of memory\n");
        return 2;
    } catch (...) {
        std::fprintf(stderr, "steammock_codegen: unexpected failure\n");
        return 2;
    }
}
