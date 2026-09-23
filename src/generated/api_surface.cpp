// ============================================================================
//  GENERATED FILE - do not edit by hand.
//
//  Source:   gen/steam_api.idl.json (surface 'seed', revision 2, 43 calls)
//  Regenerate: steammock_codegen
// ============================================================================

#include "bridge/surface.hpp"

namespace steammock {
namespace {

const SurfaceParam kParams_SteamAPI_ISteamApps_GetAppBuildId[] = {
    {"self", "opaque_ptr", false},
};

const SurfaceParam kParams_SteamAPI_ISteamFriends_GetPersonaName[] = {
    {"self", "opaque_ptr", false},
};

const SurfaceParam kParams_SteamAPI_ISteamUserStats_GetAchievement[] = {
    {"self", "opaque_ptr", false},
    {"pchName", "cstring", false},
    {"pbAchieved", "bool", true},
};

const SurfaceParam kParams_SteamAPI_ISteamUserStats_GetAchievementName[] = {
    {"self", "opaque_ptr", false},
    {"iAchievement", "uint32", false},
};

const SurfaceParam kParams_SteamAPI_ISteamUserStats_GetNumAchievements[] = {
    {"self", "opaque_ptr", false},
};

const SurfaceParam kParams_SteamAPI_ISteamUserStats_GetStatInt32[] = {
    {"self", "opaque_ptr", false},
    {"pchName", "cstring", false},
    {"pData", "int32", true},
};

const SurfaceParam kParams_SteamAPI_ISteamUserStats_RequestCurrentStats[] = {
    {"self", "opaque_ptr", false},
};

const SurfaceParam kParams_SteamAPI_ISteamUserStats_SetAchievement[] = {
    {"self", "opaque_ptr", false},
    {"pchName", "cstring", false},
};

const SurfaceParam kParams_SteamAPI_ISteamUserStats_SetStatInt32[] = {
    {"self", "opaque_ptr", false},
    {"pchName", "cstring", false},
    {"nData", "int32", false},
};

const SurfaceParam kParams_SteamAPI_ISteamUserStats_StoreStats[] = {
    {"self", "opaque_ptr", false},
};

const SurfaceParam kParams_SteamAPI_ISteamUser_GetSteamID[] = {
    {"self", "opaque_ptr", false},
};

const SurfaceParam kParams_SteamAPI_ISteamUtils_GetAppID[] = {
    {"self", "opaque_ptr", false},
};

const SurfaceParam kParams_SteamAPI_ISteamUtils_GetCurrentGameLanguage[] = {
    {"self", "opaque_ptr", false},
};

const SurfaceParam kParams_SteamAPI_ISteamUtils_GetSecondsSinceAppActive[] = {
    {"self", "opaque_ptr", false},
};

const SurfaceParam kParams_SteamAPI_ISteamUtils_GetServerRealTime[] = {
    {"self", "opaque_ptr", false},
};

const SurfaceParam kParams_SteamAPI_ISteamUtils_GetSteamUILanguage[] = {
    {"self", "opaque_ptr", false},
};

const SurfaceParam kParams_SteamAPI_RegisterCallResult[] = {
    {"pCallback", "opaque_ptr", false},
    {"hAPICall", "uint64", false},
};

const SurfaceParam kParams_SteamAPI_RegisterCallback[] = {
    {"pCallback", "opaque_ptr", false},
    {"iCallback", "int32", false},
};

const SurfaceParam kParams_SteamAPI_RestartAppIfNecessary[] = {
    {"unOwnAppID", "uint32", false},
};

const SurfaceParam kParams_SteamAPI_SetMiniDumpComment[] = {
    {"pchMsg", "cstring", false},
};

const SurfaceParam kParams_SteamAPI_UnregisterCallResult[] = {
    {"pCallback", "opaque_ptr", false},
    {"hAPICall", "uint64", false},
};

const SurfaceParam kParams_SteamAPI_UnregisterCallback[] = {
    {"pCallback", "opaque_ptr", false},
};

const SurfaceParam kParams_SteamAPI_WriteMiniDump[] = {
    {"uStructuredExceptionCode", "uint32", false},
    {"pvExceptionInfo", "opaque_ptr", false},
    {"uBuildID", "uint32", false},
};

const SurfaceParam kParams_SteamInternal_ContextInit[] = {
    {"pContextInitData", "opaque_ptr", false},
};

const SurfaceParam kParams_SteamInternal_CreateInterface[] = {
    {"ver", "cstring", false},
};

const SurfaceParam kParams_SteamInternal_FindOrCreateGameServerInterface[] = {
    {"hSteamUser", "int32", false},
    {"pszVersion", "cstring", false},
};

const SurfaceParam kParams_SteamInternal_FindOrCreateUserInterface[] = {
    {"hSteamUser", "int32", false},
    {"pszVersion", "cstring", false},
};

const SurfaceParam kParams_SteamInternal_GameServer_Init[] = {
    {"unIP", "uint32", false},
    {"usSteamPort", "uint16", false},
    {"usGamePort", "uint16", false},
    {"usQueryPort", "uint16", false},
    {"eServerMode", "int32", false},
    {"pchVersionString", "cstring", false},
};

const SurfaceCall kCalls[] = {
    {"SteamAPI_GetHSteamPipe", "uint32", nullptr, 0},
    {"SteamAPI_GetHSteamUser", "uint32", nullptr, 0},
    {"SteamAPI_GetSteamInstallPath", "cstring", nullptr, 0},
    {"SteamAPI_ISteamApps", "opaque_ptr", nullptr, 0},
    {"SteamAPI_ISteamApps_GetAppBuildId", "int32", kParams_SteamAPI_ISteamApps_GetAppBuildId, 1},
    {"SteamAPI_ISteamFriends_GetPersonaName", "cstring", kParams_SteamAPI_ISteamFriends_GetPersonaName, 1},
    {"SteamAPI_ISteamUser", "opaque_ptr", nullptr, 0},
    {"SteamAPI_ISteamUserStats", "opaque_ptr", nullptr, 0},
    {"SteamAPI_ISteamUserStats_GetAchievement", "bool", kParams_SteamAPI_ISteamUserStats_GetAchievement, 3},
    {"SteamAPI_ISteamUserStats_GetAchievementName", "cstring", kParams_SteamAPI_ISteamUserStats_GetAchievementName, 2},
    {"SteamAPI_ISteamUserStats_GetNumAchievements", "uint32", kParams_SteamAPI_ISteamUserStats_GetNumAchievements, 1},
    {"SteamAPI_ISteamUserStats_GetStatInt32", "bool", kParams_SteamAPI_ISteamUserStats_GetStatInt32, 3},
    {"SteamAPI_ISteamUserStats_RequestCurrentStats", "bool", kParams_SteamAPI_ISteamUserStats_RequestCurrentStats, 1},
    {"SteamAPI_ISteamUserStats_SetAchievement", "bool", kParams_SteamAPI_ISteamUserStats_SetAchievement, 2},
    {"SteamAPI_ISteamUserStats_SetStatInt32", "bool", kParams_SteamAPI_ISteamUserStats_SetStatInt32, 3},
    {"SteamAPI_ISteamUserStats_StoreStats", "bool", kParams_SteamAPI_ISteamUserStats_StoreStats, 1},
    {"SteamAPI_ISteamUser_GetSteamID", "uint64", kParams_SteamAPI_ISteamUser_GetSteamID, 1},
    {"SteamAPI_ISteamUtils", "opaque_ptr", nullptr, 0},
    {"SteamAPI_ISteamUtils_GetAppID", "uint32", kParams_SteamAPI_ISteamUtils_GetAppID, 1},
    {"SteamAPI_ISteamUtils_GetCurrentGameLanguage", "cstring", kParams_SteamAPI_ISteamUtils_GetCurrentGameLanguage, 1},
    {"SteamAPI_ISteamUtils_GetSecondsSinceAppActive", "uint32", kParams_SteamAPI_ISteamUtils_GetSecondsSinceAppActive, 1},
    {"SteamAPI_ISteamUtils_GetServerRealTime", "uint32", kParams_SteamAPI_ISteamUtils_GetServerRealTime, 1},
    {"SteamAPI_ISteamUtils_GetSteamUILanguage", "cstring", kParams_SteamAPI_ISteamUtils_GetSteamUILanguage, 1},
    {"SteamAPI_Init", "bool", nullptr, 0},
    {"SteamAPI_IsSteamRunning", "bool", nullptr, 0},
    {"SteamAPI_RegisterCallResult", "void", kParams_SteamAPI_RegisterCallResult, 2},
    {"SteamAPI_RegisterCallback", "void", kParams_SteamAPI_RegisterCallback, 2},
    {"SteamAPI_RestartAppIfNecessary", "bool", kParams_SteamAPI_RestartAppIfNecessary, 1},
    {"SteamAPI_RunCallbacks", "void", nullptr, 0},
    {"SteamAPI_SetMiniDumpComment", "void", kParams_SteamAPI_SetMiniDumpComment, 1},
    {"SteamAPI_Shutdown", "void", nullptr, 0},
    {"SteamAPI_UnregisterCallResult", "void", kParams_SteamAPI_UnregisterCallResult, 2},
    {"SteamAPI_UnregisterCallback", "void", kParams_SteamAPI_UnregisterCallback, 1},
    {"SteamAPI_WriteMiniDump", "void", kParams_SteamAPI_WriteMiniDump, 3},
    {"SteamGameServer_GetHSteamPipe", "uint32", nullptr, 0},
    {"SteamGameServer_GetHSteamUser", "uint32", nullptr, 0},
    {"SteamGameServer_RunCallbacks", "void", nullptr, 0},
    {"SteamGameServer_Shutdown", "void", nullptr, 0},
    {"SteamInternal_ContextInit", "opaque_ptr", kParams_SteamInternal_ContextInit, 1},
    {"SteamInternal_CreateInterface", "opaque_ptr", kParams_SteamInternal_CreateInterface, 1},
    {"SteamInternal_FindOrCreateGameServerInterface", "opaque_ptr", kParams_SteamInternal_FindOrCreateGameServerInterface, 2},
    {"SteamInternal_FindOrCreateUserInterface", "opaque_ptr", kParams_SteamInternal_FindOrCreateUserInterface, 2},
    {"SteamInternal_GameServer_Init", "bool", kParams_SteamInternal_GameServer_Init, 6},
};

}  // namespace

const char* api_surface_name() noexcept { return "seed"; }
int api_surface_revision() noexcept { return 2; }
const SurfaceCall* api_surface_calls(std::size_t& count) noexcept {
    count = 43;
    return kCalls;
}

}  // namespace steammock
