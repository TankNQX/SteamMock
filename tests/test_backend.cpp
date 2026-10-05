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
#include <exception>
#include <optional>
#include <set>
#include <string>
#include <variant>
#include <vector>

#include "bridge/call.hpp"
#include "bridge/frame.hpp"
#include "bridge/inventory.hpp"
#include "bridge/json_read.hpp"
#include "bridge/leaderboard.hpp"
#include "bridge/lobby.hpp"
#include "bridge/protocol.hpp"
#include "bridge/scenario.hpp"
#include "bridge/session.hpp"
#include "bridge/surface.hpp"
#include "bridge/synth.hpp"

#ifndef STEAMMOCK_SCENARIO_PATH
#error "STEAMMOCK_SCENARIO_PATH must name the example scenario"
#endif

namespace
{

using steammock::Answer;
using steammock::Dispatcher;
using steammock::InventoryWorld;
using steammock::Json;
using steammock::LeaderboardWorld;
using steammock::LobbyWorld;
using steammock::Profile;
using steammock::Session;

int g_failures = 0;

void check(const char* what, bool ok)
{
    std::printf("  [%s] %s\n", ok ? "ok  " : "FAIL", what);
    if (!ok)
    {
        ++g_failures;
    }
}

std::string text_of(const Json& value, const char* key)
{
    const Json* member = steammock::json_member(value, key);
    return member != nullptr && member->is_string() ? steammock::as_string(*member) : std::string();
}

std::int64_t int_of(const Json& value, const char* key)
{
    const Json* member = steammock::json_member(value, key);
    return member != nullptr ? steammock::as_int64(*member) : 0;
}

bool out_flag(const Answer& answer, const char* key)
{
    const Json* value = steammock::json_member(answer.out, key);
    return value != nullptr && steammock::as_bool(*value);
}

std::int64_t out_int(const Answer& answer, const char* key)
{
    const Json* value = steammock::json_member(answer.out, key);
    return value != nullptr ? steammock::as_int64(*value) : 0;
}

double out_real(const Answer& answer, const char* key)
{
    const Json* value = steammock::json_member(answer.out, key);
    return value != nullptr ? steammock::as_double(*value) : 0.0;
}

bool has_out(const Answer& answer) { return steammock::carries_out(answer.out); }

// The app id a scenario served a handshake, or a number no profile has when it served
// none. A check should be able to say which identity a game resolved to without the
// test dying of `bad_optional_access` when the answer is "none" - which, one line
// down, is often exactly what is being checked.
std::int64_t served_app_id(const Dispatcher& dispatcher, const Json& hello)
{
    const std::optional<Profile> profile = dispatcher.profile_for(hello);
    return profile.has_value() ? profile->app_id : -1;
}

// The same fixture the Python tests used, so the ported expectations still mean
// something: one game, one known stat, one locked achievement.
Profile make_profile(const char* scripted = "{}")
{
    const std::string text =
        std::string("{\"app_id\":480,\"steam_id\":76561198000000001,\"persona_name\":\"Tester\",") +
        "\"language\":\"english\",\"stats\":{\"Deaths\":3}," +
        "\"achievements\":[{\"name\":\"ACH_BOOTED\",\"achieved\":false,"
        "\"display_name\":\"Booted\",\"display_description\":\"Start the game\"}],"
        "\"scripted\":" +
        scripted + "}";
    Json data;
    if (!steammock::parse(text, data))
    {
        check("the test fixture parses", false);
        return Profile{};
    }
    return Profile::from_json("test", data);
}

Session make_session(const char* scripted = "{}")
{
    Json hello = Json::object();
    hello["exe"] = Json("game.exe");
    hello["pid"] = Json(1234);
    return Session("session0", hello, make_profile(scripted));
}

Json name_argument(const char* name)
{
    Json args = Json::object();
    args["pchName"] = Json(name);
    return args;
}

// ---------------------------------------------------------------------------

void test_replies()
{
    std::printf("[:] replies\n");

    const Json unanswered = steammock::make_reply(7, false, Json(), Json());
    check("an unanswered reply says default", text_of(unanswered, "answer") == "default");
    check("an unanswered reply carries no return value",
          steammock::json_member(unanswered, "ret") == nullptr);
    check("an unanswered reply carries no out parameters",
          steammock::json_member(unanswered, "out") == nullptr);
    check("a reply echoes the sequence", int_of(unanswered, "seq") == 7);
    check("a reply carries the protocol version",
          int_of(unanswered, "v") == steammock::kProtocolVersion);
    check("ping: the frame cap is still the one the stub mirrors",
          steammock::kMaxFrameBytes == 4u * 1024u * 1024u);

    // A reply is the one place a value arrives from a program this does not
    // control, so the readers that turn one into a number have to survive what it
    // sends: a double larger than the target can hold, a NaN, or a negative where
    // the wire says unsigned. Each of those used to be a cast with no defined
    // result, which is the kind of thing that works until the day it does not.
    check("a double too large for int64 reads as the fallback",
          steammock::as_int64(Json(1e308), 7) == 7);
    check("a NaN reads as the fallback", steammock::as_int64(Json(std::nan("")), 7) == 7);
    check("an unsigned read refuses a negative double", steammock::as_uint64(Json(-1.5), 7u) == 7u);
    check("a double inside the range still reads", steammock::as_int64(Json(1.5)) == 1);

    // A `uint64` with its top bit set is a number, not a negative one. The flat path
    // knows the type at the call site and the packed path carries the type with the
    // value; both used to spell `2^64-1` as `-1`, which the reply readers then
    // refused. This is the check that fails if either goes back to a cast.
    const std::uint64_t top_bit = 0xFFFFFFFFFFFFFFFFull;
    check("an unsigned argument is written as an unsigned number",
          steammock::arg_uint(top_bit).is_number_unsigned());
    check("an unsigned argument reads back as what it was",
          steammock::as_uint64(steammock::arg_uint(top_bit)) == top_bit);
    check("a packed unsigned argument keeps its type",
          std::holds_alternative<std::uint64_t>(steammock::wire_uint(top_bit)));
    check("and a packed signed one keeps its own sign",
          std::holds_alternative<std::int64_t>(steammock::wire_int(-1)));

    Json out = Json::object();
    out["pData"] = Json(42);
    const Json answered = steammock::make_reply(8, true, Json(true), out);
    check("an answered reply says handled", text_of(answered, "answer") == "handled");
    check("an answered reply carries the value",
          steammock::json_member(answered, "ret") != nullptr &&
              steammock::as_bool(*steammock::json_member(answered, "ret")));
    check("an answered reply carries out parameters",
          steammock::json_member(answered, "out") != nullptr &&
              steammock::json_member(*steammock::json_member(answered, "out"), "pData") !=
                  nullptr &&
              steammock::as_int64(*steammock::json_member(*steammock::json_member(answered, "out"),
                                                          "pData")) == 42);

    check("an answer without out parameters omits out",
          steammock::json_member(steammock::make_reply(9, true, Json(3), Json()), "out") ==
              nullptr);
    check("an empty out object is not carried",
          steammock::json_member(steammock::make_reply(10, true, Json(3), Json::object()), "out") ==
              nullptr);

    // A scenario that scripts a call without saying what it returns means "the
    // zero of your type", not "no opinion" - and that distinction is the whole
    // point of the protocol.
    const Json bare = steammock::make_reply(11, true, Json(), Json());
    check("an answered reply with no value still has ret",
          steammock::json_member(bare, "ret") != nullptr);
    check("and that ret is null", steammock::json_member(bare, "ret")->is_null());
}

void test_relabelling()
{
    std::printf("[:] where an answer came from\n");

    const Dispatcher dispatcher;
    Session scripted_session = make_session("{\"SteamAPI_ISteamUtils_GetAppID\":{\"ret\":999}}");
    const Answer win =
        dispatcher.answer(scripted_session, "SteamAPI_ISteamUtils_GetAppID", Json::object());
    check("a scripted call wins over the state machine", win.answered);
    check("the scripted value is the one used", steammock::as_int64(win.ret) == 999);
    check("it is labelled scripted", win.via == "scripted");

    Session declining = make_session("{\"SteamAPI_Init\":{\"answer\":\"default\"}}");
    const Answer declined = dispatcher.answer(declining, "SteamAPI_Init", Json::object());
    check("a scripted call can decline to answer", !declined.answered);
    check("a declined scripted call is still labelled scripted", declined.via == "scripted");

    Session state_session = make_session();
    const Answer from_state =
        dispatcher.answer(state_session, "SteamAPI_GetHSteamUser", Json::object());
    check("state answers are labelled state", from_state.answered && from_state.via == "state");
    check("the state answer is the documented constant", steammock::as_int64(from_state.ret) == 1);

    Session none_session = make_session();
    const Answer unanswered = dispatcher.answer(none_session, "SteamAPI_Shutdown", Json::object());
    check("an unanswered call is labelled none", !unanswered.answered && unanswered.via == "none");
}

void test_identity()
{
    std::printf("[:] identity comes from the profile\n");

    Session session = make_session();
    check("the steam id is the profile's",
          steammock::as_uint64(
              session.handle("SteamAPI_ISteamUser_GetSteamID", Json::object()).ret) ==
              76561198000000001ull);
    check("the persona name is the profile's",
          steammock::as_string(
              session.handle("SteamAPI_ISteamFriends_GetPersonaName", Json::object()).ret) ==
              "Tester");
    check("the app id is the profile's",
          steammock::as_int64(
              session.handle("SteamAPI_ISteamUtils_GetAppID", Json::object()).ret) == 480);
    check("the language is the profile's",
          steammock::as_string(
              session.handle("SteamAPI_ISteamApps_GetCurrentGameLanguage", Json::object()).ret) ==
              "english");
    check("the install path is reported",
          !steammock::as_string(session.handle("SteamAPI_GetSteamInstallPath", Json::object()).ret)
               .empty());
    check("the build id is reported",
          steammock::as_int64(
              session.handle("SteamAPI_ISteamApps_GetAppBuildId", Json::object()).ret) == 1);

    const Answer unknown = session.handle("SteamAPI_Init", Json::object());
    check("a call nobody knows has no opinion", !unknown.answered);
    check("an unanswered state call carries nothing",
          unknown.ret.is_null() && unknown.out.is_null());
}

// The friends list is the scenario's, and what a game gets is a list of ids and names: a friend
// named as a profile resolves to that profile's identity, and one spelled out is somebody this
// run simply does not have. An invite needs the first kind, because an id is what it is sent to.
void test_friends()
{
    std::printf("[:] friends\n");

    Json document;
    check("the friends scenario parses",
          steammock::parse("{\"profiles\":{"
                           "\"default\":{\"steam_id\":11,\"persona_name\":\"One\","
                           "\"friends\":[\"other\",{\"steam_id\":99,\"persona_name\":\"Away\"}]},"
                           "\"other\":{\"steam_id\":22,\"persona_name\":\"Two\","
                           "\"friends\":[\"default\"]}}}",
                           document));
    const Dispatcher dispatcher(document);

    Json hello = Json::object();
    hello["profile"] = Json("default");
    const std::optional<Profile> profile = dispatcher.profile_for(hello);
    check("the profile is served", profile.has_value());
    if (!profile.has_value())
    {
        return;
    }

    check("a friend named by profile resolves to that profile",
          profile->friends.size() == 2 && profile->friends[0].steam_id == 22 &&
              profile->friends[0].persona_name == "Two" && profile->friends[0].profile == "other");
    check("a friend spelled out stays as written", profile->friends[1].steam_id == 99 &&
                                                       profile->friends[1].persona_name == "Away" &&
                                                       profile->friends[1].profile.empty());

    Session session("friends", hello, *profile);
    const Answer count =
        dispatcher.answer(session, "SteamAPI_ISteamFriends_GetFriendCount", Json::object());
    check("the count is the list's", count.ret == Json(2) && count.via == "state");

    Json index = Json::object();
    index["iFriend"] = Json(1);
    const Answer by_index =
        dispatcher.answer(session, "SteamAPI_ISteamFriends_GetFriendByIndex", index);
    check("an index answers with the friend's id", by_index.ret == Json(99));

    Json past_end = Json::object();
    past_end["iFriend"] = Json(7);
    const Answer past =
        dispatcher.answer(session, "SteamAPI_ISteamFriends_GetFriendByIndex", past_end);
    check("past the end answers no id rather than an invented one", past.ret == Json(0));

    Json by_id = Json::object();
    by_id["steamIDFriend"] = Json(22);
    const Answer named =
        dispatcher.answer(session, "SteamAPI_ISteamFriends_GetFriendPersonaName", by_id);
    check("a friend is named by id", named.ret == Json("Two"));

    Json stranger = Json::object();
    stranger["steamIDFriend"] = Json(1234);
    const Answer unnamed =
        dispatcher.answer(session, "SteamAPI_ISteamFriends_GetFriendPersonaName", stranger);
    check("somebody who is not a friend has no name", unnamed.ret == Json(std::string()));

    // A friends list naming a profile the scenario does not have is a file that cannot mean what
    // it says. Nothing is served from it, the blank default identity included: this is the same
    // refusal a rule or a `default_profile` naming a profile it does not have gets, which used to
    // be a silence and took a day to find.
    Json typo_document;
    check("the typo scenario parses",
          steammock::parse("{\"profiles\":{\"default\":{\"friends\":[\"missing\"]}}}",
                           typo_document));
    const Dispatcher typo(typo_document);
    check("a friend who is not a profile is reported",
          typo.load_error().find("missing") != std::string::npos);
    check("and a scenario that cannot mean it serves nobody",
          !typo.has_profile("default") && !typo.profile_for(hello).has_value());
}

// An invite goes from a member of a room to a friend who is not in it, and the friend is told.
// The payload names the room and the person who asked, and the three ways of asking for an invite
// that goes nowhere are answered false rather than half-done.
void test_invites()
{
    std::printf("[:] invites\n");

    auto profile_with_friend = [](const char* persona, std::uint64_t steam_id,
                                  std::uint64_t friend_id)
    {
        const std::string text = std::string("{\"app_id\":480,\"steam_id\":") +
                                 std::to_string(steam_id) + ",\"persona_name\":\"" + persona +
                                 "\",\"friends\":[{\"steam_id\":" + std::to_string(friend_id) +
                                 ",\"persona_name\":\"Friend\"}]}";
        Json data;
        check("the invite fixture parses", steammock::parse(text, data));
        return Profile::from_json(persona, data);
    };

    constexpr std::uint64_t kHostId = 76561198000000001ull;
    constexpr std::uint64_t kGuestId = 76561198000000002ull;
    constexpr std::uint64_t kStrangerId = 76561198000000003ull;

    Json hello = Json::object();
    hello["exe"] = Json("game.exe");
    hello["pid"] = Json(1234);
    Session host("host", hello, profile_with_friend("Host", kHostId, kGuestId));
    Session guest("guest", hello, profile_with_friend("Guest", kGuestId, kHostId));

    LobbyWorld world;
    std::vector<std::pair<std::uint64_t, Json>> told;
    auto ask = [&](Session& who, const char* call, const Json& args)
    {
        Answer answer;
        told.clear();
        check((std::string("the world answers ") + call).c_str(),
              world.answer(who, call, args, answer, told));
        return answer;
    };
    auto invitation = [](std::uint64_t room, std::uint64_t invitee)
    {
        Json args = Json::object();
        args["steamIDLobby"] = Json(static_cast<std::int64_t>(room));
        args["steamIDInvitee"] = Json(static_cast<std::int64_t>(invitee));
        return args;
    };

    Json create = Json::object();
    create["eLobbyType"] = Json(2);
    create["cMaxMembers"] = Json(4);
    ask(host, "SteamAPI_ISteamMatchmaking_CreateLobby", create);
    Json first_lobby = Json::object();
    first_lobby["iLobby"] = Json(0);
    const std::uint64_t room = steammock::as_uint64(
        ask(host, "SteamAPI_ISteamMatchmaking_GetLobbyByIndex", first_lobby).ret);
    check("the host has a room to invite into", room != 0);

    // A friend and only a friend hears about it. Spacewar never calls this, because its own
    // invite path is the overlay dialog, so what is tested here is the API's own shape.
    const Answer invited =
        ask(host, "SteamAPI_ISteamMatchmaking_InviteUserToLobby", invitation(room, kGuestId));
    check("a friend is invited and the call says so", steammock::as_bool(invited.ret));
    check("and the friend is the one told", told.size() == 1u && told[0].first == kGuestId);
    check("the payload is a LobbyInvite_t",
          !told.empty() && steammock::as_string(*steammock::json_member(told[0].second, "event")) ==
                               "LobbyInvite_t");
    const Json* fields = told.empty() ? nullptr : steammock::json_member(told[0].second, "in");
    check("naming the room and the person who asked",
          fields != nullptr &&
              steammock::as_uint64(*steammock::json_member(*fields, "m_ulSteamIDLobby")) == room &&
              steammock::as_uint64(*steammock::json_member(*fields, "m_ulSteamIDUser")) == kHostId);

    // Nobody else hears about it: a game standing nowhere has no room to invite into and nothing
    // to attribute the invite to, and a game has no claim on somebody it is not friends with.
    const Answer standing_nowhere =
        ask(guest, "SteamAPI_ISteamMatchmaking_InviteUserToLobby", invitation(room, kHostId));
    check("a game that is not in the room cannot invite to it",
          !steammock::as_bool(standing_nowhere.ret) && told.empty());

    const Answer stranger =
        ask(host, "SteamAPI_ISteamMatchmaking_InviteUserToLobby", invitation(room, kStrangerId));
    check("somebody who is not a friend cannot be invited",
          !steammock::as_bool(stranger.ret) && told.empty());

    // The route a game actually takes, and the one Spacewar's Invite Friend item uses: the room and
    // no target at all, because the overlay is what picks the friend. This stands in for the
    // overlay, and then for the click that accepts, because there is no UI here to click.
    Json dialog = Json::object();
    dialog["steamIDLobby"] = Json(static_cast<std::int64_t>(room));
    const Answer opened =
        ask(host, "SteamAPI_ISteamFriends_ActivateGameOverlayInviteDialog", dialog);
    check("the overlay invites the friend the game has", opened.answered && told.size() == 2u);
    check("both go to that friend",
          told.size() == 2u && told[0].first == kGuestId && told[1].first == kGuestId);
    check("the invite comes first",
          !told.empty() && steammock::as_string(*steammock::json_member(told[0].second, "event")) ==
                               "LobbyInvite_t");
    check("then the join request, which is the click that accepts",
          told.size() > 1u && steammock::as_string(*steammock::json_member(
                                  told[1].second, "event")) == "GameRichPresenceJoinRequested_t");
    const Json* request = told.size() > 1u ? steammock::json_member(told[1].second, "in") : nullptr;
    check("and it carries the command line the game's own parser reads",
          request != nullptr &&
              steammock::as_string(*steammock::json_member(*request, "m_rgchConnect")) ==
                  std::string("+connect_lobby ") + std::to_string(room));

    Json join = Json::object();
    join["steamIDLobby"] = Json(static_cast<std::int64_t>(room));
    ask(guest, "SteamAPI_ISteamMatchmaking_JoinLobby", join);
    const Answer already_in =
        ask(host, "SteamAPI_ISteamMatchmaking_InviteUserToLobby", invitation(room, kGuestId));
    check("and a member is not invited to the room it is standing in",
          !steammock::as_bool(already_in.ret) && told.empty());

    // An overlay opened on a friend already standing in the room tells nobody anything.
    ask(host, "SteamAPI_ISteamFriends_ActivateGameOverlayInviteDialog", dialog);
    check("nor is a friend already in the room invited again by the overlay", told.empty());
}

void test_stats()
{
    std::printf("[:] stats\n");

    Session session = make_session();
    const Answer read = session.handle("SteamAPI_ISteamUserStats_GetStat", name_argument("Deaths"));
    check("a known stat is answered", read.answered && steammock::as_bool(read.ret));
    check("a stat is read as named out parameters", out_int(read, "pData") == 3);

    Json write = name_argument("Deaths");
    write["nData"] = Json(9);
    check("writing a stat is accepted",
          steammock::as_bool(session.handle("SteamAPI_ISteamUserStats_SetStat", write).ret));

    const Answer reread =
        session.handle("SteamAPI_ISteamUserStats_GetStat", name_argument("Deaths"));
    check("reading it back sees the new value", out_int(reread, "pData") == 9);
    check("the write is remembered for a transcript",
          session.stats_written().size() == 1u && session.stats_written()[0].second == 9);

    // A game may invent a stat locally without the scenario listing it first.
    Json invented = name_argument("Invented");
    invented["nData"] = Json(1);
    session.handle("SteamAPI_ISteamUserStats_SetStat", invented);
    check("a stat a game invents can be read back",
          steammock::as_bool(session.handle("SteamAPI_ISteamUserStats_GetStat", invented).ret));

    const Answer missing =
        session.handle("SteamAPI_ISteamUserStats_GetStat", name_argument("NoSuchStat"));
    check("an unknown stat is still answered", missing.answered);
    check("an unknown stat fails, like Steam", !steammock::as_bool(missing.ret));
    check("an unknown stat leaves the caller's variable alone", !has_out(missing));

    // The float overloads are their own flat names, and the one that writes sends `fData`
    // where the integer slot sends `nData`. A stat in this harness is an integer, so the
    // float write lands as the whole part of the number.
    Json float_write = name_argument("Deaths");
    float_write["fData"] = Json(9.5);
    check("a float write is accepted",
          steammock::as_bool(
              session.handle("SteamAPI_ISteamUserStats_SetStat0", float_write).ret));

    const Answer float_read =
        session.handle("SteamAPI_ISteamUserStats_GetStat0", name_argument("Deaths"));
    check("the float overload reads the same stat as the integer one",
          steammock::as_bool(float_read.ret) && out_real(float_read, "pData") == 9.0);
    check("and the integer overload reads what the float one wrote",
          out_int(session.handle("SteamAPI_ISteamUserStats_GetStat", name_argument("Deaths")),
                  "pData") == 9);

    // An average rate is a count the game reports with the session it took, and what it reads
    // back afterwards is the rate for that session.
    Json rate = name_argument("AverageSpeed");
    rate["flCountThisSession"] = Json(30.0);
    rate["dSessionLength"] = Json(10.0);
    check("an average rate is accepted",
          steammock::as_bool(
              session.handle("SteamAPI_ISteamUserStats_UpdateAvgRateStat", rate).ret));
    check("and the game reads back the rate it reported",
          out_real(session.handle("SteamAPI_ISteamUserStats_GetStat0",
                                  name_argument("AverageSpeed")),
                   "pData") == 3.0);

    Json no_session = name_argument("AverageSpeed");
    no_session["flCountThisSession"] = Json(30.0);
    no_session["dSessionLength"] = Json(0.0);
    check("a rate with no session to divide by is refused",
          !steammock::as_bool(
              session.handle("SteamAPI_ISteamUserStats_UpdateAvgRateStat", no_session).ret));
    check("and that leaves the stat where it was",
          out_real(session.handle("SteamAPI_ISteamUserStats_GetStat0",
                                  name_argument("AverageSpeed")),
                   "pData") == 3.0);

    // And the one answer here that is a payload rather than a value: a game asks for its stats
    // and Steam tells it they have arrived. Spacewar's stats screen draws nothing at all -
    // including the inventory that is drawn on it - until one of these has been handed to it.
    {
        const Answer asked =
            session.handle("SteamAPI_ISteamUserStats_RequestCurrentStats", Json::object());
        check("asking for the current stats is answered",
              asked.answered && steammock::as_bool(asked.ret));
        check("  and it says they have arrived, in the payload the SDK declares",
              asked.events.is_array() && asked.events.size() == 1u &&
                  text_of(asked.events[0], "event") == "UserStatsReceived_t");
        const steammock::Json* in = asked.events.is_array() && !asked.events.empty()
                                        ? steammock::json_member(asked.events[0], "in")
                                        : nullptr;
        check("  and the payload is about this session: the app and the player",
              in != nullptr && int_of(*in, "m_nGameID") == 480 &&
                  steammock::as_uint64(*steammock::json_member(*in, "m_steamIDUser")) != 0u);
        check("  and it says the result was OK", in != nullptr && int_of(*in, "m_eResult") == 1);
    }
}

void test_achievements()
{
    std::printf("[:] achievements\n");

    Session session = make_session();
    const Answer locked =
        session.handle("SteamAPI_ISteamUserStats_GetAchievement", name_argument("ACH_BOOTED"));
    check("an achievement can be read", locked.answered && steammock::as_bool(locked.ret));
    check("it starts locked", !out_flag(locked, "pbAchieved"));

    check("unlocking it is accepted",
          steammock::as_bool(
              session.handle("SteamAPI_ISteamUserStats_SetAchievement", name_argument("ACH_BOOTED"))
                  .ret));
    const Answer unlocked =
        session.handle("SteamAPI_ISteamUserStats_GetAchievement", name_argument("ACH_BOOTED"));
    check("it reads back as unlocked", out_flag(unlocked, "pbAchieved"));
    check("the unlock is remembered for a transcript",
          session.achievements_set().size() == 1u && session.achievements_set()[0] == "ACH_BOOTED");

    const Answer missing =
        session.handle("SteamAPI_ISteamUserStats_GetAchievement", name_argument("ACH_MISSING"));
    check("an unknown achievement reports failure",
          missing.answered && !steammock::as_bool(missing.ret));
    check(
        "unlocking an unknown achievement reports failure",
        !steammock::as_bool(
            session.handle("SteamAPI_ISteamUserStats_SetAchievement", name_argument("ACH_MISSING"))
                .ret));

    check("the list is counted",
          steammock::as_int64(
              session.handle("SteamAPI_ISteamUserStats_GetNumAchievements", Json::object()).ret) ==
              1);
    Json first = Json::object();
    first["iAchievement"] = Json(0);
    check("achievement names are indexed",
          steammock::as_string(
              session.handle("SteamAPI_ISteamUserStats_GetAchievementName", first).ret) ==
              "ACH_BOOTED");
    Json past_the_end = Json::object();
    past_the_end["iAchievement"] = Json(9);
    check("an index past the end is empty text",
          steammock::as_string(
              session.handle("SteamAPI_ISteamUserStats_GetAchievementName", past_the_end).ret)
              .empty());

    // The text a game draws the achievement with: Steam hands it out per key, and the two keys
    // every game asks for are the pair the scenario writes.
    Json attribute = name_argument("ACH_BOOTED");
    attribute["pchKey"] = Json("name");
    check("an achievement draws the name the scenario gave it",
          steammock::as_string(
              session.handle("SteamAPI_ISteamUserStats_GetAchievementDisplayAttribute", attribute)
                  .ret) == "Booted");
    attribute["pchKey"] = Json("desc");
    check("and its description",
          steammock::as_string(
              session.handle("SteamAPI_ISteamUserStats_GetAchievementDisplayAttribute", attribute)
                  .ret) == "Start the game");

    attribute["pchKey"] = Json("hidden");
    check("a key no scenario describes is empty text",
          steammock::as_string(
              session.handle("SteamAPI_ISteamUserStats_GetAchievementDisplayAttribute", attribute)
                  .ret)
              .empty());
    Json unknown_attribute = name_argument("ACH_MISSING");
    unknown_attribute["pchKey"] = Json("name");
    check("and so is an achievement the profile does not have",
          steammock::as_string(session.handle(
                                          "SteamAPI_ISteamUserStats_GetAchievementDisplayAttribute",
                                          unknown_attribute)
                                   .ret)
              .empty());
}

void test_describe()
{
    std::printf("[:] a session can describe itself\n");

    const Session session = make_session();
    const std::string text = session.describe();
    check("the description names the session", text.find("session0") != std::string::npos);
    check("the description names the executable", text.find("game.exe") != std::string::npos);
    check("the description names the profile", text.find("test") != std::string::npos);
    check("the process id is read from the hello", session.pid() == 1234);
}

void test_scenarios()
{
    std::printf("[:] scenarios and match rules\n");

    Json document;
    const std::string text = "{\"profiles\":{\"default\":{\"app_id\":1},\"other\":{\"app_id\":2}},"
                             "\"match\":[{\"exe_contains\":\"special\",\"profile\":\"other\"},"
                             "{\"exe_contains\":\"game\",\"profile\":\"default\"}],"
                             "\"default_profile\":\"default\"}";
    check("the scenario parses", steammock::parse(text, document));
    const Dispatcher dispatcher(document);

    Json special = Json::object();
    special["exe"] = Json("my_special_game.exe");
    check("a match rule can pick a profile by executable", served_app_id(dispatcher, special) == 2);

    Json plain = Json::object();
    plain["exe"] = Json("game.exe");
    check("the first matching rule wins", served_app_id(dispatcher, plain) == 1);

    // A name asked for by name is not a hint. Substituting the default is how three
    // clients came to run as two players while every run still looked plausible, so a
    // scenario without the name is refused and the caller says so.
    Json by_name = Json::object();
    by_name["profile"] = Json("other");
    check("a profile asked for by name is served before any match rule",
          served_app_id(dispatcher, by_name) == 2);

    Json missing = Json::object();
    missing["profile"] = Json("no_such_profile");
    check("a profile asked for by name and not there is refused, not replaced",
          !dispatcher.profile_for(missing).has_value());

    std::string named;
    check("and the name that could not be served is reported",
          !dispatcher.profile_for(missing, &named).has_value() && named == "no_such_profile");

    Json unknown = Json::object();
    unknown["exe"] = Json("unknown.exe");
    check("an unmatched game gets the default", served_app_id(dispatcher, unknown) == 1);

    Json typo_document;
    check("the typo scenario parses",
          steammock::parse("{\"profiles\":{\"default\":{\"app_id\":1}},"
                           "\"match\":[{\"exe_contains\":\"game\",\"profile\":\"typo\"}]}",
                           typo_document));
    const Dispatcher typo(typo_document);
    Json game = Json::object();
    game["exe"] = Json("game.exe");
    // The same silence as a name asked for by name, one step further along: a rule that
    // matched the game and names a profile the scenario does not have used to fall through
    // to the default, which runs a player nobody asked for.
    std::string rule_named_it;
    check("a rule naming a profile the scenario does not have is refused, not replaced",
          !typo.profile_for(game, &rule_named_it).has_value() && rule_named_it == "typo");

    // The third way in, and the last one that was still quiet: a scenario whose own
    // `default_profile` names a profile it does not have. Every game that fell through to
    // the default used to run as a default-constructed Profile - app id 0, persona
    // "DebugPlayer", a blank identity that looks like a working run.
    Json bad_default;
    check("the bad-default scenario parses",
          steammock::parse("{\"profiles\":{\"other\":{\"app_id\":2}},"
                           "\"default_profile\":\"other_typo\"}",
                           bad_default));
    const Dispatcher stranded(bad_default);
    std::string default_named_it;
    check("a default profile the scenario does not have is refused, not replaced",
          !stranded.profile_for(game, &default_named_it).has_value() &&
              default_named_it == "other_typo");

    // A rule that names no profile at all is not that: saying nothing is a request for the
    // default, and only a name the scenario lacks is a mistake.
    Json ruleless_document;
    check("the ruleless scenario parses",
          steammock::parse("{\"profiles\":{\"default\":{\"app_id\":1}},"
                           "\"match\":[{\"exe_contains\":\"game\"}]}",
                           ruleless_document));
    const Dispatcher ruleless(ruleless_document);
    check("a rule that names no profile takes the default", served_app_id(ruleless, game) == 1);

    const Dispatcher empty{Json::object()};
    check("an empty scenario still has a default profile", empty.has_profile("default"));
    check("the default profile is named", empty.default_profile() == "default");

    Dispatcher loaded;
    std::string error;
    const bool ok = Dispatcher::load_file(STEAMMOCK_SCENARIO_PATH, loaded, error);
    check("the bundled example scenario loads", ok);
    if (!ok)
    {
        std::printf("        %s\n", error.c_str());
        return;
    }
    Json fake_game = Json::object();
    fake_game["exe"] = Json("fake_game.exe");
    const std::optional<Profile> served = loaded.profile_for(fake_game);
    check("the example gives fake_game the default profile",
          served.has_value() && served->app_id == 480);
    if (!served.has_value())
    {
        return; // every check below would only say the same thing again
    }
    const Profile& profile = *served;
    check("the example scripts SteamAPI_Init",
          profile.scripted_for("SteamAPI_Init") != nullptr &&
              steammock::as_bool(
                  *steammock::json_member(*profile.scripted_for("SteamAPI_Init"), "ret")));
    check("the example seeds a stat", [&profile]
          {
        std::int64_t deaths = -1;
        return profile.find_stat("Deaths", deaths) && deaths == 0; }());
    check("the example names both games", loaded.profile_names().size() == 2u);
    check("the example carries match rules", loaded.match_rules().size() == 2u);

    Dispatcher nowhere;
    std::string why;
    check("a scenario that is not there reports why",
          !Dispatcher::load_file("no/such/scenario.json", nowhere, why) && !why.empty());

    // The `overrides` block: the file stating what it is testing. It is read at startup like
    // everything else in the file, so an entry that cannot be answered is refused then rather
    // than discovered by a game at call time.
    Json with_overrides;
    check("a scenario with an overrides block parses",
          steammock::parse("{\"profiles\":{\"default\":{\"app_id\":1}},"
                           "\"overrides\":{\"SteamAPI_Init\":{\"ret\":false}}}",
                           with_overrides));
    const Dispatcher overriding(with_overrides);
    check("it loads", overriding.load_error().empty());
    const Json* entry = overriding.override_for("SteamAPI_Init");
    check("the block is read by call name", entry != nullptr);
    check("and carries what the file wrote",
          entry != nullptr && steammock::json_member(*entry, "ret") != nullptr &&
              !steammock::as_bool(*steammock::json_member(*entry, "ret")));
    check("a call the file passes over has no override",
          overriding.override_for("SteamAPI_Shutdown") == nullptr);

    // Neither half of a block that cannot be answered is skipped: an `overrides` that is not
    // an object, and an entry inside it that says nothing. Both are files that cannot mean
    // what they say, which is what this scenario reader refuses rather than half-serves.
    Json not_an_object;
    check("an overrides block that is not an object parses",
          steammock::parse("{\"overrides\":[]}", not_an_object));
    const Dispatcher refused_block(not_an_object);
    check("it is refused", refused_block.load_error().find("overrides") != std::string::npos);
    check("and nothing is served from it", !refused_block.has_profile("default"));

    Json empty_entry;
    check("an entry that says nothing parses",
          steammock::parse("{\"overrides\":{\"SteamAPI_Init\":{}}}", empty_entry));
    const Dispatcher refused_entry(empty_entry);
    check("it is refused too", refused_entry.load_error().find("SteamAPI_Init") != std::string::npos);
}

// Each session gets its own copy of a profile, so two games matched to the same
// scenario cannot see each other's stats. The README says several games run side
// by side "each with its own profile"; this is what makes that true.
void test_profiles_are_per_session()
{
    std::printf("[:] sessions do not share their profile\n");

    const Dispatcher dispatcher;
    Json game = Json::object();
    game["exe"] = Json("game.exe");
    const std::optional<Profile> served = dispatcher.profile_for(game);
    if (!served.has_value())
    {
        check("the scenario serves this game a profile", false);
        return;
    }
    const Profile& matched = *served;

    Session one("one", Json::object(), matched);
    Session two("two", Json::object(), matched);

    Json write = name_argument("Deaths");
    write["nData"] = Json(77);
    one.handle("SteamAPI_ISteamUserStats_SetStat", write);

    check("the game that wrote sees its own value",
          steammock::as_bool(
              one.handle("SteamAPI_ISteamUserStats_GetStat", name_argument("Deaths")).ret));
    check("the other game is not handed that value",
          !steammock::as_bool(
              two.handle("SteamAPI_ISteamUserStats_GetStat", name_argument("Deaths")).ret));
}

// The lobbies a run holds are the one piece of backend state that is not per game,
// so what is worth proving is what two games can see of each other through it: one
// makes a room, the other finds it, joins it, and reads back a roster that neither
// of them was told.
void test_the_lobbies_a_run_holds()
{
    std::printf("[:] the lobbies a run holds\n");

    auto profile_for = [](const char* persona, std::uint64_t steam_id)
    {
        const std::string text = std::string("{\"app_id\":480,\"steam_id\":") +
                                 std::to_string(steam_id) + ",\"persona_name\":\"" + persona +
                                 "\",\"language\":\"english\"}";
        Json data;
        check("the lobby fixture parses", steammock::parse(text, data));
        return Profile::from_json(persona, data);
    };
    constexpr std::uint64_t kHostId = 76561198000000001ull;
    constexpr std::uint64_t kGuestId = 76561198000000002ull;

    auto session_for = [](const char* id, const Profile& profile)
    {
        Json hello = Json::object();
        hello["exe"] = Json("game.exe");
        hello["pid"] = Json(1234);
        return Session(id, hello, profile);
    };

    Session host = session_for("host", profile_for("Host", kHostId));
    Session guest = session_for("guest", profile_for("Guest", kGuestId));

    LobbyWorld world;
    // Cleared by hand because the world appends: whoever a call tells is not whoever
    // the next call tells.
    std::vector<std::pair<std::uint64_t, Json>> told;

    auto ask = [&](Session& who, const char* call, const Json& args)
    {
        Answer answer;
        told.clear();
        const bool handled = world.answer(who, call, args, answer, told);
        check((std::string("the world answers ") + call).c_str(), handled);
        return answer;
    };
    // The field of the payload a call came with, which is how the world completes one.
    auto field_of = [](const Answer& answer, const char* field) -> const Json*
    {
        if (!answer.events.is_array() || answer.events.empty())
        {
            return nullptr;
        }
        const Json* in = steammock::json_member(answer.events.front(), "in");
        return in != nullptr ? steammock::json_member(*in, field) : nullptr;
    };
    auto lobby_argument = [](const char* key, std::uint64_t value, const char* /*call*/)
    {
        Json args = Json::object();
        args[key] = Json(static_cast<std::int64_t>(value));
        return args;
    };

    {
        const Answer listed =
            ask(host, "SteamAPI_ISteamMatchmaking_RequestLobbyList", Json::object());
        const Json* count = field_of(listed, "m_nLobbiesMatching");
        check("a run with no lobbies lists none",
              count != nullptr && steammock::as_int64(*count) == 0);
    }

    Json create = Json::object();
    create["eLobbyType"] = Json(2);
    create["cMaxMembers"] = Json(4);
    const Answer created = ask(host, "SteamAPI_ISteamMatchmaking_CreateLobby", create);
    const Json* made = field_of(created, "m_ulSteamIDLobby");
    check("creating a lobby hands one back", made != nullptr && steammock::as_uint64(*made) != 0);
    check("and the world says so", created.via == "lobby");

    const Json* call_of_payload = nullptr;
    if (created.events.is_array() && !created.events.empty())
    {
        call_of_payload = steammock::json_member(created.events.front(), "call");
    }
    check("the payload completes the call the answer returned",
          call_of_payload != nullptr && call_of_payload->is_number() &&
              steammock::as_uint64(*call_of_payload) == steammock::as_uint64(created.ret));

    const std::uint64_t lobby = made != nullptr ? steammock::as_uint64(*made) : 0;

    // The host names it, the way a game does, and the world keeps that.
    Json named = lobby_argument("steamIDLobby", lobby, "named");
    named["pchKey"] = Json("name");
    named["pchValue"] = Json("A lobby");
    ask(host, "SteamAPI_ISteamMatchmaking_SetLobbyData", named);

    // Now the other game can see it.
    const Answer listed = ask(guest, "SteamAPI_ISteamMatchmaking_RequestLobbyList", Json::object());
    const Json* count = field_of(listed, "m_nLobbiesMatching");
    check("the other game lists the one that exists",
          count != nullptr && steammock::as_int64(*count) == 1);

    Json index = Json::object();
    index["iLobby"] = Json(0);
    const Answer found = ask(guest, "SteamAPI_ISteamMatchmaking_GetLobbyByIndex", index);
    check("and it is the host's lobby rather than a lookalike",
          found.ret.is_number() && steammock::as_uint64(found.ret) == lobby);

    Json name_key = lobby_argument("steamIDLobby", lobby, "name key");
    name_key["pchKey"] = Json("name");
    check("with the name the host wrote",
          steammock::as_string(
              ask(guest, "SteamAPI_ISteamMatchmaking_GetLobbyData", name_key).ret) == "A lobby");

    // It joins, and the room is told - including the game that joined.
    const Answer entered = ask(guest, "SteamAPI_ISteamMatchmaking_JoinLobby",
                               lobby_argument("steamIDLobby", lobby, "join"));
    const Json* room_joined = field_of(entered, "m_ulSteamIDLobby");
    check("joining says which room it is in",
          room_joined != nullptr && steammock::as_uint64(*room_joined) == lobby);
    check("both members of the room are told", told.size() == 4);

    bool told_host = false;
    bool told_guest = false;
    for (const auto& note : told)
    {
        told_host = told_host || note.first == kHostId;
        told_guest = told_guest || note.first == kGuestId;
    }
    check("the host is told", told_host);
    check("and so is the game that just joined", told_guest);

    // What the two of them read now is the same room, seen from either side.
    check("the room has two members",
          steammock::as_int64(ask(host, "SteamAPI_ISteamMatchmaking_GetNumLobbyMembers",
                                  lobby_argument("steamIDLobby", lobby, "members"))
                                  .ret) == 2);

    Json second_member = lobby_argument("steamIDLobby", lobby, "member");
    second_member["iMember"] = Json(1);
    check("and the second one is the game that joined",
          steammock::as_uint64(
              ask(host, "SteamAPI_ISteamMatchmaking_GetLobbyMemberByIndex", second_member).ret) ==
              kGuestId);

    check("the host still owns it",
          steammock::as_uint64(ask(guest, "SteamAPI_ISteamMatchmaking_GetLobbyOwner",
                                   lobby_argument("steamIDLobby", lobby, "owner"))
                                   .ret) == kHostId);

    Json a_member = Json::object();
    a_member["steamIDFriend"] = Json(static_cast<std::int64_t>(kGuestId));
    check("a member can be asked for by name",
          steammock::as_string(
              ask(host, "SteamAPI_ISteamFriends_GetFriendPersonaName", a_member).ret) == "Guest");

    Json member_name = lobby_argument("steamIDLobby", lobby, "member name");
    member_name["steamIDUser"] = Json(static_cast<std::int64_t>(kGuestId));
    member_name["pchKey"] = Json("name");
    check("and a roster row has a name to draw",
          steammock::as_string(
              ask(host, "SteamAPI_ISteamMatchmaking_GetLobbyMemberData", member_name).ret) ==
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

// A board is the run's, not a game's: one game posts a score and another reads the ranking
// it did not write. That is the whole reason it is not per session, and Spacewar's
// leaderboard menu is the game that reads it.
//
// The calls here are the menu's own sequence - find the board, ask for its name, download a
// range, read a row out of what came back - because the shape a game actually uses is the
// shape that has to work.
void test_the_boards()
{
    std::printf("[:] the boards\n");

    auto profile_for = [](const char* persona, std::uint64_t steam_id)
    {
        const std::string text = std::string("{\"app_id\":480,\"steam_id\":") +
                                 std::to_string(steam_id) + ",\"persona_name\":\"" + persona +
                                 "\",\"language\":\"english\"}";
        Json data;
        check("the board fixture parses", steammock::parse(text, data));
        return Profile::from_json(persona, data);
    };
    constexpr std::uint64_t kHostId = 76561198000000001ull;
    constexpr std::uint64_t kGuestId = 76561198000000002ull;

    auto session_for = [](const char* id, const Profile& profile)
    {
        Json hello = Json::object();
        hello["exe"] = Json("game.exe");
        hello["pid"] = Json(1234);
        return Session(id, hello, profile);
    };

    const Session host = session_for("host", profile_for("Host", kHostId));
    const Session guest = session_for("guest", profile_for("Guest", kGuestId));

    LeaderboardWorld boards;

    auto ask = [&](const Session& who, const char* call, const Json& args)
    {
        Answer answer;
        const bool handled = boards.answer(who, call, args, answer);
        check((std::string("the world answers ") + call).c_str(), handled);
        return answer;
    };
    auto field_of = [](const Answer& answer, const char* field) -> const Json*
    {
        if (!answer.events.is_array() || answer.events.empty())
        {
            return nullptr;
        }
        const Json* in = steammock::json_member(answer.events.front(), "in");
        return in != nullptr ? steammock::json_member(*in, field) : nullptr;
    };
    // One call made the way a game makes it: the arguments it passes, and nothing else.
    Json wanted = Json::object();
    wanted["pchLeaderboardName"] = Json("Feet Traveled");

    // A board nobody has asked for yet is not one. This is the call a game makes first, and
    // the answer to it is what a menu draws "there is no such board" from.
    const Answer missing = ask(guest, "SteamAPI_ISteamUserStats_FindLeaderboard", wanted);
    const Json* missing_flag = field_of(missing, "m_bLeaderboardFound");
    check("a board nobody asked for is not found",
          missing_flag != nullptr && steammock::as_int64(*missing_flag) == 0);

    Json create = wanted;
    create["eLeaderboardSortMethod"] = Json(2);  // descending: the biggest wins
    create["eLeaderboardDisplayType"] = Json(1); // a plain number
    const Answer created = ask(host, "SteamAPI_ISteamUserStats_FindOrCreateLeaderboard", create);
    const std::uint64_t board = created.ret.is_number() ? steammock::as_uint64(created.ret) : 0;
    check("find-or-create makes one and completes the call with it", board != 0);
    const Json* board_in_payload = field_of(created, "m_hSteamLeaderboard");
    check("  and the payload names the board it made",
          board_in_payload != nullptr && steammock::as_uint64(*board_in_payload) != 0);
    check("  and the two handles are different things: a call to wait on, a board to play on",
          board_in_payload != nullptr && steammock::as_uint64(*board_in_payload) != board);

    const std::uint64_t board_handle =
        board_in_payload != nullptr ? steammock::as_uint64(*board_in_payload) : 0;
    Json of_board = Json::object();
    of_board["hSteamLeaderboard"] = Json(static_cast<std::int64_t>(board_handle));

    check("it is called what the game asked for",
          steammock::as_string(
              ask(host, "SteamAPI_ISteamUserStats_GetLeaderboardName", of_board).ret) ==
              "Feet Traveled");
    check("and it is empty until somebody posts a score",
          steammock::as_int64(
              ask(host, "SteamAPI_ISteamUserStats_GetLeaderboardEntryCount", of_board).ret) == 0);
    check("and it ranks the way the game asked it to",
          steammock::as_int64(
              ask(host, "SteamAPI_ISteamUserStats_GetLeaderboardSortMethod", of_board).ret) == 2);

    // Two players post scores, and the board ranks them.
    auto upload = [&](const Session& who, std::int64_t method, std::int64_t score)
    {
        Json args = of_board;
        args["eLeaderboardUploadScoreMethod"] = Json(method);
        args["nScore"] = Json(score);
        args["pScoreDetails"] = Json();
        args["cScoreDetailsCount"] = Json(0);
        return ask(who, "SteamAPI_ISteamUserStats_UploadLeaderboardScore", args);
    };

    const Answer host_first = upload(host, 1, 100); // keep best
    check("a player who was not on the board is ranked when they post",
          steammock::as_int64(*field_of(host_first, "m_nGlobalRankNew")) == 1 &&
              steammock::as_int64(*field_of(host_first, "m_nGlobalRankPrevious")) == 0);
    check("and the board says a score it did not have changed it",
          steammock::as_int64(*field_of(host_first, "m_bScoreChanged")) == 1);

    const Answer guest_first = upload(guest, 1, 300);
    check("and the better score takes the top",
          steammock::as_int64(*field_of(guest_first, "m_nGlobalRankNew")) == 1);
    check("which moves the other player down rather than deleting them",
          steammock::as_int64(
              ask(host, "SteamAPI_ISteamUserStats_GetLeaderboardEntryCount", of_board).ret) == 2);

    const Answer host_worse = upload(host, 1, 50);
    check("keep-best throws away a worse score",
          steammock::as_int64(*field_of(host_worse, "m_bScoreChanged")) == 0);
    check("  and says so by leaving the rank where it was",
          steammock::as_int64(*field_of(host_worse, "m_nGlobalRankNew")) == 2 &&
              steammock::as_int64(*field_of(host_worse, "m_nScore")) == 100);

    const Answer host_forced = upload(host, 2, 900);
    check("force-update replaces it anyway",
          steammock::as_int64(*field_of(host_forced, "m_bScoreChanged")) == 1 &&
              steammock::as_int64(*field_of(host_forced, "m_nGlobalRankNew")) == 1);
    check("  and remembers where the player was before",
          steammock::as_int64(*field_of(host_forced, "m_nGlobalRankPrevious")) == 2);

    // Now the menu's own read: a range of global ranks, top first, and a row out of it.
    Json download = of_board;
    download["eLeaderboardDataRequest"] = Json(0); // global
    download["nRangeStart"] = Json(1);
    download["nRangeEnd"] = Json(10);
    const Answer rows = ask(guest, "SteamAPI_ISteamUserStats_DownloadLeaderboardEntries", download);
    const Json* count = field_of(rows, "m_cEntryCount");
    check("a download of the top ten is answered with what the board holds",
          count != nullptr && steammock::as_int64(*count) == 2);
    const Json* entries_handle = field_of(rows, "m_hSteamLeaderboardEntries");
    check("  and the rows come back under a handle of their own",
          entries_handle != nullptr && steammock::as_uint64(*entries_handle) != 0);
    const std::uint64_t entries =
        entries_handle != nullptr ? steammock::as_uint64(*entries_handle) : 0;

    auto row_args = [&](std::int64_t index, std::int64_t details_max)
    {
        Json args = Json::object();
        args["hSteamLeaderboardEntries"] = Json(static_cast<std::int64_t>(entries));
        args["index"] = Json(index);
        args["cDetailsMax"] = Json(details_max);
        return args;
    };

    const Answer first =
        ask(guest, "SteamAPI_ISteamUserStats_GetDownloadedLeaderboardEntry", row_args(0, 0));
    Answer unheard;
    check("a row read out of the download is the player's", steammock::as_bool(first.ret));
    const Json* entry = steammock::json_member(first.out, "pLeaderboardEntry");
    check("  and the row itself is a structure the game's own memory gets filled with",
          entry != nullptr && entry->is_object());
    const Json* who = entry != nullptr ? steammock::json_member(*entry, "m_steamIDUser") : nullptr;
    check("  with the player who earned it",
          who != nullptr && steammock::as_uint64(*who) == kHostId);
    const Json* score = entry != nullptr ? steammock::json_member(*entry, "m_nScore") : nullptr;
    const Json* rank = entry != nullptr ? steammock::json_member(*entry, "m_nGlobalRank") : nullptr;
    check("  and the score and the place it took",
          score != nullptr && steammock::as_int64(*score) == 900 && rank != nullptr &&
              steammock::as_int64(*rank) == 1);
    check("and a row nobody downloaded is not invented",
          !boards.answer(guest, "SteamAPI_ISteamUserStats_GetDownloadedLeaderboardEntry",
                         row_args(5, 0), unheard));
    check("and a handle nobody was given has no rows either",
          !boards.answer(
              guest, "SteamAPI_ISteamUserStats_GetDownloadedLeaderboardEntry",
              [&]
              {
                  Json args = row_args(0, 0);
                  args["hSteamLeaderboardEntries"] = Json(static_cast<std::int64_t>(4242));
                  return args;
              }(),
              unheard));
    check("and a detail array is written only when the game asked for one",
          steammock::json_member(
              ask(guest, "SteamAPI_ISteamUserStats_GetDownloadedLeaderboardEntry", row_args(0, 4))
                  .out,
              "pDetails") != nullptr);

    // The two ends of "there is no board here": a handle this run never gave out, and a
    // board name it has never been asked for.
    Json stranger = Json::object();
    stranger["hSteamLeaderboard"] = Json(static_cast<std::int64_t>(4242));
    check("a board handle nobody was given has no name",
          !boards.answer(host, "SteamAPI_ISteamUserStats_GetLeaderboardName", stranger, unheard));
    check("and a count is not invented for it either",
          !boards.answer(host, "SteamAPI_ISteamUserStats_GetLeaderboardEntryCount", stranger,
                         unheard));

    check("the world handles the leaderboard surface",
          LeaderboardWorld::handled_calls().size() >= 9);
}
// A packet is stamped with the end of the process that sent it, which is the handle the
// call was made through - not something read off the destination. The difference is the
// whole of it for a process that hosts, because that one is a customer and a game server
// at once: Spacewar's own ticket exchange sends from the customer's ISteamNetworking to
// another player, and a packet stamped with the game server's id instead is one the peer
// reads as something the server sent, and drops.
void test_who_sent_a_packet()
{
    std::printf("[:] who sent a packet\n");

    auto profile_for = [](const char* persona, std::uint64_t steam_id)
    {
        const std::string text = std::string("{\"app_id\":480,\"steam_id\":") +
                                 std::to_string(steam_id) + ",\"persona_name\":\"" + persona +
                                 "\",\"language\":\"english\"}";
        Json data;
        check("the two-game fixture parses", steammock::parse(text, data));
        return Profile::from_json(persona, data);
    };
    constexpr std::uint64_t kHostId = 76561198000000001ull;
    constexpr std::uint64_t kGuestId = 76561198000000002ull;

    auto session_for = [](const char* id, const Profile& profile)
    {
        Json hello = Json::object();
        hello["exe"] = Json("game.exe");
        hello["pid"] = Json(1234);
        return Session(id, hello, profile);
    };

    Session host = session_for("host", profile_for("Host", kHostId));
    Session guest = session_for("guest", profile_for("Guest", kGuestId));

    LobbyWorld world;
    std::vector<std::pair<std::uint64_t, Json>> told;

    auto ask = [&](Session& who, const char* call, const Json& args)
    {
        Answer answer;
        told.clear();
        const bool handled = world.answer(who, call, args, answer, told);
        check((std::string("the world answers ") + call).c_str(), handled);
        return answer;
    };
    auto send = [](std::uint64_t to, const char* bytes, std::int32_t h_user)
    {
        Json args = Json::object();
        args["steamIDRemote"] = Json(static_cast<std::int64_t>(to));
        args["pubData"] = Json(bytes);
        args["nChannel"] = Json(0);
        // What the stub puts on the wire for every ISteamNetworking call: the handle the
        // object was handed out under, which is the only thing that says which end asked.
        args["hSteamUser"] = Json(static_cast<std::int64_t>(h_user));
        return args;
    };
    auto read_args = [](std::int32_t h_user, std::int32_t channel)
    {
        Json args = Json::object();
        args["hSteamUser"] = Json(static_cast<std::int64_t>(h_user));
        args["nChannel"] = Json(channel);
        return args;
    };
    auto read = [&](Session& who, std::int32_t h_user, Answer& into)
    {
        told.clear();
        return world.answer(who, "SteamAPI_ISteamNetworking_ReadP2PPacket", read_args(h_user, 0),
                            into, told);
    };
    auto remote_of = [](const Answer& answer)
    {
        const Json* value = steammock::json_member(answer.out, "psteamIDRemote");
        return value != nullptr ? steammock::as_uint64(*value) : 0ull;
    };

    // The host's game server id, minted by the first call that asks - which is what
    // Spacewar does when it puts that id in the lobby's payload.
    const std::uint64_t server =
        steammock::as_uint64(ask(host, "SteamAPI_ISteamGameServer_GetSteamID", Json::object()).ret);
    check("a game server has an id of its own", server != 0 && server != kHostId);

    // The customer's object, which is what the ticket exchange sends a ticket through.
    ask(host, "SteamAPI_ISteamNetworking_SendP2PPacket",
        send(kGuestId, "0f", LobbyWorld::kCustomerHSteamUser));

    Answer read_back;
    check("the guest reads what the customer sent",
          read(guest, LobbyWorld::kCustomerHSteamUser, read_back));
    check("and it is from the host rather than from its game server",
          remote_of(read_back) == kHostId);
    check("with the bytes that were sent", text_of(read_back.out, "pubDest") == "0f");

    // The game server's object, which is what the server sends its clients through: that
    // one has to arrive as the game server, or a client cannot tell the server's messages
    // from another player's.
    ask(host, "SteamAPI_ISteamNetworking_SendP2PPacket",
        send(kGuestId, "2a", LobbyWorld::kGameServerHSteamUser));
    check("the guest reads what the game server sent",
          read(guest, LobbyWorld::kCustomerHSteamUser, read_back));
    check("and that one is from the game server", remote_of(read_back) == server);
    check("with the bytes that were sent", text_of(read_back.out, "pubDest") == "2a");

    // And the other way: a packet addressed to the game server is the game server's to
    // read, rather than the other end's of a process that has both.
    ask(guest, "SteamAPI_ISteamNetworking_SendP2PPacket",
        send(server, "3c", LobbyWorld::kCustomerHSteamUser));

    Answer for_the_server;
    check("the game server reads what a client addressed to it",
          read(host, LobbyWorld::kGameServerHSteamUser, for_the_server));
    check("and it came from the client", remote_of(for_the_server) == kGuestId);

    Answer for_the_customer;
    told.clear();
    check("the host's own end is not handed it instead",
          !world.answer(host, "SteamAPI_ISteamNetworking_ReadP2PPacket",
                        read_args(LobbyWorld::kCustomerHSteamUser, 0), for_the_customer, told));

    // And the other direction of the same fact: a packet addressed to the host's own id
    // belongs to the customer end, which is where the game's ticket exchange puts a
    // guest's ticket - `p2pauth` addresses the server owner's SteamID, not the game
    // server's, and a guest's auth player is told that id by the world update.
    ask(guest, "SteamAPI_ISteamNetworking_SendP2PPacket",
        send(kHostId, "4d", LobbyWorld::kCustomerHSteamUser));

    Answer for_the_owner;
    check("the host's own end reads what a guest addressed to it",
          read(host, LobbyWorld::kCustomerHSteamUser, for_the_owner));
    check("and it came from the guest", remote_of(for_the_owner) == kGuestId);

    Answer not_for_the_server;
    told.clear();
    check("the game server end is not handed that one either",
          !world.answer(host, "SteamAPI_ISteamNetworking_ReadP2PPacket",
                        read_args(LobbyWorld::kGameServerHSteamUser, 0), not_for_the_server, told));

    // A session's own exchange does not queue behind another game's. A guest's packet
    // reaches the host's game server first, and then the host's own customer end sends it
    // one: the read hands back the host's own, because that one never left the machine.
    // That is the ordering a game relies on without saying so - its server is in its own
    // process and a peer's is not - and it is why a hosting game's own ticket beats a
    // guest's to the slot its server hands out first.
    ask(guest, "SteamAPI_ISteamNetworking_SendP2PPacket",
        send(server, "5e", LobbyWorld::kCustomerHSteamUser));
    ask(host, "SteamAPI_ISteamNetworking_SendP2PPacket",
        send(server, "6f", LobbyWorld::kCustomerHSteamUser));

    Answer own_first;
    check("the host's own packet to its game server is served first",
          read(host, LobbyWorld::kGameServerHSteamUser, own_first));
    check("and it is the host's own, not the guest's",
          remote_of(own_first) == kHostId && text_of(own_first.out, "pubDest") == "6f");

    Answer then_the_guest;
    check("the guest's packet is still waiting behind it",
          read(host, LobbyWorld::kGameServerHSteamUser, then_the_guest));
    check("and it is the guest's, unchanged",
          remote_of(then_the_guest) == kGuestId && text_of(then_the_guest.out, "pubDest") == "5e");

    // The auth handshake has two halves and both have to be answered: a game server asking
    // about a player, and a player asking about the peer it is about to play with. Each is
    // promised its answer as a ValidateAuthTicketResponse_t callback rather than as the
    // return value, so a world that answers only the server's half leaves a client's auth
    // player waiting for a validation that never comes - until the game's own ticket
    // timeout drops the peer it asked about, which is a client leaving a server that had
    // never registered it.
    auto auth_args = [](std::uint64_t peer)
    {
        Json args = Json::object();
        args["pAuthTicket"] = Json(static_cast<std::int64_t>(0x1000));
        args["cbAuthTicket"] = Json(14);
        args["steamID"] = Json(static_cast<std::int64_t>(peer));
        return args;
    };

    Answer server_half;
    told.clear();
    check("the server's half of the handshake is answered",
          world.answer(host, "SteamAPI_ISteamGameServer_BeginAuthSession", auth_args(kGuestId),
                       server_half, told));
    check("and it announces the validation to the server",
          told.size() == 1 && told[0].first == kHostId &&
              text_of(told[0].second, "event") == "ValidateAuthTicketResponse_t");

    Answer user_half;
    told.clear();
    check("so is a player's own half", world.answer(guest, "SteamAPI_ISteamUser_BeginAuthSession",
                                                    auth_args(kHostId), user_half, told));
    check("with the OK auth response", steammock::as_int64(user_half.ret) == 0);
    check("announcing the validation to that player",
          told.size() == 1 && told[0].first == kGuestId &&
              text_of(told[0].second, "event") == "ValidateAuthTicketResponse_t");
    const Json* named = told.empty() ? nullptr : steammock::json_member(told[0].second, "in");
    const Json* peer = named == nullptr ? nullptr : steammock::json_member(*named, "m_SteamID");
    check("and naming the peer it asked about",
          peer != nullptr && steammock::as_uint64(*peer) == kHostId);
}

// A roster is asked for names by id, and the ids it asks about are not only the people
// standing in a room. Spacewar draws its scoreboard from a set of ids it collected as it
// played and re-asks for every name whenever it rebuilds the list, so answering only for
// current members of a room means declining most of those - a game drawing an empty name
// where a player's is.
void test_names_after_the_room()
{
    std::printf("[:] names for players who have left\n");

    auto profile_for = [](const char* persona, std::uint64_t steam_id)
    {
        const std::string text = std::string("{\"app_id\":480,\"steam_id\":") +
                                 std::to_string(steam_id) + ",\"persona_name\":\"" + persona +
                                 "\",\"language\":\"english\"}";
        Json data;
        check("the roster fixture parses", steammock::parse(text, data));
        return Profile::from_json(persona, data);
    };
    constexpr std::uint64_t kHostId = 76561198000000001ull;
    constexpr std::uint64_t kGuestId = 76561198000000002ull;
    constexpr std::uint64_t kStrangerId = 76561198000000099ull;

    auto session_for = [](const char* id, const Profile& profile)
    {
        Json hello = Json::object();
        hello["exe"] = Json("game.exe");
        hello["pid"] = Json(1234);
        return Session(id, hello, profile);
    };

    Session host = session_for("host", profile_for("Host", kHostId));
    Session guest = session_for("guest", profile_for("Guest", kGuestId));

    LobbyWorld world;
    std::vector<std::pair<std::uint64_t, Json>> told;
    auto ask = [&](Session& who, const char* call, const Json& args)
    {
        Answer answer;
        told.clear();
        check((std::string("the world answers ") + call).c_str(),
              world.answer(who, call, args, answer, told));
        return answer;
    };
    auto name_of = [](std::uint64_t steam_id)
    {
        Json args = Json::object();
        args["steamIDFriend"] = Json(static_cast<std::int64_t>(steam_id));
        return args;
    };

    // The two games meet in a room, and one of them leaves it.
    Json create = Json::object();
    create["eLobbyType"] = Json(2);
    create["cMaxMembers"] = Json(4);
    const Answer created = ask(host, "SteamAPI_ISteamMatchmaking_CreateLobby", create);
    const Json* made =
        created.events.is_array() && !created.events.empty()
            ? steammock::json_member(*steammock::json_member(created.events.front(), "in"),
                                     "m_ulSteamIDLobby")
            : nullptr;
    const std::uint64_t lobby = made != nullptr ? steammock::as_uint64(*made) : 0;
    Json join = Json::object();
    join["steamIDLobby"] = Json(static_cast<std::int64_t>(lobby));
    ask(guest, "SteamAPI_ISteamMatchmaking_JoinLobby", join);

    check("a member of the room is named",
          steammock::as_string(
              ask(host, "SteamAPI_ISteamFriends_GetFriendPersonaName", name_of(kGuestId)).ret) ==
              "Guest");

    Json leave = Json::object();
    leave["steamIDLobby"] = Json(static_cast<std::int64_t>(lobby));
    ask(guest, "SteamAPI_ISteamMatchmaking_LeaveLobby", leave);

    Answer after;
    told.clear();
    check("and is still named after leaving the room the name came from",
          world.answer(host, "SteamAPI_ISteamFriends_GetFriendPersonaName", name_of(kGuestId),
                       after, told) &&
              steammock::as_string(after.ret) == "Guest");

    // What has not changed: nobody this run has ever heard from has a name, because
    // inventing one would be a roster row for a player who does not exist.
    Answer stranger;
    told.clear();
    check("an id nobody has heard from still has no name",
          !world.answer(host, "SteamAPI_ISteamFriends_GetFriendPersonaName", name_of(kStrangerId),
                        stranger, told));
}

// Everything an inventory is, from the world's side: the catalogue a game reads names out of,
// what a player holds and how it got there, and the two payloads that end a result.
void test_the_items()
{
    std::printf("[:] the items\n");

    auto profile_for = [](const char* persona, std::uint64_t steam_id)
    {
        const std::string text = std::string("{\"app_id\":480,\"steam_id\":") +
                                 std::to_string(steam_id) + ",\"persona_name\":\"" + persona +
                                 "\",\"language\":\"english\"}";
        Json data;
        check("the item fixture parses", steammock::parse(text, data));
        return Profile::from_json(persona, data);
    };
    constexpr std::uint64_t kPlayerId = 76561198000000011ull;
    constexpr std::uint64_t kStrangerId = 76561198000000012ull;

    auto session_for = [](const char* id, const Profile& profile)
    {
        Json hello = Json::object();
        hello["exe"] = Json("game.exe");
        hello["pid"] = Json(1234);
        return Session(id, hello, profile);
    };

    const Session player = session_for("player", profile_for("Player", kPlayerId));
    const Session stranger = session_for("stranger", profile_for("Stranger", kStrangerId));

    InventoryWorld items;

    auto ask = [&](const Session& who, const char* call, const Json& args)
    {
        Answer answer;
        const bool handled = items.answer(who, call, args, answer);
        check((std::string("the world answers ") + call).c_str(), handled);
        return answer;
    };
    auto handle_of = [](const Answer& answer)
    {
        // The handle the answer handed this game, which is an out parameter rather than the
        // return value: `GetAllItems( NULL )` is the call a game makes without wanting one.
        const Json* value = steammock::json_member(answer.out, "pResultHandle");
        return value != nullptr ? static_cast<std::int32_t>(steammock::as_int64(*value)) : -1;
    };
    auto events_of = [](const Answer& answer, const char* name)
    {
        if (!answer.events.is_array())
        {
            return 0;
        }
        int count = 0;
        for (const Json& event : answer.events)
        {
            const Json* field = steammock::json_member(event, "event");
            if (field != nullptr && steammock::as_string(*field) == name)
            {
                ++count;
            }
        }
        return count;
    };
    auto event_field = [](const Answer& answer, const char* name,
                          const char* field) -> const Json*
    {
        if (!answer.events.is_array())
        {
            return nullptr;
        }
        for (const Json& event : answer.events)
        {
            const Json* what = steammock::json_member(event, "event");
            if (what == nullptr || steammock::as_string(*what) != name)
            {
                continue;
            }
            const Json* in = steammock::json_member(event, "in");
            return in != nullptr ? steammock::json_member(*in, field) : nullptr;
        }
        return nullptr;
    };

    // A catalogue call, which is the first thing a game makes: it is answered yes, and *no* is
    // the answer a game would stop asking after.
    const Answer loaded =
        ask(player, "SteamAPI_ISteamInventory_LoadItemDefinitions", Json::object());
    check("the catalogue loads", steammock::as_bool(loaded.ret));
    check("the catalogue is this run's own, not anything a game sent",
          items.definitions().size() >= 8u);

    // Nothing is held until something is granted, and the grant is what the game asks for.
    check("a player holds nothing before anything is granted",
          items.inventory_of(kPlayerId).empty());

    const Answer granted = ask(player, "SteamAPI_ISteamInventory_GrantPromoItems", Json::object());
    const std::int32_t granted_handle = handle_of(granted);
    check("granting hands back a result handle and ends with a result ready",
          granted_handle >= 0 && events_of(granted, "SteamInventoryResultReady_t") == 1);
    check("  and the payload names that handle",
          event_field(granted, "SteamInventoryResultReady_t", "m_handle") != nullptr &&
              static_cast<std::int32_t>(steammock::as_int64(*event_field(
                  granted, "SteamInventoryResultReady_t", "m_handle"))) == granted_handle);
    check("  and it says the result is OK",
          static_cast<std::int32_t>(steammock::as_int64(
              *event_field(granted, "SteamInventoryResultReady_t", "m_result"))) == 1);

    // What a game does next: read the result, twice - once with a null array to be told how
    // many there are, and once with an array of that many. What the world answers is the same
    // list both times; how much of it lands in the game's array is the buffer's business.
    auto id_args = [](const char* name, std::int32_t handle)
    {
        Json args = Json::object();
        args[name] = Json(static_cast<std::int64_t>(handle));
        return args;
    };
    auto item_list = [](const Answer& answer) -> const Json*
    {
        return steammock::json_member(answer.out, "pOutItemsArray");
    };
    auto definition_of = [](const Json& item)
    {
        const Json* value = steammock::json_member(item, "m_iDefinition");
        return value != nullptr ? static_cast<std::int32_t>(steammock::as_int64(*value)) : -1;
    };

    const Answer listed = ask(player, "SteamAPI_ISteamInventory_GetResultItems",
                              id_args("resultHandle", granted_handle));
    check("the granted items come back as a list of items",
          steammock::as_bool(listed.ret) && item_list(listed) != nullptr &&
              item_list(listed)->is_array() && item_list(listed)->size() == 2u);
    check("  and each one is the structure a game's own array is made of",
          definition_of((*item_list(listed))[0]) == 100 &&
              definition_of((*item_list(listed))[1]) == 101 &&
              int_of((*item_list(listed))[0], "m_unQuantity") == 1 &&
              int_of((*item_list(listed))[0], "m_unFlags") == 0 &&
              steammock::as_uint64(*steammock::json_member((*item_list(listed))[0], "m_itemId")) !=
                  0u);
    check("  and the instances are different ones, not one item twice",
          steammock::as_uint64(*steammock::json_member((*item_list(listed))[0], "m_itemId")) !=
              steammock::as_uint64(*steammock::json_member((*item_list(listed))[1], "m_itemId")));
    check("  and the count is not in the answer: the buffer owns it",
          steammock::json_member(listed.out, "punOutItemsArraySize") == nullptr);

    // Held, now - which is what makes the next answer different from the one before it.
    check("the granted items are held", items.inventory_of(kPlayerId).size() == 2u);
    check("  and the other player holds nothing", items.inventory_of(kStrangerId).empty());

    // A full update is the whole inventory, and it arrives as two payloads in the SDK's order:
    // the full update first, the result ready that ends it second.
    const Answer everything = ask(player, "SteamAPI_ISteamInventory_GetAllItems", Json::object());
    const std::int32_t full_handle = handle_of(everything);
    check("everything held comes back as a full update",
          events_of(everything, "SteamInventoryFullUpdate_t") == 1 &&
              events_of(everything, "SteamInventoryResultReady_t") == 1);
    check("  and the full update comes first",
          everything.events.is_array() && everything.events.size() == 2u &&
              steammock::as_string(*steammock::json_member(everything.events[0], "event")) ==
                  "SteamInventoryFullUpdate_t");
    check("  and both payloads name the handle the call was given",
          event_field(everything, "SteamInventoryFullUpdate_t", "m_handle") != nullptr &&
              static_cast<std::int32_t>(steammock::as_int64(*event_field(
                  everything, "SteamInventoryFullUpdate_t", "m_handle"))) == full_handle);
    const Answer full_list = ask(player, "SteamAPI_ISteamInventory_GetResultItems",
                                 id_args("resultHandle", full_handle));
    check("  and it carries every item held",
          item_list(full_list) != nullptr && item_list(full_list)->size() == 2u &&
              definition_of((*item_list(full_list))[0]) == 100 &&
              definition_of((*item_list(full_list))[1]) == 101);

    // Granting again grants nothing new: an inventory is a set of instances, and this is the
    // call a game makes on every start.
    const Answer again = ask(player, "SteamAPI_ISteamInventory_GrantPromoItems", Json::object());
    const Answer again_list = ask(player, "SteamAPI_ISteamInventory_GetResultItems",
                                  id_args("resultHandle", handle_of(again)));
    check("granting twice does not grant twice", item_list(again_list) != nullptr &&
                                                     item_list(again_list)->empty() &&
                                                     items.inventory_of(kPlayerId).size() == 2u);

    // The catalogue is read by name, which is the call a game makes to draw an item.
    Json wanted = Json::object();
    wanted["iDefinition"] = Json(100);
    wanted["pchPropertyName"] = Json("name");
    const Answer named = ask(player, "SteamAPI_ISteamInventory_GetItemDefinitionProperty", wanted);
    check("a definition's name is read out of the catalogue",
          steammock::as_bool(named.ret) && out_flag(named, "pchValueBuffer") == false &&
              text_of(named.out, "pchValueBuffer") == "Ship Decoration 1");
    check("  and the length is not in the answer: the buffer owns it",
          steammock::json_member(named.out, "punValueBufferSizeOut") == nullptr);

    // A definition the app does not have, and a property it does not carry: both are the
    // catalogue's own "no", which a game turns into "(unknown)".
    Json missing_definition = wanted;
    missing_definition["iDefinition"] = Json(999);
    const Answer unknown_definition =
        ask(player, "SteamAPI_ISteamInventory_GetItemDefinitionProperty", missing_definition);
    check("a definition this app does not have is answered no",
          !steammock::as_bool(unknown_definition.ret));
    Json missing_property = wanted;
    missing_property["pchPropertyName"] = Json("colour");
    const Answer unknown_property =
        ask(player, "SteamAPI_ISteamInventory_GetItemDefinitionProperty", missing_property);
    check("a property the definition does not carry is answered no",
          !steammock::as_bool(unknown_property.ret));

    // A result belongs to the player it was handed to.
    Json check_args = Json::object();
    check_args["resultHandle"] = Json(static_cast<std::int64_t>(granted_handle));
    check_args["steamIDExpected"] = Json(kPlayerId);
    check("a result belongs to the player it was handed to",
          steammock::as_bool(
              ask(player, "SteamAPI_ISteamInventory_CheckResultSteamID", check_args).ret));
    check_args["steamIDExpected"] = Json(kStrangerId);
    check("  and not to anybody else",
          !steammock::as_bool(
              ask(player, "SteamAPI_ISteamInventory_CheckResultSteamID", check_args).ret));

    // A handle the run never handed out has no list behind it - and one that belongs to another
    // player's inventory is the same answer, whatever the reader's own id is.
    Answer invented;
    Json invented_args = Json::object();
    invented_args["resultHandle"] = Json(static_cast<std::int64_t>(999999));
    check(
        "a handle nobody was given is not something this world answers for",
        !items.answer(player, "SteamAPI_ISteamInventory_GetResultItems", invented_args, invented));
    check("  and another player's result is not this player's to read",
          !items.answer(stranger, "SteamAPI_ISteamInventory_GetResultItems", check_args, invented));

    // A game destroys what it has read, and a result that is gone is gone: Spacewar destroys
    // every result it is given, and the second read of one is a read of nothing.
    Json destroy_args = Json::object();
    destroy_args["resultHandle"] = Json(static_cast<std::int64_t>(granted_handle));
    check("a result is answered when it is destroyed",
          steammock::as_bool(
              ask(player, "SteamAPI_ISteamInventory_DestroyResult", destroy_args).ret));
    check("  and reading it afterwards is not answered",
          !items.answer(player, "SteamAPI_ISteamInventory_GetResultItems",
                        id_args("resultHandle", granted_handle), invented));
    check("  and destroying it did not empty the inventory",
          items.inventory_of(kPlayerId).size() == 2u);

    // The three calls that ask for an item to be made or moved: answered, and nothing changes.
    // This is the whole of what keeps an inventory a record of what a game was given.
    const std::size_t held = items.inventory_of(kPlayerId).size();
    Json exchange = Json::object();
    exchange["unArrayGenerateLength"] = Json(1);
    const Answer exchanged = ask(player, "SteamAPI_ISteamInventory_ExchangeItems", exchange);
    check("an exchange is answered and grants nothing",
          handle_of(exchanged) >= 0 && items.inventory_of(kPlayerId).size() == held);
    const Answer generated = ask(player, "SteamAPI_ISteamInventory_GenerateItems", Json::object());
    check("items asked for are not minted", items.inventory_of(kPlayerId).size() == held);
    const Answer dropped = ask(player, "SteamAPI_ISteamInventory_TriggerItemDrop", Json::object());
    check("a drop is answered and there is none",
          handle_of(dropped) >= 0 && items.inventory_of(kPlayerId).size() == held);

    check("the world handles the inventory surface", InventoryWorld::handled_calls().size() >= 11);
}

// Every call the worlds claim to handle has to be one the stub can send: the stub is
// generated from an SDK's own surface, and a world that answers a name nobody exports is a
// world with an opinion no game can ever ask about.
//
// The surface is generated from gen/steam_api_surface.json, which is Valve's API and not in
// this repository - so a checkout with no SDK has nothing to check against, and says so
// rather than passing by default.
void test_the_worlds_handle_calls_the_stub_has()
{
    std::printf("[:] the calls the worlds answer\n");

    std::size_t count = 0;
    const steammock::SurfaceCall* calls = steammock::api_surface_calls(count);
    if (count == 0)
    {
        std::printf("  [skip] no generated surface in this checkout: nothing to check against\n");
        return;
    }
    auto exported = [calls, count](const std::string& name)
    {
        for (std::size_t index = 0; index < count; ++index)
        {
            if (name == calls[index].name)
            {
                return true;
            }
        }
        return false;
    };

    std::vector<std::string> claimed = LobbyWorld::handled_calls();
    for (const std::string& call : LeaderboardWorld::handled_calls())
    {
        claimed.push_back(call);
    }
    for (const std::string& call : InventoryWorld::handled_calls())
    {
        claimed.push_back(call);
    }
    std::size_t unknown = 0;
    for (const std::string& call : claimed)
    {
        if (!exported(call))
        {
            std::printf("        not in the surface: %s\n", call.c_str());
            ++unknown;
        }
    }
    check("every call the worlds answer is one the stub exports", unknown == 0);
}

void test_surface_matches_the_idl()
{
    std::printf("[:] the generated surface\n");

    std::size_t count = 0;
    const steammock::SurfaceCall* calls = steammock::api_surface_calls(count);
    // Nothing to say about a surface that was not imported: it is Valve's API, it is
    // not part of the checkout, and a build without one has a stub that exports
    // nothing but its own diagnostics. Saying so is the check, rather than failing
    // on a table that is empty on purpose - the same shape as the end-to-end test's
    // half that needs an interface layout.
    if (count == 0u)
    {
        std::printf("  [skip] no API surface was imported, so there is nothing here to check\n");
        return;
    }
    std::set<std::string> names;
    for (std::size_t index = 0; index < count; ++index)
    {
        names.insert(calls[index].name);
    }
    check("the surface has calls", count > 0u);
    // The name is the SDK the surface was read from, so it is a fact about the
    // import rather than a constant a person maintains.
    check("it names the surface it came from", std::string(steammock::api_surface_name()) != "?");
    check("a policy call is listed", names.count("SteamAPI_Init") == 1u);
    check("an out-parameter call is listed", names.count("SteamAPI_ISteamUserStats_GetStat") == 1u);
    // Windows resolves a game's whole import table before it runs, so a name a
    // real game imports and the stub does not export is a game that will not
    // start. Both of these are in Spacewar's own executable.
    check("a GameServer call is listed", names.count("SteamGameServer_GetHSteamPipe") == 1u);
    check("an internal helper is listed", names.count("SteamInternal_CreateInterface") == 1u);

    // Every call the state machine answers has to be in the surface, or the stub
    // and the backend have drifted apart.
    std::vector<std::string> missing;
    for (const std::string& handled : steammock::state_handled_calls())
    {
        if (names.count(handled) == 0u)
        {
            missing.push_back(handled);
        }
    }
    for (const std::string& name : missing)
    {
        std::printf("        the state machine answers %s, which the IDL does not export\n",
                    name.c_str());
    }
    check("every call the state machine answers is exported", missing.empty());

    // And the surface has to describe the parameters --list-api prints.
    for (std::size_t index = 0; index < count; ++index)
    {
        if (std::string(calls[index].name) != "SteamAPI_ISteamUserStats_GetStat")
        {
            continue;
        }
        check("the out parameter is marked as one",
              calls[index].param_count == 3u && calls[index].params[2].out);
        check("the out parameter keeps its name",
              std::string(calls[index].params[2].name) == "pData");
    }

    // Which interface a name belongs to, and how many slots one version of it gives the method.
    // The second is the only way a table keyed by one name can show that the SDK overloads a
    // method: a C function cannot be overloaded, so `GetStat`'s int32 slot and its float slot
    // travel under the one name and an override on it covers both. A reader has to be told that,
    // and this is where the layouts and the surface are held together.
    std::size_t unnamed_overloads = 0;
    for (std::size_t index = 0; index < count; ++index)
    {
        const std::string name(calls[index].name);
        const std::string owner(calls[index].interface_name);
        if (name == "SteamAPI_ISteamUserStats_GetStat")
        {
            check("the name says which interface declares it", owner == "ISteamUserStats");
        }
        if (name == "SteamAPI_ISteamUserStats_GetStat0")
        {
            // The SDK's own flat alias for one overload of `GetStat`. Whether the layouts name the
            // slot with it depends on the import, so what is pinned is that the alias knows its
            // interface either way: matched by the digits on the end, or named by the slot itself.
            check("an alias knows which interface it belongs to", owner == "ISteamUserStats");
        }
        if (name == "SteamAPI_Init")
        {
            check("a top-level entry point belongs to no interface", owner.empty());
            check("and carries its own slot alone", calls[index].overloads == 1u);
        }
        // Only a slot can share a name with another, so a name that does has to name an interface.
        // Counted rather than checked where it is found, because the same sentence printed once per
        // call is not a report.
        if (calls[index].overloads > 1u && owner.empty())
        {
            ++unnamed_overloads;
        }
    }
    check("every name shared by slots names an interface", unnamed_overloads == 0u);
}

void test_numbers()
{
    std::printf("[:] numbers in a transcript\n");

    // Readability: a transcript is meant to be read by a person.
    check("a fraction survives", steammock::Json(0.5).dump() == "0.5");
    check("a duration reads as a duration", steammock::Json(0.164).dump() == "0.164");
    // The spelling is nlohmann's now, and a whole double keeps its point. What has
    // to hold is that a reader gets the same number back, which the checks around
    // this one are for.
    check("a whole second stays a number", steammock::Json(1500.0).dump() == "1500.0");
    check("zero is zero", steammock::Json(0.0).dump() == "0.0");
    check("a whole number keeps its point", steammock::Json(-12.0).dump() == "-12.0");

    // Exactness: the writer only shortens when the text still denotes the very
    // same double, which is what the C library reading it back proves.
    for (const double value :
         {0.164, 1500.0, 0.1, 1.0 / 3.0, -2.5, 3.141592653589793, 1e-300, 1e300})
    {
        const std::string text = steammock::Json(value).dump();
        if (std::strtod(text.c_str(), nullptr) != value)
        {
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
    for (const double value : {0.5, -2.5, 1500.0, 0.0})
    {
        check("a binary-exact double is read back unchanged",
              steammock::parse(steammock::Json(value).dump(), back) &&
                  steammock::as_double(back) == value);
    }
    check("a fraction is read back to within a rounding step",
          steammock::parse(steammock::Json(0.164).dump(), back) &&
              std::fabs(steammock::as_double(back) - 0.164) < 1e-15);
}

} // namespace

int run()
{
    std::printf("[+] SteamMock backend tests\n\n");
    test_replies();
    test_relabelling();
    test_identity();
    test_friends();
    test_invites();
    test_stats();
    test_achievements();
    test_describe();
    test_scenarios();
    test_profiles_are_per_session();
    test_the_lobbies_a_run_holds();
    test_the_boards();
    test_the_items();
    test_names_after_the_room();
    test_who_sent_a_packet();
    test_the_worlds_handle_calls_the_stub_has();
    test_surface_matches_the_idl();
    test_numbers();

    if (g_failures == 0)
    {
        std::printf("\n[+] all checks passed\n");
    }
    else
    {
        std::printf("\n[-] %d check(s) FAILED\n", g_failures);
    }
    return g_failures == 0 ? 0 : 1;
}

// An exception escaping `main` terminates the process with no message at all, and the
// only realistic source in a test is a failed allocation. Report it the way a failing
// check is reported instead, so ctest's output says what happened.
int main()
{
    try
    {
        return run();
    }
    catch (const std::exception& error)
    {
        std::printf("\n[-] the test itself threw: %s\n", error.what());
        return 1;
    }
    catch (...)
    {
        std::printf("\n[-] the test itself threw something that is not a std::exception\n");
        return 1;
    }
}
