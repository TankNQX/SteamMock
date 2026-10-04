// ---------------------------------------------------------------------------
//  steammock_codegen - turn the two data files into the generated files.
// ---------------------------------------------------------------------------
//  gen/steam_api_surface.json is the source of truth for what the stub exports, and
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

namespace
{

const char* const kDefaultIdl = "gen/steam_api_surface.json";
const char* const kDefaultInterfaces = "gen/steam_interfaces.json";

// A FILE* that closes what it was given, on every way out of the function that owns it.
// The readers below used to close by hand, so an exception from the string they append to
// left the handle open - which for a generator run once per build is a leaked handle rather
// than a wrong file, but it is the same shape of bug as the one that used to truncate the
// outputs.
struct FileCloser
{
    std::FILE* file = nullptr;
    ~FileCloser()
    {
        if (file != nullptr)
        {
            std::fclose(file);
        }
    }
    FileCloser(const FileCloser&) = delete;
    FileCloser& operator=(const FileCloser&) = delete;
};

bool read_file_bytes(const std::string& path, std::string& out)
{
    std::FILE* const opened = std::fopen(path.c_str(), "rb");
    if (opened == nullptr)
    {
        return false;
    }
    FileCloser closer{opened};
    char buffer[4096];
    std::size_t got = 0;
    while ((got = std::fread(buffer, 1, sizeof(buffer), opened)) > 0)
    {
        out.append(buffer, got);
    }
    // Which of the two ended the loop: end of file, or an error. The count coming back
    // short was read as "that was all of it" either way, so a read that failed halfway
    // returned a truncated file as a success - and in --check mode that is a file compared
    // against the wrong bytes.
    return std::ferror(opened) == 0;
}

bool file_exists(const std::string& path)
{
    std::FILE* const opened = std::fopen(path.c_str(), "rb");
    if (opened == nullptr)
    {
        return false;
    }
    std::fclose(opened);
    return true;
}

bool write_file_bytes(const std::string& path, const std::string& text)
{
    // To a temporary beside the destination and renamed over it, rather than truncated in
    // place: these are files the build reads as inputs, and a `fwrite` that fails partway -
    // or a process that dies mid-write - used to leave the previous version gone and a
    // half-written one in its place, with the build then compiling the half.
    const std::string temporary = path + ".tmp";
    std::FILE* file = std::fopen(temporary.c_str(), "wb");
    if (file == nullptr)
    {
        return false;
    }
    const std::size_t written = text.empty() ? 0u : std::fwrite(text.data(), 1, text.size(), file);
    // fclose's result is the one that says whether what was buffered reached the disk: a
    // write that only fails at flush time (a full disk) used to be reported as success.
    const bool flushed = std::fclose(file) == 0;
    if (written != text.size() || !flushed)
    {
        std::remove(temporary.c_str());
        return false;
    }
    std::remove(path.c_str());
    if (std::rename(temporary.c_str(), path.c_str()) != 0)
    {
        std::remove(temporary.c_str());
        return false;
    }
    return true;
}

std::string parent_of(const std::string& path)
{
    const std::size_t slash = path.find_last_of("\\/");
    return slash == std::string::npos ? std::string() : path.substr(0, slash);
}

std::string join_path(const std::string& directory, const std::string& name)
{
    if (directory.empty())
    {
        return name;
    }
    const char last = directory.back();
    return directory + (last == '/' || last == '\\' ? "" : "/") + name;
}

void print_usage(std::FILE* out)
{
    std::fprintf(out,
                 "steammock_codegen - regenerate the stub's trampolines, export list and "
                 "interfaces\n\n"
                 "usage: steammock_codegen [--idl FILE] [--interfaces FILE] [--root DIR] "
                 "[--check]\n\n"
                 "  --idl FILE         the API surface to read (default %s)\n"
                 "  --interfaces FILE  the interface layouts to read (default %s)\n"
                 "  --root DIR         the checkout the outputs belong to (default: the parent of\n"
                 "                     the surface's directory, or the current directory when the\n"
                 "                     surface path is relative)\n"
                 "  --check            do not write; fail if the generated files are out of date\n",
                 kDefaultIdl, kDefaultInterfaces);
}

} // namespace

