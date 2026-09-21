// ---------------------------------------------------------------------------
//  steambridge_codegen - turn gen/steam_api.idl.json into the generated files.
// ---------------------------------------------------------------------------
//  The IDL is the single source of truth for what the stub exports. Adding a
//  call means adding one entry there and regenerating:
//
//    steambridge_codegen            # rewrite the generated files
//    steambridge_codegen --check    # fail if they are stale (used by CI)
//
//  Outputs, all under <root>/src/generated/:
//    api_stub.cpp             one trampoline per call
//    steam_api_exports.def    the export list, so names stay undecorated
//    api_surface.cpp          the table the backend prints with --list-api
//
//  Written as bytes and compared as bytes, because the files are committed and
//  line endings must not depend on the platform that ran the generator.

#include <cstdio>
#include <new>
#include <string>
#include <utility>
#include <vector>

#include "bridge/idl.hpp"

namespace {

const char* const kDefaultIdl = "gen/steam_api.idl.json";

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
                 "steambridge_codegen - regenerate the stub's trampolines and export list\n\n"
                 "usage: steambridge_codegen [--idl FILE] [--root DIR] [--check]\n\n"
                 "  --idl FILE   the API surface to read (default %s)\n"
                 "  --root DIR   the checkout the outputs belong to (default: the parent of\n"
                 "               the IDL's directory)\n"
                 "  --check      do not write; fail if the generated files are out of date\n",
                 kDefaultIdl);
}

}  // namespace

int run(int argc, char** argv) {
    std::string idl_path = kDefaultIdl;
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
                std::fprintf(stderr, "steambridge_codegen: --idl needs a value\n");
                return 2;
            }
            continue;
        }
        if (argument == "--root") {
            if (!take(root)) {
                std::fprintf(stderr, "steambridge_codegen: --root needs a value\n");
                return 2;
            }
            root_given = true;
            continue;
        }
        std::fprintf(stderr, "steambridge_codegen: unknown option '%s'\n", argument.c_str());
        print_usage(stderr);
        return 2;
    }

    if (!root_given) {
        // "gen/steam_api.idl.json" means the checkout is one level above gen/,
        // which is how the tool is run from the repository root.
        root = parent_of(parent_of(idl_path));
    }

    steambridge::Idl idl;
    std::string error;
    if (!steambridge::Idl::load_file(idl_path, idl, error)) {
        std::fprintf(stderr, "idl error: %s\n", error.c_str());
        return 2;
    }

    const std::vector<std::pair<std::string, std::string>> outputs = {
        {join_path(root, "src/generated/api_stub.cpp"), steambridge::render_api_stub(idl)},
        {join_path(root, "src/generated/steam_api_exports.def"),
         steambridge::render_exports_def(idl)},
        {join_path(root, "src/generated/api_surface.cpp"), steambridge::render_api_surface(idl)},
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
            std::fprintf(stderr, "stale generated files (run: steambridge_codegen):\n");
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
        std::fprintf(stderr, "steambridge_codegen: out of memory\n");
        return 2;
    } catch (...) {
        std::fprintf(stderr, "steambridge_codegen: unexpected failure\n");
        return 2;
    }
}
