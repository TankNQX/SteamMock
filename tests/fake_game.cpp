// ============================================================================
//  tests/fake_game.cpp - a stand-in for a game.
// ----------------------------------------------------------------------------
//  It loads the stub the way a game's import table would (by name, from the path
//  in STEAMBRIDGE_STUB), calls a slice of the flat API through GetProcAddress,
//  and prints every result as one `key=value` line so the end-to-end test can
//  check them without parsing prose.
//
//  This is also the completeness check for the generated .def: a name that is not
//  exported makes this program exit 3 and say which one.
// ============================================================================

#include <windows.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace {

using init_fn = bool (*)();
using void_fn = void (*)();
using bool_fn = bool (*)();
using interface_fn = void* (*)();
using bool_self_fn = bool (*)(void*);
using steam_id_fn = std::uint64_t (*)(void*);
using cstring_self_fn = const char* (*)(void*);
using app_id_fn = std::uint32_t (*)(void*);
using stat_get_fn = bool (*)(void*, const char*, std::int32_t*);
using stat_set_fn = bool (*)(void*, const char*, std::int32_t);
using achievement_get_fn = bool (*)(void*, const char*, bool*);
using achievement_set_fn = bool (*)(void*, const char*);
using count_fn = std::uint32_t (*)(void*);
using achievement_name_fn = const char* (*)(void*, std::uint32_t);
using session_fn = const char* (*)();
using stats_fn = unsigned long (*)(unsigned long*);

template <typename Fn>
Fn resolve(HMODULE module, const char* name) {
    const FARPROC address = GetProcAddress(module, name);
    if (address == nullptr) {
        std::printf("missing export: %s\n", name);
        std::exit(3);
    }
    return reinterpret_cast<Fn>(reinterpret_cast<void*>(address));
}

std::string bounded(const char* text) {
    return text != nullptr ? std::string(text) : std::string("<null>");
}

}  // namespace

