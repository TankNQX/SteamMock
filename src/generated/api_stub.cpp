// ============================================================================
//  GENERATED FILE - do not edit by hand.
//
//  Source:   gen/steam_api.idl.json (surface 'seed', revision 2, 43 calls)
//  Regenerate: steammock_codegen
// ============================================================================

#include "bridge/call.hpp"
#include "bridge/export.hpp"
#include "bridge/synth.hpp"

namespace {

// Handed back for a string-returning call nobody answered, so a game that
// skips its null check still reads empty text instead of faulting.
const char kEmptyString[] = "";

}  // namespace

// SteamAPI_GetHSteamPipe
STEAMMOCK_EXPORT std::uint32_t STEAMMOCK_CALL SteamAPI_GetHSteamPipe(void) {
    std::uint32_t result = 0;
    try {
        steammock::Json args = steammock::Json::object();

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_GetHSteamPipe", args, reply)) {
            result = static_cast<std::uint32_t>(steammock::reply_uint(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_GetHSteamUser
STEAMMOCK_EXPORT std::uint32_t STEAMMOCK_CALL SteamAPI_GetHSteamUser(void) {
    std::uint32_t result = 0;
    try {
        steammock::Json args = steammock::Json::object();

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_GetHSteamUser", args, reply)) {
            result = static_cast<std::uint32_t>(steammock::reply_uint(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_GetSteamInstallPath
STEAMMOCK_EXPORT const char* STEAMMOCK_CALL SteamAPI_GetSteamInstallPath(void) {
    const char* result = kEmptyString;
    try {
        steammock::Json args = steammock::Json::object();

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_GetSteamInstallPath", args, reply)) {
            result = steammock::reply_cstring(reply, kEmptyString);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamApps
STEAMMOCK_EXPORT void* STEAMMOCK_CALL SteamAPI_ISteamApps(void) {
    void* result = nullptr;
    try {
        steammock::Json args = steammock::Json::object();

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamApps", args, reply)) {
            result = steammock::reply_pointer(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamApps_GetAppBuildId
STEAMMOCK_EXPORT std::int32_t STEAMMOCK_CALL SteamAPI_ISteamApps_GetAppBuildId(void* self) {
    std::int32_t result = 0;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("self", steammock::arg_pointer(self));

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamApps_GetAppBuildId", args, reply)) {
            result = static_cast<std::int32_t>(steammock::reply_int(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamFriends_GetPersonaName
STEAMMOCK_EXPORT const char* STEAMMOCK_CALL SteamAPI_ISteamFriends_GetPersonaName(void* self) {
    const char* result = kEmptyString;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("self", steammock::arg_pointer(self));

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamFriends_GetPersonaName", args, reply)) {
            result = steammock::reply_cstring(reply, kEmptyString);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUser
STEAMMOCK_EXPORT void* STEAMMOCK_CALL SteamAPI_ISteamUser(void) {
    void* result = nullptr;
    try {
        steammock::Json args = steammock::Json::object();

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamUser", args, reply)) {
            result = steammock::reply_pointer(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats
STEAMMOCK_EXPORT void* STEAMMOCK_CALL SteamAPI_ISteamUserStats(void) {
    void* result = nullptr;
    try {
        steammock::Json args = steammock::Json::object();

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamUserStats", args, reply)) {
            result = steammock::reply_pointer(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats_GetAchievement
STEAMMOCK_EXPORT bool STEAMMOCK_CALL SteamAPI_ISteamUserStats_GetAchievement(void* self, const char* pchName, bool* pbAchieved) {
    bool result = false;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("self", steammock::arg_pointer(self));
        args.set("pchName", steammock::arg_cstring(pchName));
        args.set("pbAchieved", pbAchieved != nullptr
                                  ? steammock::arg_bool(static_cast<bool>(*pbAchieved))
                                  : steammock::Json::null());

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamUserStats_GetAchievement", args, reply)) {
            if (pbAchieved != nullptr) {
                const steammock::Json* value = steammock::reply_out(reply, "pbAchieved");
                if (value != nullptr && !value->is_null()) {
                    *pbAchieved = static_cast<bool>(value->as_bool());
                }
            }
            result = steammock::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats_GetAchievementName
STEAMMOCK_EXPORT const char* STEAMMOCK_CALL SteamAPI_ISteamUserStats_GetAchievementName(void* self, std::uint32_t iAchievement) {
    const char* result = kEmptyString;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("self", steammock::arg_pointer(self));
        args.set("iAchievement", steammock::arg_uint(iAchievement));

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamUserStats_GetAchievementName", args, reply)) {
            result = steammock::reply_cstring(reply, kEmptyString);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats_GetNumAchievements
STEAMMOCK_EXPORT std::uint32_t STEAMMOCK_CALL SteamAPI_ISteamUserStats_GetNumAchievements(void* self) {
    std::uint32_t result = 0;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("self", steammock::arg_pointer(self));

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamUserStats_GetNumAchievements", args, reply)) {
            result = static_cast<std::uint32_t>(steammock::reply_uint(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats_GetStatInt32
STEAMMOCK_EXPORT bool STEAMMOCK_CALL SteamAPI_ISteamUserStats_GetStatInt32(void* self, const char* pchName, std::int32_t* pData) {
    bool result = false;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("self", steammock::arg_pointer(self));
        args.set("pchName", steammock::arg_cstring(pchName));
        args.set("pData", pData != nullptr
                                  ? steammock::arg_int(static_cast<std::int32_t>(*pData))
                                  : steammock::Json::null());

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamUserStats_GetStatInt32", args, reply)) {
            if (pData != nullptr) {
                const steammock::Json* value = steammock::reply_out(reply, "pData");
                if (value != nullptr && !value->is_null()) {
                    *pData = static_cast<std::int32_t>(value->as_int64());
                }
            }
            result = steammock::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats_RequestCurrentStats
STEAMMOCK_EXPORT bool STEAMMOCK_CALL SteamAPI_ISteamUserStats_RequestCurrentStats(void* self) {
    bool result = false;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("self", steammock::arg_pointer(self));

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamUserStats_RequestCurrentStats", args, reply)) {
            result = steammock::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats_SetAchievement
STEAMMOCK_EXPORT bool STEAMMOCK_CALL SteamAPI_ISteamUserStats_SetAchievement(void* self, const char* pchName) {
    bool result = false;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("self", steammock::arg_pointer(self));
        args.set("pchName", steammock::arg_cstring(pchName));

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamUserStats_SetAchievement", args, reply)) {
            result = steammock::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats_SetStatInt32
STEAMMOCK_EXPORT bool STEAMMOCK_CALL SteamAPI_ISteamUserStats_SetStatInt32(void* self, const char* pchName, std::int32_t nData) {
    bool result = false;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("self", steammock::arg_pointer(self));
        args.set("pchName", steammock::arg_cstring(pchName));
        args.set("nData", steammock::arg_int(nData));

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamUserStats_SetStatInt32", args, reply)) {
            result = steammock::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats_StoreStats
STEAMMOCK_EXPORT bool STEAMMOCK_CALL SteamAPI_ISteamUserStats_StoreStats(void* self) {
    bool result = false;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("self", steammock::arg_pointer(self));

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamUserStats_StoreStats", args, reply)) {
            result = steammock::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUser_GetSteamID
STEAMMOCK_EXPORT std::uint64_t STEAMMOCK_CALL SteamAPI_ISteamUser_GetSteamID(void* self) {
    std::uint64_t result = 0;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("self", steammock::arg_pointer(self));

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamUser_GetSteamID", args, reply)) {
            result = steammock::reply_uint(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUtils
STEAMMOCK_EXPORT void* STEAMMOCK_CALL SteamAPI_ISteamUtils(void) {
    void* result = nullptr;
    try {
        steammock::Json args = steammock::Json::object();

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamUtils", args, reply)) {
            result = steammock::reply_pointer(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUtils_GetAppID
STEAMMOCK_EXPORT std::uint32_t STEAMMOCK_CALL SteamAPI_ISteamUtils_GetAppID(void* self) {
    std::uint32_t result = 0;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("self", steammock::arg_pointer(self));

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamUtils_GetAppID", args, reply)) {
            result = static_cast<std::uint32_t>(steammock::reply_uint(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUtils_GetCurrentGameLanguage
STEAMMOCK_EXPORT const char* STEAMMOCK_CALL SteamAPI_ISteamUtils_GetCurrentGameLanguage(void* self) {
    const char* result = kEmptyString;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("self", steammock::arg_pointer(self));

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamUtils_GetCurrentGameLanguage", args, reply)) {
            result = steammock::reply_cstring(reply, kEmptyString);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUtils_GetSecondsSinceAppActive
STEAMMOCK_EXPORT std::uint32_t STEAMMOCK_CALL SteamAPI_ISteamUtils_GetSecondsSinceAppActive(void* self) {
    std::uint32_t result = 0;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("self", steammock::arg_pointer(self));

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamUtils_GetSecondsSinceAppActive", args, reply)) {
            result = static_cast<std::uint32_t>(steammock::reply_uint(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUtils_GetServerRealTime
STEAMMOCK_EXPORT std::uint32_t STEAMMOCK_CALL SteamAPI_ISteamUtils_GetServerRealTime(void* self) {
    std::uint32_t result = 0;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("self", steammock::arg_pointer(self));

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamUtils_GetServerRealTime", args, reply)) {
            result = static_cast<std::uint32_t>(steammock::reply_uint(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUtils_GetSteamUILanguage
STEAMMOCK_EXPORT const char* STEAMMOCK_CALL SteamAPI_ISteamUtils_GetSteamUILanguage(void* self) {
    const char* result = kEmptyString;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("self", steammock::arg_pointer(self));

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_ISteamUtils_GetSteamUILanguage", args, reply)) {
            result = steammock::reply_cstring(reply, kEmptyString);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_Init
STEAMMOCK_EXPORT bool STEAMMOCK_CALL SteamAPI_Init(void) {
    bool result = false;
    try {
        steammock::Json args = steammock::Json::object();

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_Init", args, reply)) {
            result = steammock::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_IsSteamRunning
STEAMMOCK_EXPORT bool STEAMMOCK_CALL SteamAPI_IsSteamRunning(void) {
    bool result = false;
    try {
        steammock::Json args = steammock::Json::object();

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_IsSteamRunning", args, reply)) {
            result = steammock::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_RegisterCallResult
STEAMMOCK_EXPORT void STEAMMOCK_CALL SteamAPI_RegisterCallResult(void* pCallback, std::uint64_t hAPICall) {
    try {
        steammock::Json args = steammock::Json::object();
        args.set("pCallback", steammock::arg_pointer(pCallback));
        args.set("hAPICall", steammock::arg_uint(hAPICall));

        steammock::Json reply;
        (void)steammock::invoke("SteamAPI_RegisterCallResult", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamAPI_RegisterCallback
STEAMMOCK_EXPORT void STEAMMOCK_CALL SteamAPI_RegisterCallback(void* pCallback, std::int32_t iCallback) {
    try {
        steammock::Json args = steammock::Json::object();
        args.set("pCallback", steammock::arg_pointer(pCallback));
        args.set("iCallback", steammock::arg_int(iCallback));

        steammock::Json reply;
        (void)steammock::invoke("SteamAPI_RegisterCallback", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamAPI_RestartAppIfNecessary
STEAMMOCK_EXPORT bool STEAMMOCK_CALL SteamAPI_RestartAppIfNecessary(std::uint32_t unOwnAppID) {
    bool result = false;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("unOwnAppID", steammock::arg_uint(unOwnAppID));

        steammock::Json reply;
        if (steammock::invoke("SteamAPI_RestartAppIfNecessary", args, reply)) {
            result = steammock::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_RunCallbacks
STEAMMOCK_EXPORT void STEAMMOCK_CALL SteamAPI_RunCallbacks(void) {
    try {
        steammock::Json args = steammock::Json::object();

        steammock::Json reply;
        (void)steammock::invoke("SteamAPI_RunCallbacks", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamAPI_SetMiniDumpComment
STEAMMOCK_EXPORT void STEAMMOCK_CALL SteamAPI_SetMiniDumpComment(const char* pchMsg) {
    try {
        steammock::Json args = steammock::Json::object();
        args.set("pchMsg", steammock::arg_cstring(pchMsg));

        steammock::Json reply;
        (void)steammock::invoke("SteamAPI_SetMiniDumpComment", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamAPI_Shutdown
STEAMMOCK_EXPORT void STEAMMOCK_CALL SteamAPI_Shutdown(void) {
    try {
        steammock::Json args = steammock::Json::object();

        steammock::Json reply;
        (void)steammock::invoke("SteamAPI_Shutdown", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamAPI_UnregisterCallResult
STEAMMOCK_EXPORT void STEAMMOCK_CALL SteamAPI_UnregisterCallResult(void* pCallback, std::uint64_t hAPICall) {
    try {
        steammock::Json args = steammock::Json::object();
        args.set("pCallback", steammock::arg_pointer(pCallback));
        args.set("hAPICall", steammock::arg_uint(hAPICall));

        steammock::Json reply;
        (void)steammock::invoke("SteamAPI_UnregisterCallResult", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamAPI_UnregisterCallback
STEAMMOCK_EXPORT void STEAMMOCK_CALL SteamAPI_UnregisterCallback(void* pCallback) {
    try {
        steammock::Json args = steammock::Json::object();
        args.set("pCallback", steammock::arg_pointer(pCallback));

        steammock::Json reply;
        (void)steammock::invoke("SteamAPI_UnregisterCallback", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamAPI_WriteMiniDump
STEAMMOCK_EXPORT void STEAMMOCK_CALL SteamAPI_WriteMiniDump(std::uint32_t uStructuredExceptionCode, void* pvExceptionInfo, std::uint32_t uBuildID) {
    try {
        steammock::Json args = steammock::Json::object();
        args.set("uStructuredExceptionCode", steammock::arg_uint(uStructuredExceptionCode));
        args.set("pvExceptionInfo", steammock::arg_pointer(pvExceptionInfo));
        args.set("uBuildID", steammock::arg_uint(uBuildID));

        steammock::Json reply;
        (void)steammock::invoke("SteamAPI_WriteMiniDump", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamGameServer_GetHSteamPipe
STEAMMOCK_EXPORT std::uint32_t STEAMMOCK_CALL SteamGameServer_GetHSteamPipe(void) {
    std::uint32_t result = 0;
    try {
        steammock::Json args = steammock::Json::object();

        steammock::Json reply;
        if (steammock::invoke("SteamGameServer_GetHSteamPipe", args, reply)) {
            result = static_cast<std::uint32_t>(steammock::reply_uint(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamGameServer_GetHSteamUser
STEAMMOCK_EXPORT std::uint32_t STEAMMOCK_CALL SteamGameServer_GetHSteamUser(void) {
    std::uint32_t result = 0;
    try {
        steammock::Json args = steammock::Json::object();

        steammock::Json reply;
        if (steammock::invoke("SteamGameServer_GetHSteamUser", args, reply)) {
            result = static_cast<std::uint32_t>(steammock::reply_uint(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamGameServer_RunCallbacks
STEAMMOCK_EXPORT void STEAMMOCK_CALL SteamGameServer_RunCallbacks(void) {
    try {
        steammock::Json args = steammock::Json::object();

        steammock::Json reply;
        (void)steammock::invoke("SteamGameServer_RunCallbacks", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamGameServer_Shutdown
STEAMMOCK_EXPORT void STEAMMOCK_CALL SteamGameServer_Shutdown(void) {
    try {
        steammock::Json args = steammock::Json::object();

        steammock::Json reply;
        (void)steammock::invoke("SteamGameServer_Shutdown", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamInternal_ContextInit
STEAMMOCK_EXPORT void* STEAMMOCK_CALL SteamInternal_ContextInit(void* pContextInitData) {
    void* result = nullptr;
    try {
        result = steammock::context_init(pContextInitData, "SteamInternal_ContextInit");
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamInternal_CreateInterface
STEAMMOCK_EXPORT void* STEAMMOCK_CALL SteamInternal_CreateInterface(const char* ver) {
    void* result = nullptr;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("ver", steammock::arg_cstring(ver));

        steammock::Json reply;
        if (steammock::invoke("SteamInternal_CreateInterface", args, reply)) {
            result = steammock::reply_pointer(reply);
        }
        if (result == nullptr) {
            result = steammock::interface_object(ver);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamInternal_FindOrCreateGameServerInterface
STEAMMOCK_EXPORT void* STEAMMOCK_CALL SteamInternal_FindOrCreateGameServerInterface(std::int32_t hSteamUser, const char* pszVersion) {
    void* result = nullptr;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("hSteamUser", steammock::arg_int(hSteamUser));
        args.set("pszVersion", steammock::arg_cstring(pszVersion));

        steammock::Json reply;
        if (steammock::invoke("SteamInternal_FindOrCreateGameServerInterface", args, reply)) {
            result = steammock::reply_pointer(reply);
        }
        if (result == nullptr) {
            result = steammock::interface_object(pszVersion);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamInternal_FindOrCreateUserInterface
STEAMMOCK_EXPORT void* STEAMMOCK_CALL SteamInternal_FindOrCreateUserInterface(std::int32_t hSteamUser, const char* pszVersion) {
    void* result = nullptr;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("hSteamUser", steammock::arg_int(hSteamUser));
        args.set("pszVersion", steammock::arg_cstring(pszVersion));

        steammock::Json reply;
        if (steammock::invoke("SteamInternal_FindOrCreateUserInterface", args, reply)) {
            result = steammock::reply_pointer(reply);
        }
        if (result == nullptr) {
            result = steammock::interface_object(pszVersion);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamInternal_GameServer_Init
STEAMMOCK_EXPORT bool STEAMMOCK_CALL SteamInternal_GameServer_Init(std::uint32_t unIP, std::uint16_t usSteamPort, std::uint16_t usGamePort, std::uint16_t usQueryPort, std::int32_t eServerMode, const char* pchVersionString) {
    bool result = false;
    try {
        steammock::Json args = steammock::Json::object();
        args.set("unIP", steammock::arg_uint(unIP));
        args.set("usSteamPort", steammock::arg_uint(usSteamPort));
        args.set("usGamePort", steammock::arg_uint(usGamePort));
        args.set("usQueryPort", steammock::arg_uint(usQueryPort));
        args.set("eServerMode", steammock::arg_int(eServerMode));
        args.set("pchVersionString", steammock::arg_cstring(pchVersionString));

        steammock::Json reply;
        if (steammock::invoke("SteamInternal_GameServer_Init", args, reply)) {
            result = steammock::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}
