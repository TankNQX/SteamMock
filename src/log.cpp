#include "bridge/log.hpp"

#include <cstdio>
#include <cstdlib>

#if defined(_WIN32)
#  include <windows.h>
#else
#  include <unistd.h>
#endif

namespace steambridge {
namespace {

LogLevel g_level = LogLevel::info;
FILE* g_file = nullptr;
bool g_configured = false;
std::string g_prefix;

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
    return static_cast<int>(level) <= static_cast<int>(g_level);
}

void log_write(LogLevel level, std::string_view message) noexcept {
    if (!log_enabled(level)) {
        return;
    }
    std::string line;
    line.reserve(g_prefix.size() + message.size() + 40u);
    line += "[steambridge] ";
    line += level_name(level);
    line += ": ";
    if (!g_prefix.empty()) {
        line += g_prefix;
        line += ": ";
    }
    line.append(message.data(), message.size());

#if defined(_WIN32)
    line += "\r\n";
    // Visible in a debugger without a console, which is how these runs usually
    // get inspected.
    OutputDebugStringA(line.c_str());
#else
    line += "\n";
    std::fputs(line.c_str(), stderr);
#endif

    if (g_file != nullptr) {
        std::fputs(line.c_str(), g_file);
        std::fflush(g_file);
    }
}

void log_configure(const char* module_path) noexcept {
    if (g_configured) {
        return;
    }
    g_configured = true;
    g_level = parse_level(std::getenv("STEAMBRIDGE_LOG_LEVEL"));

    if (const char* path = std::getenv("STEAMBRIDGE_LOG")) {
        if (path[0] != '\0') {
            g_file = std::fopen(path, "ab");
        }
    }

#if defined(_WIN32)
    char buffer[32] = {};
    std::snprintf(buffer, sizeof(buffer), "pid %lu", static_cast<unsigned long>(GetCurrentProcessId()));
    g_prefix = buffer;
#else
    g_prefix = "pid " + std::to_string(static_cast<long>(getpid()));
#endif
    if (module_path != nullptr && module_path[0] != '\0') {
        const std::string_view path(module_path);
        const std::size_t slash = path.find_last_of("\\/");
        g_prefix += " ";
        if (slash == std::string_view::npos) {
            g_prefix.append(path.data(), path.size());
        } else {
            g_prefix.append(path.data() + slash + 1u, path.size() - slash - 1u);
        }
    }
}

}  // namespace steambridge