int main() {
    const char* stub_path = std::getenv("STEAMBRIDGE_STUB");
    if (stub_path == nullptr || stub_path[0] == '\0') {
        std::printf("STEAMBRIDGE_STUB is not set\n");
        return 2;
    }

    HMODULE stub = LoadLibraryA(stub_path);
    if (stub == nullptr) {
        std::printf("cannot load the stub: error %lu\n", static_cast<unsigned long>(GetLastError()));
        return 2;
    }

    const auto api_init = resolve<init_fn>(stub, "SteamAPI_Init");
    const auto api_shutdown = resolve<void_fn>(stub, "SteamAPI_Shutdown");
    const auto api_run_callbacks = resolve<void_fn>(stub, "SteamAPI_RunCallbacks");
    const auto is_steam_running = resolve<bool_fn>(stub, "SteamAPI_IsSteamRunning");
    const auto get_h_user = resolve<count_fn>(stub, "SteamAPI_GetHSteamUser");
    const auto get_h_pipe = resolve<count_fn>(stub, "SteamAPI_GetHSteamPipe");
    const auto install_path = resolve<session_fn>(stub, "SteamAPI_GetSteamInstallPath");
    const auto isteam_user = resolve<interface_fn>(stub, "SteamAPI_ISteamUser");
    const auto isteam_utils = resolve<interface_fn>(stub, "SteamAPI_ISteamUtils");
    const auto isteam_user_stats = resolve<interface_fn>(stub, "SteamAPI_ISteamUserStats");
    const auto get_steam_id = resolve<steam_id_fn>(stub, "SteamAPI_ISteamUser_GetSteamID");
    const auto get_persona = resolve<cstring_self_fn>(stub, "SteamAPI_ISteamFriends_GetPersonaName");
    const auto get_app_id = resolve<app_id_fn>(stub, "SteamAPI_ISteamUtils_GetAppID");
    const auto get_language = resolve<cstring_self_fn>(stub, "SteamAPI_ISteamUtils_GetCurrentGameLanguage");
    const auto get_stat = resolve<stat_get_fn>(stub, "SteamAPI_ISteamUserStats_GetStatInt32");
    const auto set_stat = resolve<stat_set_fn>(stub, "SteamAPI_ISteamUserStats_SetStatInt32");
    const auto get_achievement = resolve<achievement_get_fn>(stub, "SteamAPI_ISteamUserStats_GetAchievement");
    const auto set_achievement = resolve<achievement_set_fn>(stub, "SteamAPI_ISteamUserStats_SetAchievement");
    const auto num_achievements = resolve<count_fn>(stub, "SteamAPI_ISteamUserStats_GetNumAchievements");
    const auto achievement_name = resolve<achievement_name_fn>(stub, "SteamAPI_ISteamUserStats_GetAchievementName");
    const auto store_stats = resolve<bool_self_fn>(stub, "SteamAPI_ISteamUserStats_StoreStats");
    const auto session_id = resolve<session_fn>(stub, "SteamBridge_SessionId");
    const auto bridge_stats = resolve<stats_fn>(stub, "SteamBridge_Stats");

    // --- the way a game boots ---------------------------------------------
    const bool initialised = api_init();
    std::printf("init=%s\n", initialised ? "true" : "false");
    std::printf("is_running=%s\n", is_steam_running() ? "true" : "false");
    std::printf("h_user=%u\n", get_h_user(nullptr));
    std::printf("h_pipe=%u\n", get_h_pipe(nullptr));

    // A string returned by the stub is only valid until the next call on this
    // thread, so a game copies what it wants to keep. That is what the API does
    // here (and what reply_cstring documents).
    const std::string install = bounded(install_path());
    std::printf("install_path_set=%s\n", install.empty() ? "false" : "true");

    const std::string session = bounded(session_id());
    std::printf("session_set=%s\n", session.empty() ? "false" : "true");

    void* const user = isteam_user();
    void* const utils = isteam_utils();
    void* const user_stats = isteam_user_stats();
    std::printf("interfaces=%s\n", (user != nullptr && utils != nullptr && user_stats != nullptr) ? "true" : "false");

    // --- identity ----------------------------------------------------------
    std::printf("steam_id=%llu\n", static_cast<unsigned long long>(get_steam_id(user)));
    std::printf("persona=%s\n", bounded(get_persona(user)).c_str());
    std::printf("app_id=%u\n", get_app_id(utils));
    std::printf("language=%s\n", bounded(get_language(utils)).c_str());

    // --- stats, including an out-parameter --------------------------------
    // One `key=value` per line, each key unique: the end-to-end test parses this
    // output line by line, and a line with two pairs would be ambiguous.
    std::int32_t deaths = -1;
    const bool found_deaths = get_stat(user_stats, "Deaths", &deaths);
    std::printf("stat.Deaths.value=%d\n", static_cast<int>(deaths));
    std::printf("stat.Deaths.found=%s\n", found_deaths ? "true" : "false");

    const bool wrote = set_stat(user_stats, "Deaths", 4);
    std::printf("stat.Deaths.write_ok=%s\n", wrote ? "true" : "false");
    deaths = -1;
    const bool reread = get_stat(user_stats, "Deaths", &deaths);
    std::printf("stat.Deaths.after_write.value=%d\n", static_cast<int>(deaths));
    std::printf("stat.Deaths.after_write.found=%s\n", reread ? "true" : "false");

    std::int32_t unknown = -1;
    const bool found_unknown = get_stat(user_stats, "NoSuchStat", &unknown);
    std::printf("stat.NoSuchStat.found=%s\n", found_unknown ? "true" : "false");
    std::printf("stat.NoSuchStat.value_untouched=%s\n", unknown == -1 ? "true" : "false");

    // --- achievements ------------------------------------------------------
    bool achieved = true;
    const bool found_achievement = get_achievement(user_stats, "ACH_BOOTED", &achieved);
    std::printf("achievement.ACH_BOOTED.value=%s\n", achieved ? "true" : "false");
    std::printf("achievement.ACH_BOOTED.found=%s\n", found_achievement ? "true" : "false");

    const bool unlocked = set_achievement(user_stats, "ACH_BOOTED");
    std::printf("achievement.ACH_BOOTED.unlocked=%s\n", unlocked ? "true" : "false");
    achieved = false;
    get_achievement(user_stats, "ACH_BOOTED", &achieved);
    std::printf("achievement.ACH_BOOTED.after=%s\n", achieved ? "true" : "false");

    bool missing = true;
    const bool found_missing = get_achievement(user_stats, "ACH_MISSING", &missing);
    std::printf("achievement.ACH_MISSING.found=%s\n", found_missing ? "true" : "false");
    std::printf("achievement.ACH_MISSING.value_untouched=%s\n", missing ? "true" : "false");

    std::printf("num_achievements=%u\n", num_achievements(user_stats));
    std::printf("achievement.0=%s\n", bounded(achievement_name(user_stats, 0)).c_str());
    std::printf("store_stats=%s\n", store_stats(user_stats) ? "true" : "false");

    // --- calls nobody answers ---------------------------------------------
    // These are forwarded like everything else; the backend has no opinion about
    // them, so the stub's own no-op is what a game would see with Steam absent.
    api_run_callbacks();
    api_run_callbacks();

    unsigned long unhandled = 0;
    const unsigned long forwarded = bridge_stats(&unhandled);
    std::printf("forwarded=%lu\n", forwarded);
    std::printf("unhandled=%lu\n", unhandled);

    api_shutdown();
    FreeLibrary(stub);
    return 0;
}
