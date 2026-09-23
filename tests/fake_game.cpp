// ============================================================================
//  tests/fake_game.cpp - a stand-in for a game.
// ----------------------------------------------------------------------------
//  It loads the stub the way a game's import table would (by name, from the path
//  in STEAMMOCK_STUB), calls a slice of the flat API through GetProcAddress,
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

// ---------------------------------------------------------------------------
//  What a game built against a recent SDK has: its interfaces by vtable.
// ---------------------------------------------------------------------------
//  Such a game does not import the per-interface accessors - they are inline in
//  its own headers - so it asks SteamInternal_CreateInterface for a version
//  string and calls the object it gets back through the vtable. None of those
//  calls reaches a flat trampoline, which is why the stub hands out objects of
//  its own.
//
//  The declarations below are that game's side of the ABI: the slots in the
//  order its headers lay them out, and the functions are only ever *declared* -
//  the object and its implementation belong to the stub, which is the whole
//  point of the exercise. Only the slots this calls have to be about the right
//  thing; the ones between them are there for their offsets, so a slot the stub
//  put in the wrong place is a call answered with the wrong thing.
//
//  They sit outside the anonymous namespace so that nothing here pretends to
//  define a symbol it does not.

struct SteamID {
    std::uint64_t value;
};

struct ISteamUser019 {
    virtual std::int32_t GetHSteamUser();
    virtual bool BLoggedOn();
    virtual SteamID GetSteamID();
};

struct ISteamUtils009 {
    virtual std::uint32_t GetSecondsSinceAppActive();
    virtual std::uint32_t GetSecondsSinceComputerActive();
    virtual std::int32_t GetConnectedUniverse();
    virtual std::uint32_t GetServerRealTime();
    virtual const char* GetIPCountry();
    virtual bool GetImageSize(std::int32_t image, std::uint32_t* width, std::uint32_t* height);
    virtual bool GetImageRGBA(std::int32_t image, std::uint8_t* destination, std::int32_t size);
    virtual void GetCSERIPPort(std::uint32_t* ip, std::uint16_t* port);
    virtual std::uint8_t GetCurrentBatteryPower();
    virtual std::uint32_t GetAppID();
};

// The stats interface, for the two things the flat checks cannot show: an
// out-parameter written back through a *vtable* call, and a value class - a
// CSteamID - travelling as an argument. The slots are the layouts' order, which
// for the two GetStat overloads is the DLL's (float first) rather than the order
// the SDK's own headers declare them in. Nothing here calls those two: they are
// declared because the offsets of what follows depend on the count.
struct ISteamUserStats011 {
    virtual bool RequestCurrentStats();
    virtual bool GetStat(const char* name, float* value);
    virtual bool GetStat(const char* name, std::int32_t* value);
    virtual bool SetStat(const char* name, float value);
    virtual bool SetStat(const char* name, std::int32_t value);
    virtual bool UpdateAvgRateStat(const char* name, float count, double seconds);
    virtual bool GetAchievement(const char* name, bool* achieved);
    virtual bool SetAchievement(const char* name);
    virtual bool ClearAchievement(const char* name);
    virtual bool GetAchievementAndUnlockTime(const char* name, bool* achieved, std::uint32_t* time);
    virtual bool StoreStats();
    virtual std::int32_t GetAchievementIcon(const char* name);
    virtual const char* GetAchievementDisplayAttribute(const char* name, const char* key);
    virtual bool IndicateAchievementProgress(const char* name, std::uint32_t current,
                                             std::uint32_t maximum);
    virtual std::uint32_t GetNumAchievements();
    virtual const char* GetAchievementName(std::uint32_t index);
    virtual std::uint64_t RequestUserStats(SteamID user);
    virtual bool GetUserStat(SteamID user, const char* name, float* value);
    virtual bool GetUserStat(SteamID user, const char* name, std::int32_t* value);
    virtual bool GetUserAchievement(SteamID user, const char* name, bool* achieved);
};

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
using create_interface_fn = void* (*)(const char*);

