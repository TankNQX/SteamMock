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

STEAMBRIDGE_EXPORT const char* STEAMBRIDGE_CALL SteamBridge_Version(void) {
    // The same CMake project version the backend prints, so they cannot drift.
    return STEAMBRIDGE_VERSION;
}

// Empty until the handshake has succeeded. The pointer stays valid for the life
// of the process (the session string outlives the call), which is fine for a
// diagnostic.
STEAMBRIDGE_EXPORT const char* STEAMBRIDGE_CALL SteamBridge_SessionId(void) {
    steambridge::Client& client = steambridge::Client::instance();
    if (!client.backend_connected()) {
        return "";
    }
    return client.session_id().c_str();
}

// Returns the number of calls the stub has forwarded, and writes the number the
// backend declined to answer through `out_unhandled`.
STEAMBRIDGE_EXPORT unsigned long STEAMBRIDGE_CALL SteamBridge_Stats(unsigned long* out_unhandled) {
    steambridge::Client& client = steambridge::Client::instance();
    if (out_unhandled != nullptr) {
        *out_unhandled = client.unhandled_count();
    }
    return client.call_count();
}
