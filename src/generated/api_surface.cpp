// ============================================================================
//  GENERATED FILE - do not edit by hand.
//
//  Source:   gen/steam_api.idl.json (surface 'seed', revision 1, 28 calls)
//  Regenerate: steambridge_codegen
// ============================================================================

#include "bridge/surface.hpp"

namespace steambridge {
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
    {"pnData", "int32", true},
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

const SurfaceParam kParams_SteamAPI_RestartAppIfNecessary[] = {
    {"unOwnAppID", "uint32", false},
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
    {"SteamAPI_RestartAppIfNecessary", "bool", kParams_SteamAPI_RestartAppIfNecessary, 1},
    {"SteamAPI_RunCallbacks", "void", nullptr, 0},
    {"SteamAPI_Shutdown", "void", nullptr, 0},
};

}  // namespace

const char* api_surface_name() noexcept { return "seed"; }
int api_surface_revision() noexcept { return 1; }
const SurfaceCall* api_surface_calls(std::size_t& count) noexcept {
    count = 28;
    return kCalls;
}

}  // namespace steambridge