int run(int argc, char** argv)
{
    std::string idl_path = kDefaultIdl;
    std::string interfaces_path = kDefaultInterfaces;
    std::string root;
    bool root_given = false;
    bool check = false;

    for (int index = 1; index < argc; ++index)
    {
        const std::string argument = argv[index];
        const auto take = [&](std::string& target)
        {
            if (index + 1 >= argc)
            {
                return false;
            }
            target = argv[++index];
            return true;
        };

        if (argument == "-h" || argument == "--help")
        {
            print_usage(stdout);
            return 0;
        }
        if (argument == "--check")
        {
            check = true;
            continue;
        }
        if (argument == "--idl")
        {
            if (!take(idl_path))
            {
                std::fprintf(stderr, "steammock_codegen: --idl needs a value\n");
                return 2;
            }
            continue;
        }
        if (argument == "--interfaces")
        {
            if (!take(interfaces_path))
            {
                std::fprintf(stderr, "steammock_codegen: --interfaces needs a value\n");
                return 2;
            }
            continue;
        }
        if (argument == "--root")
        {
            if (!take(root))
            {
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

    if (!root_given)
    {
        // Where the outputs belong. The default surface path is relative and one directory
        // below the checkout - "gen/steam_api_surface.json" means the root is the directory
        // above gen/ - so for it, the root is the current directory, which is what running
        // this from the checkout root means. An absolute path says the same thing wherever
        // it is run from, and a relative path with a directory in it names its own root.
        //
        // "." rather than an empty string: nothing downstream could tell "the checkout is
        // here" from "the root was never established", and a run from the wrong directory
        // then wrote a checkout-shaped tree beside itself and said "wrote src/generated/...",
        // which reads as if it had gone where it belonged.
        const std::string derived = parent_of(parent_of(idl_path));
        root = derived.empty() ? "." : derived;
    }

    steammock::Idl idl;
    std::string error;
    if (!file_exists(idl_path))
    {
        // The surface is Valve's own API too, and is deliberately not part of the
        // checkout either: whoever builds this imports theirs from an SDK they have,
        // with `--surface`. Without one the stub exports nothing at all, which the
        // tests say out loud rather than failing on - so this is a note, and the
        // same shape as the layouts below.
        std::fprintf(stderr,
                     "steammock_codegen: no API surface at %s\n"
                     "  the stub will export nothing; import yours with\n"
                     "  python tools/steamworks_sdk_import.py --sdk <sdk>/public/steam "
                     "--surface %s\n",
                     idl_path.c_str(), idl_path.c_str());
    }
    else if (!steammock::Idl::load_file(idl_path, idl, error))
    {
        std::fprintf(stderr, "surface error: %s\n", error.c_str());
        return 2;
    }

    steammock::Interfaces interfaces;
    if (!file_exists(interfaces_path))
    {
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
    }
    else if (!steammock::Interfaces::load_file(interfaces_path, interfaces, error))
    {
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
    for (const auto& output : outputs)
    {
        const std::string& path = output.first;
        if (check)
        {
            if (!file_exists(path))
            {
                // Not there at all is what stale means, and it is how a checkout that has
                // never run the generator reports every output.
                stale.push_back(path);
                continue;
            }
            std::string current;
            if (!read_file_bytes(path, current))
            {
                // ...while a file that is there and cannot be read is not staleness at all.
                // This used to be the same answer - "run the generator" - for a missing
                // file, a permission problem and a read error, which sends whoever reads
                // the CI log to fix the wrong thing.
                std::fprintf(stderr, "cannot read %s\n", path.c_str());
                return 2;
            }
            if (current != output.second)
            {
                stale.push_back(path);
            }
            continue;
        }
        if (!write_file_bytes(path, output.second))
        {
            std::fprintf(stderr, "cannot write %s\n", path.c_str());
            return 2;
        }
        std::printf("wrote %s\n", path.c_str());
    }

    if (check)
    {
        if (!stale.empty())
        {
            std::fprintf(stderr, "stale generated files (run: steammock_codegen):\n");
            for (const std::string& path : stale)
            {
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
int main(int argc, char** argv)
{
    try
    {
        return run(argc, argv);
    }
    catch (const std::bad_alloc&)
    {
        std::fprintf(stderr, "steammock_codegen: out of memory\n");
        return 2;
    }
    catch (...)
    {
        std::fprintf(stderr, "steammock_codegen: unexpected failure\n");
        return 2;
    }
}
