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
#include <exception>
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
using register_callback_fn = void (*)(void*, std::int32_t);
using send_p2p_fn = bool (*)(void*, std::uint64_t, const void*, std::uint32_t, std::int32_t,
                             std::int32_t);
using begin_auth_fn = std::int32_t (*)(void*, void*, std::int32_t, std::uint64_t);

#if defined(_M_IX86)
#    define STEAMMOCK_TEST_CALL __thiscall
#else
#    define STEAMMOCK_TEST_CALL
#endif

// A callback object as the SDK lays one out, with its vtable written out here rather than
// left to the compiler. CCallbackBase's slots are Run(pvParam), Run(pvParam, bIOFailure,
// hSteamAPICall) and GetCallbackSizeBytes, and the stub reads them in that order: a class
// that declares the virtuals itself gets whatever order the compiler chooses, and a class
// that overrides two overloads of Run gets one that is not this - which is how this test
// first read the stub's *size* query as a callback invocation and saw both of its objects
// called for one delivery.
struct CountedCallback {
    const void* const* vtable;
    int* calls;
};

void STEAMMOCK_TEST_CALL counted_run(void* self, void* /*payload*/) {
    ++(*static_cast<CountedCallback*>(self)->calls);
}

void STEAMMOCK_TEST_CALL counted_run_of_a_call_result(void*, void*, bool, std::uint64_t) {}

std::int32_t STEAMMOCK_TEST_CALL counted_size(void*) { return 0; }

const void* const kCountedVtable[] = {
    reinterpret_cast<const void*>(&counted_run),
    reinterpret_cast<const void*>(&counted_run_of_a_call_result),
    reinterpret_cast<const void*>(&counted_size),
};

// ValidateAuthTicketResponse_t and P2PSessionRequest_t, as the SDK numbers them. The
// layouts are Valve's data and are not part of the checkout, so these cannot be read out of
// them here - and if either number ever moved, the objects below would not be called at all
// and the checks that follow would fail rather than pass quietly.
constexpr std::int32_t kValidateAuthTicketResponse = 143;
constexpr std::int32_t kP2PSessionRequest = 1202;

template <typename Fn> Fn resolve(HMODULE module, const char* name) {
    const FARPROC address = GetProcAddress(module, name);
    if (address == nullptr) {
        std::printf("missing export: %s\n", name);
        std::exit(3);
    }
    // Two casts, because Windows hands a function out as `FARPROC` and this is the
    // documented way back to the pointer type the game's own import table would have
    // had. The check is about losing the signature, and losing it is the point: the
    // export's name is what says which signature it is, and the process exits 3 rather
    // than calling through anything that was not there. What the name means is then
    // pinned by the checks that call it, which is what makes this a test and not faith.
    // NOLINTNEXTLINE(bugprone-casting-through-void)
    return reinterpret_cast<Fn>(reinterpret_cast<void*>(address));
}

std::string bounded(const char* text) {
    return text != nullptr ? std::string(text) : std::string("<null>");
}

}  // namespace

