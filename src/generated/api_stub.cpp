// ============================================================================
//  GENERATED FILE - do not edit by hand.
//
//  Source:   gen/steam_api.idl.json (surface 'seed', revision 2, 41 calls)
//  Regenerate: steambridge_codegen
// ============================================================================

#include "bridge/call.hpp"
#include "bridge/export.hpp"

namespace {

// Handed back for a string-returning call nobody answered, so a game that
// skips its null check still reads empty text instead of faulting.
const char kEmptyString[] = "";

}  // namespace

// SteamAPI_GetHSteamPipe
STEAMBRIDGE_EXPORT std::uint32_t STEAMBRIDGE_CALL SteamAPI_GetHSteamPipe(void) {
    std::uint32_t result = 0;
    try {
        steambridge::Json args = steambridge::Json::object();

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_GetHSteamPipe", args, reply)) {
            result = static_cast<std::uint32_t>(steambridge::reply_uint(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_GetHSteamUser
STEAMBRIDGE_EXPORT std::uint32_t STEAMBRIDGE_CALL SteamAPI_GetHSteamUser(void) {
    std::uint32_t result = 0;
    try {
        steambridge::Json args = steambridge::Json::object();

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_GetHSteamUser", args, reply)) {
            result = static_cast<std::uint32_t>(steambridge::reply_uint(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_GetSteamInstallPath
STEAMBRIDGE_EXPORT const char* STEAMBRIDGE_CALL SteamAPI_GetSteamInstallPath(void) {
    const char* result = kEmptyString;
    try {
        steambridge::Json args = steambridge::Json::object();

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_GetSteamInstallPath", args, reply)) {
            result = steambridge::reply_cstring(reply, kEmptyString);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamApps
STEAMBRIDGE_EXPORT void* STEAMBRIDGE_CALL SteamAPI_ISteamApps(void) {
    void* result = nullptr;
    try {
        steambridge::Json args = steambridge::Json::object();

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamApps", args, reply)) {
            result = steambridge::reply_pointer(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamApps_GetAppBuildId
STEAMBRIDGE_EXPORT std::int32_t STEAMBRIDGE_CALL SteamAPI_ISteamApps_GetAppBuildId(void* self) {
    std::int32_t result = 0;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("self", steambridge::arg_pointer(self));

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamApps_GetAppBuildId", args, reply)) {
            result = static_cast<std::int32_t>(steambridge::reply_int(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamFriends_GetPersonaName
STEAMBRIDGE_EXPORT const char* STEAMBRIDGE_CALL SteamAPI_ISteamFriends_GetPersonaName(void* self) {
    const char* result = kEmptyString;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("self", steambridge::arg_pointer(self));

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamFriends_GetPersonaName", args, reply)) {
            result = steambridge::reply_cstring(reply, kEmptyString);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUser
STEAMBRIDGE_EXPORT void* STEAMBRIDGE_CALL SteamAPI_ISteamUser(void) {
    void* result = nullptr;
    try {
        steambridge::Json args = steambridge::Json::object();

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamUser", args, reply)) {
            result = steambridge::reply_pointer(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats
STEAMBRIDGE_EXPORT void* STEAMBRIDGE_CALL SteamAPI_ISteamUserStats(void) {
    void* result = nullptr;
    try {
        steambridge::Json args = steambridge::Json::object();

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamUserStats", args, reply)) {
            result = steambridge::reply_pointer(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats_GetAchievement
STEAMBRIDGE_EXPORT bool STEAMBRIDGE_CALL SteamAPI_ISteamUserStats_GetAchievement(void* self, const char* pchName, bool* pbAchieved) {
    bool result = false;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("self", steambridge::arg_pointer(self));
        args.set("pchName", steambridge::arg_cstring(pchName));
        args.set("pbAchieved", pbAchieved != nullptr
                                  ? steambridge::arg_bool(static_cast<bool>(*pbAchieved))
                                  : steambridge::Json::null());

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamUserStats_GetAchievement", args, reply)) {
            if (pbAchieved != nullptr) {
                const steambridge::Json* value = steambridge::reply_out(reply, "pbAchieved");
                if (value != nullptr && !value->is_null()) {
                    *pbAchieved = static_cast<bool>(value->as_bool());
                }
            }
            result = steambridge::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats_GetAchievementName
STEAMBRIDGE_EXPORT const char* STEAMBRIDGE_CALL SteamAPI_ISteamUserStats_GetAchievementName(void* self, std::uint32_t iAchievement) {
    const char* result = kEmptyString;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("self", steambridge::arg_pointer(self));
        args.set("iAchievement", steambridge::arg_uint(iAchievement));

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamUserStats_GetAchievementName", args, reply)) {
            result = steambridge::reply_cstring(reply, kEmptyString);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats_GetNumAchievements
STEAMBRIDGE_EXPORT std::uint32_t STEAMBRIDGE_CALL SteamAPI_ISteamUserStats_GetNumAchievements(void* self) {
    std::uint32_t result = 0;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("self", steambridge::arg_pointer(self));

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamUserStats_GetNumAchievements", args, reply)) {
            result = static_cast<std::uint32_t>(steambridge::reply_uint(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats_GetStatInt32
STEAMBRIDGE_EXPORT bool STEAMBRIDGE_CALL SteamAPI_ISteamUserStats_GetStatInt32(void* self, const char* pchName, std::int32_t* pnData) {
    bool result = false;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("self", steambridge::arg_pointer(self));
        args.set("pchName", steambridge::arg_cstring(pchName));
        args.set("pnData", pnData != nullptr
                                  ? steambridge::arg_int(static_cast<std::int32_t>(*pnData))
                                  : steambridge::Json::null());

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamUserStats_GetStatInt32", args, reply)) {
            if (pnData != nullptr) {
                const steambridge::Json* value = steambridge::reply_out(reply, "pnData");
                if (value != nullptr && !value->is_null()) {
                    *pnData = static_cast<std::int32_t>(value->as_int64());
                }
            }
            result = steambridge::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats_RequestCurrentStats
STEAMBRIDGE_EXPORT bool STEAMBRIDGE_CALL SteamAPI_ISteamUserStats_RequestCurrentStats(void* self) {
    bool result = false;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("self", steambridge::arg_pointer(self));

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamUserStats_RequestCurrentStats", args, reply)) {
            result = steambridge::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats_SetAchievement
STEAMBRIDGE_EXPORT bool STEAMBRIDGE_CALL SteamAPI_ISteamUserStats_SetAchievement(void* self, const char* pchName) {
    bool result = false;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("self", steambridge::arg_pointer(self));
        args.set("pchName", steambridge::arg_cstring(pchName));

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamUserStats_SetAchievement", args, reply)) {
            result = steambridge::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats_SetStatInt32
STEAMBRIDGE_EXPORT bool STEAMBRIDGE_CALL SteamAPI_ISteamUserStats_SetStatInt32(void* self, const char* pchName, std::int32_t nData) {
    bool result = false;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("self", steambridge::arg_pointer(self));
        args.set("pchName", steambridge::arg_cstring(pchName));
        args.set("nData", steambridge::arg_int(nData));

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamUserStats_SetStatInt32", args, reply)) {
            result = steambridge::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUserStats_StoreStats
STEAMBRIDGE_EXPORT bool STEAMBRIDGE_CALL SteamAPI_ISteamUserStats_StoreStats(void* self) {
    bool result = false;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("self", steambridge::arg_pointer(self));

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamUserStats_StoreStats", args, reply)) {
            result = steambridge::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUser_GetSteamID
STEAMBRIDGE_EXPORT std::uint64_t STEAMBRIDGE_CALL SteamAPI_ISteamUser_GetSteamID(void* self) {
    std::uint64_t result = 0;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("self", steambridge::arg_pointer(self));

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamUser_GetSteamID", args, reply)) {
            result = steambridge::reply_uint(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUtils
STEAMBRIDGE_EXPORT void* STEAMBRIDGE_CALL SteamAPI_ISteamUtils(void) {
    void* result = nullptr;
    try {
        steambridge::Json args = steambridge::Json::object();

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamUtils", args, reply)) {
            result = steambridge::reply_pointer(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUtils_GetAppID
STEAMBRIDGE_EXPORT std::uint32_t STEAMBRIDGE_CALL SteamAPI_ISteamUtils_GetAppID(void* self) {
    std::uint32_t result = 0;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("self", steambridge::arg_pointer(self));

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamUtils_GetAppID", args, reply)) {
            result = static_cast<std::uint32_t>(steambridge::reply_uint(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUtils_GetCurrentGameLanguage
STEAMBRIDGE_EXPORT const char* STEAMBRIDGE_CALL SteamAPI_ISteamUtils_GetCurrentGameLanguage(void* self) {
    const char* result = kEmptyString;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("self", steambridge::arg_pointer(self));

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamUtils_GetCurrentGameLanguage", args, reply)) {
            result = steambridge::reply_cstring(reply, kEmptyString);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUtils_GetSecondsSinceAppActive
STEAMBRIDGE_EXPORT std::uint32_t STEAMBRIDGE_CALL SteamAPI_ISteamUtils_GetSecondsSinceAppActive(void* self) {
    std::uint32_t result = 0;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("self", steambridge::arg_pointer(self));

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamUtils_GetSecondsSinceAppActive", args, reply)) {
            result = static_cast<std::uint32_t>(steambridge::reply_uint(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUtils_GetServerRealTime
STEAMBRIDGE_EXPORT std::uint32_t STEAMBRIDGE_CALL SteamAPI_ISteamUtils_GetServerRealTime(void* self) {
    std::uint32_t result = 0;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("self", steambridge::arg_pointer(self));

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamUtils_GetServerRealTime", args, reply)) {
            result = static_cast<std::uint32_t>(steambridge::reply_uint(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_ISteamUtils_GetSteamUILanguage
STEAMBRIDGE_EXPORT const char* STEAMBRIDGE_CALL SteamAPI_ISteamUtils_GetSteamUILanguage(void* self) {
    const char* result = kEmptyString;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("self", steambridge::arg_pointer(self));

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_ISteamUtils_GetSteamUILanguage", args, reply)) {
            result = steambridge::reply_cstring(reply, kEmptyString);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_Init
STEAMBRIDGE_EXPORT bool STEAMBRIDGE_CALL SteamAPI_Init(void) {
    bool result = false;
    try {
        steambridge::Json args = steambridge::Json::object();

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_Init", args, reply)) {
            result = steambridge::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_IsSteamRunning
STEAMBRIDGE_EXPORT bool STEAMBRIDGE_CALL SteamAPI_IsSteamRunning(void) {
    bool result = false;
    try {
        steambridge::Json args = steambridge::Json::object();

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_IsSteamRunning", args, reply)) {
            result = steambridge::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_RegisterCallResult
STEAMBRIDGE_EXPORT void STEAMBRIDGE_CALL SteamAPI_RegisterCallResult(void* pCallback, std::uint64_t hAPICall) {
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("pCallback", steambridge::arg_pointer(pCallback));
        args.set("hAPICall", steambridge::arg_uint(hAPICall));

        steambridge::Json reply;
        (void)steambridge::invoke("SteamAPI_RegisterCallResult", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamAPI_RegisterCallback
STEAMBRIDGE_EXPORT void STEAMBRIDGE_CALL SteamAPI_RegisterCallback(void* pCallback, std::int32_t iCallback) {
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("pCallback", steambridge::arg_pointer(pCallback));
        args.set("iCallback", steambridge::arg_int(iCallback));

        steambridge::Json reply;
        (void)steambridge::invoke("SteamAPI_RegisterCallback", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamAPI_RestartAppIfNecessary
STEAMBRIDGE_EXPORT bool STEAMBRIDGE_CALL SteamAPI_RestartAppIfNecessary(std::uint32_t unOwnAppID) {
    bool result = false;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("unOwnAppID", steambridge::arg_uint(unOwnAppID));

        steambridge::Json reply;
        if (steambridge::invoke("SteamAPI_RestartAppIfNecessary", args, reply)) {
            result = steambridge::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamAPI_RunCallbacks
STEAMBRIDGE_EXPORT void STEAMBRIDGE_CALL SteamAPI_RunCallbacks(void) {
    try {
        steambridge::Json args = steambridge::Json::object();

        steambridge::Json reply;
        (void)steambridge::invoke("SteamAPI_RunCallbacks", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamAPI_SetMiniDumpComment
STEAMBRIDGE_EXPORT void STEAMBRIDGE_CALL SteamAPI_SetMiniDumpComment(const char* pchMsg) {
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("pchMsg", steambridge::arg_cstring(pchMsg));

        steambridge::Json reply;
        (void)steambridge::invoke("SteamAPI_SetMiniDumpComment", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamAPI_Shutdown
STEAMBRIDGE_EXPORT void STEAMBRIDGE_CALL SteamAPI_Shutdown(void) {
    try {
        steambridge::Json args = steambridge::Json::object();

        steambridge::Json reply;
        (void)steambridge::invoke("SteamAPI_Shutdown", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamAPI_UnregisterCallResult
STEAMBRIDGE_EXPORT void STEAMBRIDGE_CALL SteamAPI_UnregisterCallResult(void* pCallback, std::uint64_t hAPICall) {
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("pCallback", steambridge::arg_pointer(pCallback));
        args.set("hAPICall", steambridge::arg_uint(hAPICall));

        steambridge::Json reply;
        (void)steambridge::invoke("SteamAPI_UnregisterCallResult", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamAPI_UnregisterCallback
STEAMBRIDGE_EXPORT void STEAMBRIDGE_CALL SteamAPI_UnregisterCallback(void* pCallback) {
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("pCallback", steambridge::arg_pointer(pCallback));

        steambridge::Json reply;
        (void)steambridge::invoke("SteamAPI_UnregisterCallback", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamAPI_WriteMiniDump
STEAMBRIDGE_EXPORT void STEAMBRIDGE_CALL SteamAPI_WriteMiniDump(std::uint32_t uStructuredExceptionCode, void* pvExceptionInfo, std::uint32_t uBuildID) {
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("uStructuredExceptionCode", steambridge::arg_uint(uStructuredExceptionCode));
        args.set("pvExceptionInfo", steambridge::arg_pointer(pvExceptionInfo));
        args.set("uBuildID", steambridge::arg_uint(uBuildID));

        steambridge::Json reply;
        (void)steambridge::invoke("SteamAPI_WriteMiniDump", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamGameServer_GetHSteamPipe
STEAMBRIDGE_EXPORT std::uint32_t STEAMBRIDGE_CALL SteamGameServer_GetHSteamPipe(void) {
    std::uint32_t result = 0;
    try {
        steambridge::Json args = steambridge::Json::object();

        steambridge::Json reply;
        if (steambridge::invoke("SteamGameServer_GetHSteamPipe", args, reply)) {
            result = static_cast<std::uint32_t>(steambridge::reply_uint(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamGameServer_GetHSteamUser
STEAMBRIDGE_EXPORT std::uint32_t STEAMBRIDGE_CALL SteamGameServer_GetHSteamUser(void) {
    std::uint32_t result = 0;
    try {
        steambridge::Json args = steambridge::Json::object();

        steambridge::Json reply;
        if (steambridge::invoke("SteamGameServer_GetHSteamUser", args, reply)) {
            result = static_cast<std::uint32_t>(steambridge::reply_uint(reply));
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamGameServer_RunCallbacks
STEAMBRIDGE_EXPORT void STEAMBRIDGE_CALL SteamGameServer_RunCallbacks(void) {
    try {
        steambridge::Json args = steambridge::Json::object();

        steambridge::Json reply;
        (void)steambridge::invoke("SteamGameServer_RunCallbacks", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamGameServer_Shutdown
STEAMBRIDGE_EXPORT void STEAMBRIDGE_CALL SteamGameServer_Shutdown(void) {
    try {
        steambridge::Json args = steambridge::Json::object();

        steambridge::Json reply;
        (void)steambridge::invoke("SteamGameServer_Shutdown", args, reply);
    } catch (...) {
        // Never let an exception cross into the game.
    }
}

// SteamInternal_ContextInit
STEAMBRIDGE_EXPORT void* STEAMBRIDGE_CALL SteamInternal_ContextInit(void* pContextInitData) {
    void* result = nullptr;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("pContextInitData", steambridge::arg_pointer(pContextInitData));

        steambridge::Json reply;
        if (steambridge::invoke("SteamInternal_ContextInit", args, reply)) {
            result = steambridge::reply_pointer(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamInternal_CreateInterface
STEAMBRIDGE_EXPORT void* STEAMBRIDGE_CALL SteamInternal_CreateInterface(const char* ver) {
    void* result = nullptr;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("ver", steambridge::arg_cstring(ver));

        steambridge::Json reply;
        if (steambridge::invoke("SteamInternal_CreateInterface", args, reply)) {
            result = steambridge::reply_pointer(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}

// SteamInternal_GameServer_Init
STEAMBRIDGE_EXPORT bool STEAMBRIDGE_CALL SteamInternal_GameServer_Init(std::uint32_t unIP, std::uint16_t usSteamPort, std::uint16_t usGamePort, std::uint16_t usQueryPort, std::int32_t eServerMode, const char* pchVersionString) {
    bool result = false;
    try {
        steambridge::Json args = steambridge::Json::object();
        args.set("unIP", steambridge::arg_uint(unIP));
        args.set("usSteamPort", steambridge::arg_uint(usSteamPort));
        args.set("usGamePort", steambridge::arg_uint(usGamePort));
        args.set("usQueryPort", steambridge::arg_uint(usQueryPort));
        args.set("eServerMode", steambridge::arg_int(eServerMode));
        args.set("pchVersionString", steambridge::arg_cstring(pchVersionString));

        steambridge::Json reply;
        if (steambridge::invoke("SteamInternal_GameServer_Init", args, reply)) {
            result = steambridge::reply_bool(reply);
        }
    } catch (...) {
        // Never let an exception cross into the game.
    }
    return result;
}
