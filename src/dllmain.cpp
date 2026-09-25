// ---------------------------------------------------------------------------
//  The stub DLL.
// ---------------------------------------------------------------------------
//  The exported Steam API functions themselves live in the generated
//  src/generated/api_stub.cpp; this file owns what is not generated: the DLL
//  entry point, and a couple of our own exports that make the harness's state
//  observable from outside (a test, or dumpbin).

#ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "bridge/client.hpp"
#include "bridge/export.hpp"
#include "bridge/log.hpp"

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {
    (void)reserved;
    if (reason == DLL_PROCESS_ATTACH) {
        // Nothing else happens here on purpose: no sockets, no files, no
        // logging. The loader lock is not the place for any of it, so the
        // client configures itself lazily on the first API call instead.
        DisableThreadLibraryCalls(instance);
    }
    return TRUE;
}

// ---------------------------------------------------------------------------
//  Our exports. They are not part of the Steam API, so a game never imports
//  them; they exist so the harness can answer "which build is loaded, and did it
//  reach the backend?" without guessing from logs.
// ---------------------------------------------------------------------------

STEAMMOCK_EXPORT const char* STEAMMOCK_CALL SteamMock_Version(void) {
    // The same CMake project version the backend prints, so they cannot drift.
    return STEAMMOCK_VERSION;
}

// Empty until the handshake has succeeded. The pointer is valid until the next
// call from this thread: the id belongs to a connection and the next one replaces
// it, so what is handed back is this thread's own copy rather than the client's -
// which another thread's reconnect may be rewriting while a caller reads it.
STEAMMOCK_EXPORT const char* STEAMMOCK_CALL SteamMock_SessionId(void) {
    steammock::Client& client = steammock::Client::instance();
    if (!client.backend_connected()) {
        return "";
    }
    static thread_local std::string last;
    last = client.session_id();
    return last.c_str();
}

// Returns the number of calls the stub has forwarded, and writes the number the
// backend declined to answer through `out_unhandled`.
STEAMMOCK_EXPORT unsigned long STEAMMOCK_CALL SteamMock_Stats(unsigned long* out_unhandled) {
    steammock::Client& client = steammock::Client::instance();
    if (out_unhandled != nullptr) {
        *out_unhandled = client.unhandled_count();
    }
    return client.call_count();
}
