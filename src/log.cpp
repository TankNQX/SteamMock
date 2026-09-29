#include "bridge/log.hpp"

#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <memory>

#include <windows.h>

namespace steammock {
namespace {

// One configuration: the level, the file and the prefix a line is written with.
// It is made once and never changed after that, which is what makes it safe to
// read from every thread without a lock - a line costs one atomic load, and no
// reader can ever see a configuration half-made.
struct Config {
    LogLevel level = LogLevel::info;
    FILE* file = nullptr;
    std::string prefix;
};

// The configuration in force, or null while nobody has made one. Whoever
// publishes it does so when it is whole; what is published lives for the life of
// the process, which for a logger configured once is what a singleton is for.
std::atomic<const Config*> g_config{nullptr};

// What logging reads before anything configures - and in a process where nothing
// ever does. The same level and the same silence the globals started with.
const Config& config() noexcept {
    static const Config defaults;
    const Config* published = g_config.load(std::memory_order_acquire);
    return published != nullptr ? *published : defaults;
}

LogLevel parse_level(const char* text) noexcept {
    if (text == nullptr) {
        return LogLevel::info;
    }
    const std::string_view name(text);
    if (name == "error") {
        return LogLevel::error;
    }
    if (name == "warn" || name == "warning") {
        return LogLevel::warn;
    }
    if (name == "debug" || name == "trace") {
        return LogLevel::debug;
    }
    return LogLevel::info;
}

const char* level_name(LogLevel level) noexcept {
    switch (level) {
        case LogLevel::error: return "error";
        case LogLevel::warn: return "warn";
        case LogLevel::info: return "info";
        case LogLevel::debug: return "debug";
    }
    return "info";
}

}  // namespace

const char* log_level_name(LogLevel level) noexcept { return level_name(level); }

bool log_enabled(LogLevel level) noexcept {
    return static_cast<int>(level) <= static_cast<int>(config().level);
}

void log_write(LogLevel level, std::string_view message) noexcept {
    // Logging must never break the program. A failure here - an allocator that
    // cannot build the line - loses the line rather than terminating the game
    // this DLL is loaded into, which is what the catch below is for. Without it
    // the noexcept would mean "terminate on failure", the opposite of the
    // promise.
    try {
        // One read of the configuration, for both the level and the line. Two - a
        // `log_enabled(level)` and then this - could straddle the publication of the first
        // configuration by another thread, so the filter judged against the defaults while
        // the line went to a file and a prefix that had just been installed: a line either
        // written when the published level disables it, or dropped when it does not.
        const Config& settings = config();
        if (static_cast<int>(level) > static_cast<int>(settings.level)) {
            return;
        }
        std::string line;
        line.reserve(settings.prefix.size() + message.size() + 40u);
        line += "[steammock] ";
        line += level_name(level);
        line += ": ";
        if (!settings.prefix.empty()) {
            line += settings.prefix;
            line += ": ";
        }
        line.append(message.data(), message.size());

        line += "\r\n";
        // Visible in a debugger without a console, which is how these runs
        // usually get inspected.
        OutputDebugStringA(line.c_str());

        if (settings.file != nullptr) {
            std::fputs(line.c_str(), settings.file);
            std::fflush(settings.file);
        }
    } catch (...) {
        return;  // the line is lost; the process is not
    }
}

void log_configure(const char* module_path) noexcept {
    // Same promise as log_write: a logger that cannot configure itself keeps
    // quiet rather than taking the process with it.
    //
    // The first caller to publish wins. Two of them are possible - another
    // library in the same process asking too, or two threads of the game - and
    // whoever loses keeps the winner's configuration, because a level and a file
    // are the process's rather than a caller's. What it built is nobody's: its
    // file is closed rather than left open, and the configuration it never
    // published goes with it.
    if (g_config.load(std::memory_order_acquire) != nullptr) {
        return;
    }
    try {
        // Both owned by a guard until the configuration is published, because everything
        // between building them and publishing them can throw - the prefix is a std::string
        // built from the environment and the module path. They used to be released by hand on
        // the path that lost the race and by nobody at all on any other way out, so an
        // exception there leaked the configuration and left the log file open.
        std::unique_ptr<Config> built(new Config());
        // The file is held apart from the configuration: the Config outlives this function
        // once it is published, and the handle inside it is what has to be closed only when it
        // is not.
        struct FileHandle {
            std::FILE* file = nullptr;
            ~FileHandle() {
                if (file != nullptr) {
                    std::fclose(file);
                }
            }
        } opened;

        built->level = parse_level(std::getenv("STEAMMOCK_LOG_LEVEL"));

        if (const char* path = std::getenv("STEAMMOCK_LOG")) {
            if (path[0] != '\0') {
                opened.file = std::fopen(path, "ab");
                if (opened.file == nullptr) {
                    // Nothing else can report this: the logger is what would. A debugger
                    // watching the process is the one place left to say it, and the
                    // alternative is a run whose log never reached the file it was told to
                    // write - which is exactly the kind of silence this whole file exists to
                    // avoid.
                    OutputDebugStringA(
                        "[steammock] error: cannot open the log file STEAMMOCK_LOG names\r\n");
                }
            }
        }
        built->file = opened.file;

        char buffer[32] = {};
        std::snprintf(buffer, sizeof(buffer), "pid %lu",
                      static_cast<unsigned long>(GetCurrentProcessId()));
        built->prefix = buffer;
        if (module_path != nullptr && module_path[0] != '\0') {
            const std::string_view path(module_path);
            const std::size_t slash = path.find_last_of("\\/");
            built->prefix += " ";
            if (slash == std::string_view::npos) {
                built->prefix.append(path.data(), path.size());
            } else {
                built->prefix.append(path.data() + slash + 1u, path.size() - slash - 1u);
            }
        }

        const Config* expected = nullptr;
        if (g_config.compare_exchange_strong(expected, built.get(), std::memory_order_release,
                                             std::memory_order_acquire)) {
            // Published, so the process owns both now - for the life of the process, which is
            // what a singleton is for. The guards are what let this be the only path that
            // keeps them.
            (void)built.release();
            opened.file = nullptr;
        }
    } catch (...) {
        // A logger that cannot configure itself says so where a person can still see it: the
        // line the logger would have written is not available to it, and losing the fact
        // entirely is how a run is debugged against a log that was never written.
        OutputDebugStringA("[steammock] error: the logger could not be configured\r\n");
        return;  // logging without a prefix is better than not running
    }
}

}  // namespace steammock