template <typename Fn> Fn resolve(HMODULE module, const char* name) {
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
    const char* stub_path = std::getenv("STEAMMOCK_STUB");
    if (stub_path == nullptr || stub_path[0] == '\0') {
        std::printf("STEAMMOCK_STUB is not set\n");
        return 2;
    }

    HMODULE stub = LoadLibraryA(stub_path);
    if (stub == nullptr) {
        std::printf("cannot load the stub: error %lu\n",
                    static_cast<unsigned long>(GetLastError()));
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
    const auto get_persona =
        resolve<cstring_self_fn>(stub, "SteamAPI_ISteamFriends_GetPersonaName");
    const auto get_app_id = resolve<app_id_fn>(stub, "SteamAPI_ISteamUtils_GetAppID");
    const auto get_language =
        resolve<cstring_self_fn>(stub, "SteamAPI_ISteamUtils_GetCurrentGameLanguage");
    const auto get_stat = resolve<stat_get_fn>(stub, "SteamAPI_ISteamUserStats_GetStatInt32");
    const auto set_stat = resolve<stat_set_fn>(stub, "SteamAPI_ISteamUserStats_SetStatInt32");
    const auto get_achievement =
        resolve<achievement_get_fn>(stub, "SteamAPI_ISteamUserStats_GetAchievement");
    const auto set_achievement =
        resolve<achievement_set_fn>(stub, "SteamAPI_ISteamUserStats_SetAchievement");
    const auto num_achievements =
        resolve<count_fn>(stub, "SteamAPI_ISteamUserStats_GetNumAchievements");
    const auto achievement_name =
        resolve<achievement_name_fn>(stub, "SteamAPI_ISteamUserStats_GetAchievementName");
    const auto store_stats = resolve<bool_self_fn>(stub, "SteamAPI_ISteamUserStats_StoreStats");
    const auto session_id = resolve<session_fn>(stub, "SteamMock_SessionId");
    const auto bridge_stats = resolve<stats_fn>(stub, "SteamMock_Stats");
    const auto create_interface =
        resolve<create_interface_fn>(stub, "SteamInternal_CreateInterface");

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
    std::printf("interfaces=%s\n",
                (user != nullptr && utils != nullptr && user_stats != nullptr) ? "true" : "false");

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

    // --- the same API as a recent SDK asks for it -------------------------
    // Through the vtable, where no trampoline of ours is in the way.
    void* const modern_user = create_interface("SteamUser019");
    void* const modern_utils = create_interface("SteamUtils009");
    void* const modern_stats = create_interface("STEAMUSERSTATS_INTERFACE_VERSION011");
    std::printf("vtable.user=%s\n", modern_user != nullptr ? "true" : "false");
    std::printf("vtable.utils=%s\n", modern_utils != nullptr ? "true" : "false");
    std::printf("vtable.stats=%s\n", modern_stats != nullptr ? "true" : "false");
    std::printf("vtable.unknown=%s\n",
                create_interface("SteamUser999") == nullptr ? "true" : "false");

    if (modern_user != nullptr) {
        auto* modern_user_object = static_cast<ISteamUser019*>(modern_user);
        // Answered by the profile, exactly as the flat call is.
        std::printf("vtable.steam_id=%llu\n",
                    static_cast<unsigned long long>(modern_user_object->GetSteamID().value));
        // Nobody has an opinion about this one, so it is the game's own default.
        std::printf("vtable.h_user=%d\n", static_cast<int>(modern_user_object->GetHSteamUser()));
        std::printf("vtable.logged_on=%s\n", modern_user_object->BLoggedOn() ? "true" : "false");
    }

    if (modern_utils != nullptr) {
        auto* modern_utils_object = static_cast<ISteamUtils009*>(modern_utils);
        std::printf("vtable.app_id=%u\n", static_cast<unsigned>(modern_utils_object->GetAppID()));

        // An out-parameter through the vtable, for a call nobody answers: what
        // the game passed has to come back untouched.
        std::uint32_t width = 999;
        std::uint32_t height = 999;
        const bool sized = modern_utils_object->GetImageSize(0, &width, &height);
        std::printf("vtable.image_size_answered=%s\n", sized ? "true" : "false");
        std::printf("vtable.image_size_untouched=%s\n",
                    (width == 999 && height == 999) ? "true" : "false");
    }

    if (modern_stats != nullptr) {
        auto* stats = static_cast<ISteamUserStats011*>(modern_stats);

        // An out-parameter of a call the backend *does* answer, through the
        // vtable: the game asked whether ACH_FINISHED is unlocked, and this
        // starts at true so that only a write-back can make it false.
        bool modern_achieved = true;
        const bool found = stats->GetAchievement("ACH_FINISHED", &modern_achieved);
        std::printf("vtable.achievement.found=%s\n", found ? "true" : "false");
        std::printf("vtable.achievement.written=%s\n", modern_achieved ? "true" : "false");

        // And the same question about the achievement the *flat* call unlocked
        // earlier: one session, two ways to ask it.
        modern_achieved = false;
        stats->GetAchievement("ACH_BOOTED", &modern_achieved);
        std::printf("vtable.achievement.unlocked=%s\n", modern_achieved ? "true" : "false");

        // A value class as an argument, for a call nobody answers. Nothing can be
        // read back here - the IDs are in the transcript - but the steam id has
        // to survive being passed by value for it to be there.
        const SteamID user_id{76561198000000001ull};
        modern_achieved = true;
        const bool answered = stats->GetUserAchievement(user_id, "ACH_BOOTED", &modern_achieved);
        std::printf("vtable.user_achievement.answered=%s\n", answered ? "true" : "false");
        std::printf("vtable.user_achievement.untouched=%s\n", modern_achieved ? "true" : "false");
    }

    unsigned long unhandled = 0;
    const unsigned long forwarded = bridge_stats(&unhandled);
    std::printf("forwarded=%lu\n", forwarded);
    std::printf("unhandled=%lu\n", unhandled);

    api_shutdown();
    FreeLibrary(stub);
    return 0;
}
