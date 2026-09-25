#include "bridge/log.hpp"

#include <atomic>
#include <cstdio>
#include <cstdlib>

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
        if (!log_enabled(level)) {
            return;
        }
        // Held for the whole line: the configuration cannot change under it,
        // because nothing changes a configuration once it is published.
        const Config& settings = config();
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
        Config* built = new Config();
        built->level = parse_level(std::getenv("STEAMMOCK_LOG_LEVEL"));

        if (const char* path = std::getenv("STEAMMOCK_LOG")) {
            if (path[0] != '\0') {
                built->file = std::fopen(path, "ab");
            }
        }

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
        if (!g_config.compare_exchange_strong(expected, built, std::memory_order_release,
                                              std::memory_order_acquire)) {
            if (built->file != nullptr) {
                std::fclose(built->file);
            }
            delete built;
        }
    } catch (...) {
        return;  // logging without a prefix is better than not running
    }
}

}  // namespace steammock
