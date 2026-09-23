// ============================================================================
//  C++ unit tests: the backend's own logic.
// ----------------------------------------------------------------------------
//  What used to live in python/tests/test_protocol.py, plus the check that the
//  two halves of the harness still agree: every call the session state machine
//  answers has to be in the generated surface, or the stub exports something the
//  backend has quietly stopped having an opinion about.
//
//  These never open a socket: the server is exercised end to end by the
//  end_to_end test, and everything decided here is decided before a byte moves.
//
//  Exits non-zero if a check fails.
// ============================================================================

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <string>
#include <vector>

#include "bridge/frame.hpp"
#include "bridge/json.hpp"
#include "bridge/lobby.hpp"
#include "bridge/protocol.hpp"
#include "bridge/scenario.hpp"
#include "bridge/session.hpp"
#include "bridge/surface.hpp"

#ifndef STEAMMOCK_SCENARIO_PATH
#    error "STEAMMOCK_SCENARIO_PATH must name the example scenario"
#endif

namespace {

using steammock::Answer;
using steammock::Dispatcher;
using steammock::Json;
using steammock::LobbyWorld;
using steammock::Profile;
using steammock::Session;

int g_failures = 0;

void check(const char* what, bool ok) {
    std::printf("  [%s] %s\n", ok ? "ok  " : "FAIL", what);
    if (!ok) {
        ++g_failures;
    }
}

std::string text_of(const Json& value, const char* key) {
    const Json* member = value.find(key);
    return member != nullptr && member->is_string() ? member->as_string() : std::string();
}

std::int64_t int_of(const Json& value, const char* key) {
    const Json* member = value.find(key);
    return member != nullptr ? member->as_int64() : 0;
}

bool out_flag(const Answer& answer, const char* key) {
    const Json* value = answer.out.find(key);
    return value != nullptr && value->as_bool();
}

std::int64_t out_int(const Answer& answer, const char* key) {
    const Json* value = answer.out.find(key);
    return value != nullptr ? value->as_int64() : 0;
}

bool has_out(const Answer& answer) { return steammock::carries_out(answer.out); }

// The same fixture the Python tests used, so the ported expectations still mean
// something: one game, one known stat, one locked achievement.
Profile make_profile(const char* scripted = "{}") {
    const std::string text =
        std::string("{\"app_id\":480,\"steam_id\":76561198000000001,\"persona_name\":\"Tester\",") +
        "\"language\":\"english\",\"stats\":{\"Deaths\":3}," +
        "\"achievements\":[{\"name\":\"ACH_BOOTED\",\"achieved\":false}],\"scripted\":" + scripted +
        "}";
    Json data;
    if (!Json::parse(text, data)) {
        check("the test fixture parses", false);
        return Profile{};
    }
    return Profile::from_json("test", data);
}

Session make_session(const char* scripted = "{}") {
    Json hello = Json::object();
    hello.set("exe", Json::string("game.exe"));
    hello.set("pid", Json::integer(1234));
    return Session("session0", hello, make_profile(scripted));
}

Json name_argument(const char* name) {
    Json args = Json::object();
    args.set("pchName", Json::string(name));
    return args;
}

// ---------------------------------------------------------------------------

void test_replies() {
    std::printf("[:] replies\n");

    const Json unanswered = steammock::make_reply(7, false, Json::null(), Json::null());
    check("an unanswered reply says default", text_of(unanswered, "answer") == "default");
    check("an unanswered reply carries no return value", unanswered.find("ret") == nullptr);
    check("an unanswered reply carries no out parameters", unanswered.find("out") == nullptr);
    check("a reply echoes the sequence", int_of(unanswered, "seq") == 7);
    check("a reply carries the protocol version",
          int_of(unanswered, "v") == steammock::kProtocolVersion);
    check("ping: the frame cap is still the one the stub mirrors",
          steammock::kMaxFrameBytes == 4u * 1024u * 1024u);

    Json out = Json::object();
    out.set("pData", Json::integer(42));
    const Json answered = steammock::make_reply(8, true, Json::boolean(true), out);
    check("an answered reply says handled", text_of(answered, "answer") == "handled");
    check("an answered reply carries the value",
          answered.find("ret") != nullptr && answered.find("ret")->as_bool());
    check("an answered reply carries out parameters",
          answered.find("out") != nullptr && answered.find("out")->find("pData") != nullptr &&
              answered.find("out")->find("pData")->as_int64() == 42);

    check("an answer without out parameters omits out",
          steammock::make_reply(9, true, Json::integer(3), Json::null()).find("out") == nullptr);
    check("an empty out object is not carried",
          steammock::make_reply(10, true, Json::integer(3), Json::object()).find("out") == nullptr);

    // A scenario that scripts a call without saying what it returns means "the
    // zero of your type", not "no opinion" - and that distinction is the whole
    // point of the protocol.
    const Json bare = steammock::make_reply(11, true, Json::null(), Json::null());
    check("an answered reply with no value still has ret", bare.find("ret") != nullptr);
    check("and that ret is null", bare.find("ret")->is_null());
}

void test_relabelling() {
    std::printf("[:] where an answer came from\n");

    const Dispatcher dispatcher;
    Session scripted_session = make_session("{\"SteamAPI_ISteamUtils_GetAppID\":{\"ret\":999}}");
    const Answer win =
        dispatcher.answer(scripted_session, "SteamAPI_ISteamUtils_GetAppID", Json::object());
    check("a scripted call wins over the state machine", win.answered);
    check("the scripted value is the one used", win.ret.as_int64() == 999);
    check("it is labelled scripted", win.via == "scripted");

    Session declining = make_session("{\"SteamAPI_Init\":{\"answer\":\"default\"}}");
    const Answer declined = dispatcher.answer(declining, "SteamAPI_Init", Json::object());
    check("a scripted call can decline to answer", !declined.answered);
    check("a declined scripted call is still labelled scripted", declined.via == "scripted");

    Session state_session = make_session();
    const Answer from_state =
        dispatcher.answer(state_session, "SteamAPI_GetHSteamUser", Json::object());
    check("state answers are labelled state", from_state.answered && from_state.via == "state");
    check("the state answer is the documented constant", from_state.ret.as_int64() == 1);

    Session none_session = make_session();
    const Answer unanswered = dispatcher.answer(none_session, "SteamAPI_Shutdown", Json::object());
    check("an unanswered call is labelled none", !unanswered.answered && unanswered.via == "none");
}

void test_identity() {
    std::printf("[:] identity comes from the profile\n");

    Session session = make_session();
    check("the steam id is the profile's",
          session.handle("SteamAPI_ISteamUser_GetSteamID", Json::object()).ret.as_uint64() ==
              76561198000000001ull);
    check("the persona name is the profile's",
          session.handle("SteamAPI_ISteamFriends_GetPersonaName", Json::object()).ret.as_string() ==
              "Tester");
    check("the app id is the profile's",
          session.handle("SteamAPI_ISteamUtils_GetAppID", Json::object()).ret.as_int64() == 480);
    check("the language is the profile's",
          session.handle("SteamAPI_ISteamUtils_GetCurrentGameLanguage", Json::object())
                  .ret.as_string() == "english");
    check("the install path is reported",
          !session.handle("SteamAPI_GetSteamInstallPath", Json::object()).ret.as_string().empty());
    check("the build id is reported",
          session.handle("SteamAPI_ISteamApps_GetAppBuildId", Json::object()).ret.as_int64() == 1);

    const Answer unknown = session.handle("SteamAPI_Init", Json::object());
    check("a call nobody knows has no opinion", !unknown.answered);
    check("an unanswered state call carries nothing",
          unknown.ret.is_null() && unknown.out.is_null());
}

void test_stats() {
    std::printf("[:] stats\n");

    Session session = make_session();
    const Answer read =
        session.handle("SteamAPI_ISteamUserStats_GetStatInt32", name_argument("Deaths"));
    check("a known stat is answered", read.answered && read.ret.as_bool());
    check("a stat is read as named out parameters", out_int(read, "pData") == 3);

    Json write = name_argument("Deaths");
    write.set("nData", Json::integer(9));
    check("writing a stat is accepted",
          session.handle("SteamAPI_ISteamUserStats_SetStatInt32", write).ret.as_bool());

    const Answer reread =
        session.handle("SteamAPI_ISteamUserStats_GetStatInt32", name_argument("Deaths"));
    check("reading it back sees the new value", out_int(reread, "pData") == 9);
    check("the write is remembered for a transcript",
          session.stats_written().size() == 1u && session.stats_written()[0].second == 9);

    // A game may invent a stat locally without the scenario listing it first.
    Json invented = name_argument("Invented");
    invented.set("nData", Json::integer(1));
    session.handle("SteamAPI_ISteamUserStats_SetStatInt32", invented);
    check("a stat a game invents can be read back",
          session.handle("SteamAPI_ISteamUserStats_GetStatInt32", invented).ret.as_bool());

    const Answer missing =
        session.handle("SteamAPI_ISteamUserStats_GetStatInt32", name_argument("NoSuchStat"));
    check("an unknown stat is still answered", missing.answered);
    check("an unknown stat fails, like Steam", !missing.ret.as_bool());
    check("an unknown stat leaves the caller's variable alone", !has_out(missing));
}

void test_achievements() {
    std::printf("[:] achievements\n");

    Session session = make_session();
    const Answer locked =
        session.handle("SteamAPI_ISteamUserStats_GetAchievement", name_argument("ACH_BOOTED"));
    check("an achievement can be read", locked.answered && locked.ret.as_bool());
    check("it starts locked", !out_flag(locked, "pbAchieved"));

    check("unlocking it is accepted",
          session.handle("SteamAPI_ISteamUserStats_SetAchievement", name_argument("ACH_BOOTED"))
              .ret.as_bool());
    const Answer unlocked =
        session.handle("SteamAPI_ISteamUserStats_GetAchievement", name_argument("ACH_BOOTED"));
    check("it reads back as unlocked", out_flag(unlocked, "pbAchieved"));
    check("the unlock is remembered for a transcript",
          session.achievements_set().size() == 1u && session.achievements_set()[0] == "ACH_BOOTED");

    const Answer missing =
        session.handle("SteamAPI_ISteamUserStats_GetAchievement", name_argument("ACH_MISSING"));
    check("an unknown achievement reports failure", missing.answered && !missing.ret.as_bool());
    check("unlocking an unknown achievement reports failure",
          !session.handle("SteamAPI_ISteamUserStats_SetAchievement", name_argument("ACH_MISSING"))
               .ret.as_bool());

    check("the list is counted",
          session.handle("SteamAPI_ISteamUserStats_GetNumAchievements", Json::object())
                  .ret.as_int64() == 1);
    Json first = Json::object();
    first.set("iAchievement", Json::integer(0));
    check("achievement names are indexed",
          session.handle("SteamAPI_ISteamUserStats_GetAchievementName", first).ret.as_string() ==
              "ACH_BOOTED");
    Json past_the_end = Json::object();
    past_the_end.set("iAchievement", Json::integer(9));
    check("an index past the end is empty text",
          session.handle("SteamAPI_ISteamUserStats_GetAchievementName", past_the_end)
              .ret.as_string()
              .empty());
}

void test_describe() {
    std::printf("[:] a session can describe itself\n");

    const Session session = make_session();
    const std::string text = session.describe();
    check("the description names the session", text.find("session0") != std::string::npos);
    check("the description names the executable", text.find("game.exe") != std::string::npos);
    check("the description names the profile", text.find("test") != std::string::npos);
    check("the process id is read from the hello", session.pid() == 1234);
}

void test_scenarios() {
    std::printf("[:] scenarios and match rules\n");

    Json document;
    const std::string text = "{\"profiles\":{\"default\":{\"app_id\":1},\"other\":{\"app_id\":2}},"
                             "\"match\":[{\"exe_contains\":\"special\",\"profile\":\"other\"},"
                             "{\"exe_contains\":\"game\",\"profile\":\"default\"}],"
                             "\"default_profile\":\"default\"}";
    check("the scenario parses", Json::parse(text, document));
    const Dispatcher dispatcher(document);

    Json special = Json::object();
    special.set("exe", Json::string("my_special_game.exe"));
    check("a match rule can pick a profile by executable",
          dispatcher.profile_for(special).app_id == 2);

    Json plain = Json::object();
    plain.set("exe", Json::string("game.exe"));
    check("the first matching rule wins", dispatcher.profile_for(plain).app_id == 1);

    Json unknown = Json::object();
    unknown.set("exe", Json::string("unknown.exe"));
    check("an unmatched game gets the default", dispatcher.profile_for(unknown).app_id == 1);

    Json typo_document;
    check("the typo scenario parses",
          Json::parse("{\"profiles\":{\"default\":{\"app_id\":1}},"
                      "\"match\":[{\"exe_contains\":\"game\",\"profile\":\"typo\"}]}",
                      typo_document));
    const Dispatcher typo(typo_document);
    Json game = Json::object();
    game.set("exe", Json::string("game.exe"));
    check("a rule naming an unknown profile falls back to the default",
          typo.profile_for(game).app_id == 1);

    const Dispatcher empty{Json::object()};
    check("an empty scenario still has a default profile", empty.has_profile("default"));
    check("the default profile is named", empty.default_profile() == "default");

    Dispatcher loaded;
    std::string error;
    const bool ok = Dispatcher::load_file(STEAMMOCK_SCENARIO_PATH, loaded, error);
    check("the bundled example scenario loads", ok);
    if (!ok) {
        std::printf("        %s\n", error.c_str());
        return;
    }
    Json fake_game = Json::object();
    fake_game.set("exe", Json::string("fake_game.exe"));
    const Profile profile = loaded.profile_for(fake_game);
    check("the example gives fake_game the default profile", profile.app_id == 480);
    check("the example scripts SteamAPI_Init",
          profile.scripted_for("SteamAPI_Init") != nullptr &&
              profile.scripted_for("SteamAPI_Init")->find("ret")->as_bool());
    check("the example seeds a stat",
          profile.find_stat("Deaths") != nullptr && *profile.find_stat("Deaths") == 0);
    check("the example names both games", loaded.profile_names().size() == 2u);
    check("the example carries match rules", loaded.match_rules().size() == 2u);

    Dispatcher nowhere;
    std::string why;
    check("a scenario that is not there reports why",
          !Dispatcher::load_file("no/such/scenario.json", nowhere, why) && !why.empty());
}

// Each session gets its own copy of a profile, so two games matched to the same
// scenario cannot see each other's stats. The README says several games run side
// by side "each with its own profile"; this is what makes that true.
void test_profiles_are_per_session() {
    std::printf("[:] sessions do not share their profile\n");

    const Dispatcher dispatcher;
    Json game = Json::object();
    game.set("exe", Json::string("game.exe"));
    const Profile matched = dispatcher.profile_for(game);

    Session one("one", Json::object(), matched);
    Session two("two", Json::object(), matched);

    Json write = name_argument("Deaths");
    write.set("nData", Json::integer(77));
    one.handle("SteamAPI_ISteamUserStats_SetStatInt32", write);

    check(
        "the game that wrote sees its own value",
        one.handle("SteamAPI_ISteamUserStats_GetStatInt32", name_argument("Deaths")).ret.as_bool());
    check("the other game is not handed that value",
          !two.handle("SteamAPI_ISteamUserStats_GetStatInt32", name_argument("Deaths"))
               .ret.as_bool());
}

// The lobbies a run holds are the one piece of backend state that is not per game,
// so what is worth proving is what two games can see of each other through it: one
// makes a room, the other finds it, joins it, and reads back a roster that neither
// of them was told.
void test_the_lobbies_a_run_holds() {
    std::printf("[:] the lobbies a run holds\n");

    auto profile_for = [](const char* persona, std::uint64_t steam_id) {
        const std::string text = std::string("{\"app_id\":480,\"steam_id\":") +
                                 std::to_string(steam_id) + ",\"persona_name\":\"" + persona +
                                 "\",\"language\":\"english\"}";
        Json data;
        check("the lobby fixture parses", Json::parse(text, data));
        return Profile::from_json(persona, data);
    };
    constexpr std::uint64_t kHostId = 76561198000000001ull;
    constexpr std::uint64_t kGuestId = 76561198000000002ull;

    auto session_for = [](const char* id, const Profile& profile) {
        Json hello = Json::object();
        hello.set("exe", Json::string("game.exe"));
        hello.set("pid", Json::integer(1234));
        return Session(id, hello, profile);
    };

    Session host = session_for("host", profile_for("Host", kHostId));
    Session guest = session_for("guest", profile_for("Guest", kGuestId));

    LobbyWorld world;
    // Cleared by hand because the world appends: whoever a call tells is not whoever
    // the next call tells.
    std::vector<std::pair<std::uint64_t, Json>> told;

    auto ask = [&](Session& who, const char* call, const Json& args) {
        Answer answer;
        told.clear();
        const bool handled = world.answer(who, call, args, answer, told);
        check((std::string("the world answers ") + call).c_str(), handled);
        return answer;
    };
    // The field of the payload a call came with, which is how the world completes one.
    auto field_of = [](const Answer& answer, const char* field) -> const Json* {
        if (!answer.events.is_array() || answer.events.items().empty()) {
            return nullptr;
        }
        const Json* in = answer.events.items().front().find("in");
        return in != nullptr ? in->find(field) : nullptr;
    };
    auto lobby_argument = [](const char* key, std::uint64_t value, const char* call) {
        (void)call;
        Json args = Json::object();
        args.set(key, Json::integer(static_cast<std::int64_t>(value)));
        return args;
    };

    {
        const Answer listed =
            ask(host, "SteamAPI_ISteamMatchmaking_RequestLobbyList", Json::object());
        const Json* count = field_of(listed, "m_nLobbiesMatching");
        check("a run with no lobbies lists none", count != nullptr && count->as_int64() == 0);
    }

    Json create = Json::object();
    create.set("eLobbyType", Json::integer(2));
    create.set("cMaxMembers", Json::integer(4));
    const Answer created = ask(host, "SteamAPI_ISteamMatchmaking_CreateLobby", create);
    const Json* made = field_of(created, "m_ulSteamIDLobby");
    check("creating a lobby hands one back", made != nullptr && made->as_uint64() != 0);
    check("and the world says so", created.via == "lobby");

    const Json* call_of_payload = nullptr;
    if (created.events.is_array() && !created.events.items().empty()) {
        call_of_payload = created.events.items().front().find("call");
    }
    check("the payload completes the call the answer returned",
          call_of_payload != nullptr && call_of_payload->is_number() &&
              call_of_payload->as_uint64() == created.ret.as_uint64());

    const std::uint64_t lobby = made != nullptr ? made->as_uint64() : 0;

    // The host names it, the way a game does, and the world keeps that.
    Json named = lobby_argument("steamIDLobby", lobby, "named");
    named.set("pchKey", Json::string("name"));
    named.set("pchValue", Json::string("A lobby"));
    ask(host, "SteamAPI_ISteamMatchmaking_SetLobbyData", named);

    // Now the other game can see it.
    const Answer listed = ask(guest, "SteamAPI_ISteamMatchmaking_RequestLobbyList", Json::object());
    const Json* count = field_of(listed, "m_nLobbiesMatching");
    check("the other game lists the one that exists", count != nullptr && count->as_int64() == 1);

    Json index = Json::object();
    index.set("iLobby", Json::integer(0));
    const Answer found = ask(guest, "SteamAPI_ISteamMatchmaking_GetLobbyByIndex", index);
    check("and it is the host's lobby rather than a lookalike",
          found.ret.is_number() && found.ret.as_uint64() == lobby);

    Json name_key = lobby_argument("steamIDLobby", lobby, "name key");
    name_key.set("pchKey", Json::string("name"));
    check("with the name the host wrote",
          ask(guest, "SteamAPI_ISteamMatchmaking_GetLobbyData", name_key).ret.as_string() ==
              "A lobby");

    // It joins, and the room is told - including the game that joined.
    const Answer entered = ask(guest, "SteamAPI_ISteamMatchmaking_JoinLobby",
                               lobby_argument("steamIDLobby", lobby, "join"));
    const Json* room_joined = field_of(entered, "m_ulSteamIDLobby");
    check("joining says which room it is in",
          room_joined != nullptr && room_joined->as_uint64() == lobby);
    check("both members of the room are told", told.size() == 4);

    bool told_host = false;
    bool told_guest = false;
    for (const auto& note : told) {
        told_host = told_host || note.first == kHostId;
        told_guest = told_guest || note.first == kGuestId;
    }
    check("the host is told", told_host);
    check("and so is the game that just joined", told_guest);

    // What the two of them read now is the same room, seen from either side.
    check("the room has two members", ask(host, "SteamAPI_ISteamMatchmaking_GetNumLobbyMembers",
                                          lobby_argument("steamIDLobby", lobby, "members"))
                                              .ret.as_int64() == 2);

    Json second_member = lobby_argument("steamIDLobby", lobby, "member");
    second_member.set("iMember", Json::integer(1));
    check("and the second one is the game that joined",
          ask(host, "SteamAPI_ISteamMatchmaking_GetLobbyMemberByIndex", second_member)
                  .ret.as_uint64() == kGuestId);

    check("the host still owns it", ask(guest, "SteamAPI_ISteamMatchmaking_GetLobbyOwner",
                                        lobby_argument("steamIDLobby", lobby, "owner"))
                                            .ret.as_uint64() == kHostId);

    Json a_member = Json::object();
    a_member.set("steamIDFriend", Json::integer(static_cast<std::int64_t>(kGuestId)));
    check("a member can be asked for by name",
          ask(host, "SteamAPI_ISteamFriends_GetFriendPersonaName", a_member).ret.as_string() ==
              "Guest");

    Json member_name = lobby_argument("steamIDLobby", lobby, "member name");
    member_name.set("steamIDUser", Json::integer(static_cast<std::int64_t>(kGuestId)));
    member_name.set("pchKey", Json::string("name"));
    check("and a roster row has a name to draw",
          ask(host, "SteamAPI_ISteamMatchmaking_GetLobbyMemberData", member_name).ret.as_string() ==
              "Guest");

    // A room the run has never seen is not one it will invent: the world has no
    // opinion, which leaves the scenario and the session their say about it.
    Answer unheard;
    told.clear();
    check("a room nobody made is declined",
          !world.answer(guest, "SteamAPI_ISteamMatchmaking_JoinLobby",
                        lobby_argument("steamIDLobby", 424242, "stranger"), unheard, told));
    check("and nothing is told about it", told.empty());
    check("the world handles the lobby surface", LobbyWorld::handled_calls().size() >= 18);
}

void test_surface_matches_the_idl() {
    std::printf("[:] the generated surface\n");

    std::size_t count = 0;
    const steammock::SurfaceCall* calls = steammock::api_surface_calls(count);
    std::set<std::string> names;
    for (std::size_t index = 0; index < count; ++index) {
        names.insert(calls[index].name);
    }
    check("the surface has calls", count > 0u);
    check("it names the surface", std::string(steammock::api_surface_name()) == "seed");
    check("it is the revision the IDL says", steammock::api_surface_revision() == 2);
    check("a policy call is listed", names.count("SteamAPI_Init") == 1u);
    check("an out-parameter call is listed",
          names.count("SteamAPI_ISteamUserStats_GetStatInt32") == 1u);
    // Windows resolves a game's whole import table before it runs, so a name a
    // real game imports and the stub does not export is a game that will not
    // start. Both of these are in Spacewar's own executable.
    check("a GameServer call is listed", names.count("SteamGameServer_GetHSteamPipe") == 1u);
    check("an internal helper is listed", names.count("SteamInternal_CreateInterface") == 1u);

    // Every call the state machine answers has to be in the surface, or the stub
    // and the backend have drifted apart.
    std::vector<std::string> missing;
    for (const std::string& handled : steammock::state_handled_calls()) {
        if (names.count(handled) == 0u) {
            missing.push_back(handled);
        }
    }
    for (const std::string& name : missing) {
        std::printf("        the state machine answers %s, which the IDL does not export\n",
                    name.c_str());
    }
    check("every call the state machine answers is exported", missing.empty());

    // And the surface has to describe the parameters --list-api prints.
    for (std::size_t index = 0; index < count; ++index) {
        if (std::string(calls[index].name) != "SteamAPI_ISteamUserStats_GetStatInt32") {
            continue;
        }
        check("the out parameter is marked as one",
              calls[index].param_count == 3u && calls[index].params[2].out);
        check("the out parameter keeps its name",
              std::string(calls[index].params[2].name) == "pData");
    }
}

void test_numbers() {
    std::printf("[:] numbers in a transcript\n");

    // Readability: a transcript is meant to be read by a person.
    check("a fraction survives", steammock::Json::real(0.5).dump() == "0.5");
    check("a duration reads as a duration", steammock::Json::real(0.164).dump() == "0.164");
    check("a whole second stays plain", steammock::Json::real(1500.0).dump() == "1500");
    check("zero is zero", steammock::Json::real(0.0).dump() == "0");
    check("a whole number is written without a decimal point",
          steammock::Json::real(-12.0).dump() == "-12");

    // Exactness: the writer only shortens when the text still denotes the very
    // same double, which is what the C library reading it back proves.
    for (const double value :
         {0.164, 1500.0, 0.1, 1.0 / 3.0, -2.5, 3.141592653589793, 1e-300, 1e300}) {
        const std::string text = steammock::Json::real(value).dump();
        if (std::strtod(text.c_str(), nullptr) != value) {
            std::printf("        %s is not the same double as the value written\n", text.c_str());
        }
        check("a written double is exactly what a reader parses back",
              std::strtod(text.c_str(), nullptr) == value);
    }

    // The bridge's own parser is a hand-rolled one that keeps integers exact and
    // accumulates fractions digit by digit, so a value that is exact in binary
    // comes back unchanged and anything else lands within a rounding step. That
    // limit is older than this work, and it is the same reason a float parameter
    // is documented as travelling through a double.
    Json back;
    for (const double value : {0.5, -2.5, 1500.0, 0.0}) {
        check("a binary-exact double is read back unchanged",
              Json::parse(steammock::Json::real(value).dump(), back) && back.as_double() == value);
    }
    check("a fraction is read back to within a rounding step",
          Json::parse(steammock::Json::real(0.164).dump(), back) &&
              std::fabs(back.as_double() - 0.164) < 1e-15);
}

}  // namespace

int main() {
    std::printf("[+] SteamMock backend tests\n\n");
    test_replies();
    test_relabelling();
    test_identity();
    test_stats();
    test_achievements();
    test_describe();
    test_scenarios();
    test_profiles_are_per_session();
    test_the_lobbies_a_run_holds();
    test_surface_matches_the_idl();
    test_numbers();

    if (g_failures == 0) {
        std::printf("\n[+] all checks passed\n");
    } else {
        std::printf("\n[-] %d check(s) FAILED\n", g_failures);
    }
    return g_failures == 0 ? 0 : 1;
}