int run() {
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

    // The stub exports a Steam API only when an SDK's surface was imported into
    // gen/steam_api_surface.json. Without one there is no flat API to call and
    // nothing this stand-in can check, so it says so in one line and stops - the
    // end-to-end test reads that and skips the half that needs a surface, rather
    // than this process dying on the first export it cannot find.
    if (GetProcAddress(stub, "SteamAPI_Init") == nullptr) {
        std::printf("surface=none\n");
        FreeLibrary(stub);
        return 0;
    }

    const auto api_init = resolve<init_fn>(stub, "SteamAPI_Init");
    const auto api_shutdown = resolve<void_fn>(stub, "SteamAPI_Shutdown");
    const auto api_run_callbacks = resolve<void_fn>(stub, "SteamAPI_RunCallbacks");
    const auto is_steam_running = resolve<bool_fn>(stub, "SteamAPI_IsSteamRunning");
    const auto get_h_user = resolve<count_fn>(stub, "SteamAPI_GetHSteamUser");
    const auto get_h_pipe = resolve<count_fn>(stub, "SteamAPI_GetHSteamPipe");
    const auto install_path = resolve<session_fn>(stub, "SteamAPI_GetSteamInstallPath");
    const auto get_steam_id = resolve<steam_id_fn>(stub, "SteamAPI_ISteamUser_GetSteamID");
    const auto get_persona =
        resolve<cstring_self_fn>(stub, "SteamAPI_ISteamFriends_GetPersonaName");
    const auto get_app_id = resolve<app_id_fn>(stub, "SteamAPI_ISteamUtils_GetAppID");
    // The language a game is running in is ISteamApps' call, and that is the name
    // the layouts put on the wire.
    const auto get_language =
        resolve<cstring_self_fn>(stub, "SteamAPI_ISteamApps_GetCurrentGameLanguage");
    const auto get_stat = resolve<stat_get_fn>(stub, "SteamAPI_ISteamUserStats_GetStat");
    const auto set_stat = resolve<stat_set_fn>(stub, "SteamAPI_ISteamUserStats_SetStat");
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

    // An interface comes from the factory, the way this SDK's own inline accessors
    // get one: the exported `SteamAPI_ISteamXxx()` functions only appear from 1.51
    // on, and a game built against anything older asks for a version string.
    void* const user = create_interface("SteamUser019");
    void* const utils = create_interface("SteamUtils009");
    void* const user_stats = create_interface("STEAMUSERSTATS_INTERFACE_VERSION011");
    void* const apps = create_interface("STEAMAPPS_INTERFACE_VERSION008");
    std::printf("interfaces=%s\n",
                (user != nullptr && utils != nullptr && user_stats != nullptr) ? "true" : "false");

    // --- identity ----------------------------------------------------------
    // Asked once, and kept: the flat call and the vtable call below are checks about
    // reaching the backend under one name, so a third call would move their count.
    const std::uint64_t own_steam_id = get_steam_id(user);
    std::printf("steam_id=%llu\n", static_cast<unsigned long long>(own_steam_id));
    std::printf("persona=%s\n", bounded(get_persona(user)).c_str());
    std::printf("app_id=%u\n", get_app_id(utils));
    std::printf("language=%s\n", bounded(get_language(apps)).c_str());

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

    // --- one callback id, two objects, and which of them hears it -----------
    // A process that hosts registers ValidateAuthTicketResponse_t on both of its halves -
    // its game server, and its client once per peer - so one id can have several objects on
    // it, and which of them is called is the whole of what the registry is for. The answer
    // to a *game server's* ticket check belongs to the end that asked, whose object is the
    // first registered because a hosting process's game server comes up before its client
    // has any peers. A payload that answers nothing in particular - a peer wanting to talk -
    // belongs to the customer, which is the object registered last.
    //
    // Getting the first rule wrong is what leaves a player out of a three-player match: the
    // ticket comes back validated, the game server is never told, and that player waits out
    // the game's own 30-second ticket timeout and leaves.
    const auto register_callback = resolve<register_callback_fn>(stub, "SteamAPI_RegisterCallback");
    int server_first = 0;
    int server_second = 0;
    CountedCallback server_object{kCountedVtable, &server_first};
    CountedCallback peer_object{kCountedVtable, &server_second};
    register_callback(&server_object, kValidateAuthTicketResponse);
    register_callback(&peer_object, kValidateAuthTicketResponse);

    // A game server asking Steam about a ticket it was handed. The answer is a callback, and
    // it arrives on the next pump.
    const auto begin_auth =
        resolve<begin_auth_fn>(stub, "SteamAPI_ISteamGameServer_BeginAuthSession");
    char ticket[14] = {};
    const std::int32_t begin_result = begin_auth(nullptr, ticket, sizeof(ticket), own_steam_id);
    api_run_callbacks();
    api_run_callbacks();
    std::printf("callback.begin_auth_result=%d\n", static_cast<int>(begin_result));
    std::printf("callback.server_first=%d\n", server_first);
    std::printf("callback.server_second=%d\n", server_second);

    int peer_first = 0;
    int peer_second = 0;
    CountedCallback first_customer{kCountedVtable, &peer_first};
    CountedCallback second_customer{kCountedVtable, &peer_second};
    register_callback(&first_customer, kP2PSessionRequest);
    register_callback(&second_customer, kP2PSessionRequest);

    // A packet addressed to this process's own id is the first thing these two ends say to
    // each other, which is what makes the world tell the receiver to accept the session -
    // and that payload answers no call, so it is the customer's.
    const auto send_p2p = resolve<send_p2p_fn>(stub, "SteamAPI_ISteamNetworking_SendP2PPacket");
    std::printf("callback.sent=%s\n",
                send_p2p(nullptr, own_steam_id, "x", 1, 3, 0) ? "true" : "false");
    api_run_callbacks();
    api_run_callbacks();
    std::printf("callback.peer_first=%d\n", peer_first);
    std::printf("callback.peer_second=%d\n", peer_second);

    api_shutdown();
    FreeLibrary(stub);
    return 0;
}

// A game does not get a message when `main` throws, and the end-to-end test reads this
// program's output to decide what happened - so it says so rather than dying silently.
int main() {
    try {
        return run();
    } catch (const std::exception& error) {
        std::printf("the fake game threw: %s\n", error.what());
        return 2;
    } catch (...) {
        std::printf("the fake game threw something that is not a std::exception\n");
        return 2;
    }
}
