#pragma once

#include <string>

namespace steambridge {

// ---------------------------------------------------------------------------
//  The stub's own log, for when the backend is unreachable or slow.
// ---------------------------------------------------------------------------
//  Everything goes to the debugger (on Windows) and, when STEAMBRIDGE_LOG names
//  a file, to that file as well. The level comes from STEAMBRIDGE_LOG_LEVEL
//  (error, warn, info, debug; default info).

enum class LogLevel : int { error = 0, warn = 1, info = 2, debug = 3 };

void log_write(LogLevel level, std::string_view message) noexcept;
bool log_enabled(LogLevel level) noexcept;
void log_configure(const char* module_path) noexcept;   // reads the environment once

}  // namespace steambridge
