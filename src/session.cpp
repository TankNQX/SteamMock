#include "bridge/session.hpp"

#include <chrono>
#include <iterator>
#include <string>
#include <utility>

namespace steammock {
namespace {

// ---------------------------------------------------------------------------
//  Tolerant readers.
// ---------------------------------------------------------------------------
//  A scenario is hand written, so a value can arrive as the "wrong" JSON kind -
//  an app id spelled as a string, say. Python's int()/str() coerced those; these
//  do the same, and fall back to the default instead of raising, so a typo in a
//  scenario cannot take the backend down in the middle of a run.

// Not `noexcept`: reading a number out of a string allocates, and a `noexcept` here
// would turn a failed allocation into `std::terminate` instead of telling the caller.
std::int64_t to_int64(const Json& value, std::int64_t fallback) {
    if (value.is_number_unsigned()) {
        // An unsigned JSON integer at or above 2^63 has no int64 to be. It used to go
        // through as_int64, which for a value a JSON reader kept as unsigned is either
        // implementation-defined or a wrap into a negative number - and a negative app id
        // is not a reading of it. Everything else out of range here falls back, so this
        // does too.
        const std::uint64_t number = as_uint64(value, 0u);
        return number > static_cast<std::uint64_t>(9223372036854775807ll)
                   ? fallback
                   : static_cast<std::int64_t>(number);
    }
    if (value.is_number()) {
        return as_int64(value);
    }
    if (value.is_boolean()) {
        return as_bool(value) ? 1 : 0;
    }
    if (value.is_string()) {
        const std::string text = as_string(value);
        std::size_t index = 0;
        while (index < text.size() && (text[index] == ' ' || text[index] == '\t')) {
            ++index;
        }
        bool negative = false;
        if (index < text.size() && (text[index] == '+' || text[index] == '-')) {
            negative = text[index] == '-';
            ++index;
        }
        const std::size_t digits_begin = index;
        std::int64_t magnitude = 0;
        while (index < text.size() && text[index] >= '0' && text[index] <= '9') {
            if (magnitude > (9223372036854775807LL - (text[index] - '0')) / 10) {
                return fallback;
            }
            magnitude = magnitude * 10 + (text[index] - '0');
            ++index;
        }
        if (index == digits_begin) {
            return fallback;
        }
        while (index < text.size() && (text[index] == ' ' || text[index] == '\t')) {
            ++index;
        }
        if (index != text.size()) {
            return fallback;
        }
        return negative ? -magnitude : magnitude;
    }
    return fallback;
}

std::string to_text(const Json& value, const std::string& fallback) {
    if (value.is_string()) {
        return as_string(value);
    }
    if (value.is_number()) {
        // Only an integral number has a text form worth reading as a name;
        // anything else is more likely a mistake than a name. The cast says
        // out loud what the comparison used to do implicitly.
        return static_cast<double>(as_int64(value)) == as_double(value)
                   ? std::to_string(as_int64(value))
                   : fallback;
    }
    return fallback;
}

// A scenario's own boolean, read the way the other fields here are read: a value can
// arrive as the "wrong" JSON kind, so true/false, 1/0 and the words for them all count.
// `achieved` was the one field handed straight to as_bool, which answers false for
// anything that is not a boolean or a number - so an achievement the scenario wrote as
// "achieved": "true" was silently read as not earned, and nothing said so.
bool to_bool(const Json& value, bool fallback) {
    if (value.is_boolean()) {
        return as_bool(value);
    }
    if (value.is_number()) {
        return as_int64(value) != 0;
    }
    if (value.is_string()) {
        const std::string text = as_string(value);
        if (text == "true" || text == "yes" || text == "on") {
            return true;
        }
        if (text == "false" || text == "no" || text == "off") {
            return false;
        }
        // ...or a number spelled as text, which is what the tolerant readers below are for.
        return to_int64(value, fallback ? 1 : 0) != 0;
    }
    return fallback;
}

std::string string_member(const Json& args, const char* key) {
    // Deliberately not the shared as_string_member: a scenario is hand-written, so a
    // value arrives as the "wrong" JSON kind - an app id spelled as a string, a name
    // spelled as a number - and this coerces where the shared one would answer "".
    const Json* value = json_member(args, key);
    return value != nullptr ? to_text(*value, std::string()) : std::string();
}

Answer answered(Json ret) {
    Answer answer;
    answer.answered = true;
    answer.ret = std::move(ret);
    return answer;
}

Answer answered_with_out(Json ret, Json out) {
    Answer answer;
    answer.answered = true;
    answer.ret = std::move(ret);
    answer.out = std::move(out);
    return answer;
}

// Every state answer is "state"; the one place that changes it is the dispatcher,
// which relabels a scripted win.
constexpr const char* kStateVia = "state";

Answer from_state(Json ret) {
    Answer answer = answered(std::move(ret));
    answer.via = kStateVia;
    return answer;
}

Answer from_state_out(Json ret, Json out) {
    Answer answer = answered_with_out(std::move(ret), std::move(out));
    answer.via = kStateVia;
    return answer;
}

// --- the handlers ----------------------------------------------------------
// Each answers one call from the session's own state. A handler that does not
// recognise a name or an index reports Steam's failure value and leaves the
// caller's variable alone, which is what the stub's "no out-parameter" rule
// already does for us.

// The SDK's own enumerations and payload names written out, because this tree carries no Valve
// header and a value typed as a number is what the wire has anyway.
constexpr std::int64_t kResultOK = 1;  // k_EResultOK
constexpr const char* kUserStatsReceived = "UserStatsReceived_t";

Answer h_const_one(Session&, const Json&) { return from_state(Json(1)); }

Answer h_install_path(Session& session, const Json&) {
    return from_state(Json(session.profile().install_path));
}

Answer h_steam_id(Session& session, const Json&) {
    return from_state(Json(static_cast<std::int64_t>(session.profile().steam_id)));
}

Answer h_persona_name(Session& session, const Json&) {
    return from_state(Json(session.profile().persona_name));
}

// The friends list is the scenario's: it names the profiles this identity is friends with, and a
// game that draws a list of friends draws exactly that. The flags a real `GetFriendCount` takes
// choose between accepted friends, blocklist entries and co-play records, none of which exists
// here, so every declared friend is one a game is told about.
Answer h_friend_count(Session& session, const Json&) {
    return from_state(Json(static_cast<std::int64_t>(session.profile().friends.size())));
}

Answer h_friend_by_index(Session& session, const Json& args) {
    std::int64_t index = 0;
    if (const Json* value = json_member(args, "iFriend")) {
        index = to_int64(*value, 0);
    }
    const std::vector<Friend>& friends = session.profile().friends;
    if (index < 0 || static_cast<std::size_t>(index) >= friends.size()) {
        // Past the end of the list: no id rather than a name it does not have, which is what
        // ends a game's loop over the count.
        return from_state(Json(static_cast<std::uint64_t>(0)));
    }
    return from_state(Json(friends[static_cast<std::size_t>(index)].steam_id));
}

Answer h_friend_persona_name(Session& session, const Json& args) {
    std::uint64_t friend_id = 0;
    if (const Json* value = json_member(args, "steamIDFriend")) {
        friend_id = static_cast<std::uint64_t>(to_int64(*value, 0));
    }
    std::string name;
    if (!session.profile().find_friend(friend_id, name)) {
        // Somebody this profile is not friends with: the name a game is given for an id Steam
        // has nothing to say about is empty, which is what the stub answers too when nobody
        // has an opinion at all.
        return from_state(Json(std::string()));
    }
    return from_state(Json(name));
}

Answer h_app_id(Session& session, const Json&) {
    return from_state(Json(session.profile().app_id));
}

Answer h_language(Session& session, const Json&) {
    return from_state(Json(session.profile().language));
}

Answer h_ui_language(Session& session, const Json&) {
    return from_state(Json(session.profile().ui_language));
}

Answer h_seconds_since_active(Session& session, const Json&) {
    const auto elapsed = std::chrono::system_clock::now() - session.started();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
    if (seconds < 0) {
        // The session's start is wall-clock, because that is what it also means to
        // `sessions()` - connected_at_unix_ms is a system-clock reading, and it is the one
        // thing here that has to be. Wall-clock is not monotonic, so an NTP correction or
        // a clock change can put the start in the future and this difference negative,
        // which is a game told it has been active for minus four seconds. Clamped rather
        // than moved to a steady clock, which would make that snapshot meaningless.
        seconds = 0;
    }
    return from_state(Json(static_cast<std::int64_t>(seconds)));
}

Answer h_server_real_time(Session&, const Json&) {
    // Steam hands out UTC seconds; a game only ever uses it for clock sanity.
    const auto now = std::chrono::system_clock::now().time_since_epoch();
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now).count();
    return from_state(Json(static_cast<std::int64_t>(seconds)));
}

Answer h_build_id(Session& session, const Json&) {
    return from_state(Json(session.profile().build_id));
}

Answer h_true(Session&, const Json&) { return from_state(Json(true)); }

// The one call here whose answer is a *payload* rather than a value. A game asks for its stats
// and Steam says they have arrived: `UserStatsReceived_t`, whose own fields are the app id, the
// result and the player it is about - all of them this session's, which is why a scenario file
// cannot say it and this is where it comes from.
//
// It is not decoration: Spacewar's stats screen draws *nothing at all* - not even the inventory
// that is on it - until one of these has been handed to it, so a harness that answers `true` and
// stops there leaves that screen saying "Unable to retrieve data from Steam".
Answer h_request_current_stats(Session& session, const Json&) {
    Json fields = Json::object();
    fields["m_nGameID"] = Json(static_cast<std::uint64_t>(session.profile().app_id));
    fields["m_eResult"] = Json(kResultOK);
    fields["m_steamIDUser"] = Json(session.profile().steam_id);

    Json event = Json::object();
    event["event"] = Json(kUserStatsReceived);
    event["in"] = std::move(fields);

    Answer answer = from_state(Json(true));
    answer.events = Json::array();
    answer.events.push_back(std::move(event));
    return answer;
}

Answer h_get_stat(Session& session, const Json& args) {
    std::int64_t value = 0;
    if (!session.profile().find_stat(string_member(args, "pchName"), value)) {
        // Steam reports failure for a name it does not know, and leaves the
        // caller's variable alone - which is exactly what we do here too.
        return from_state(Json(false));
    }
    Json out = Json::object();
    out["pData"] = Json(value);
    return from_state_out(Json(true), std::move(out));
}

Answer h_set_stat(Session& session, const Json& args) {
    // Setting an unknown name is accepted and remembered, so a game can invent a
    // stat locally without the scenario having listed it first.
    const std::string key = string_member(args, "pchName");
    const Json* value = json_member(args, "nData");
    const std::int64_t number = value != nullptr ? to_int64(*value, 0) : 0;
    session.profile().set_stat(key, number);
    session.note_stat_written(key, number);
    return from_state(Json(true));
}

Answer h_get_achievement(Session& session, const Json& args) {
    const int index = session.profile().achievement_index(string_member(args, "pchName"));
    if (index < 0) {
        return from_state(Json(false));
    }
    Json out = Json::object();
    out["pbAchieved"] =
        Json(session.profile().achievements[static_cast<std::size_t>(index)].achieved);
    return from_state_out(Json(true), std::move(out));
}

Answer h_set_achievement(Session& session, const Json& args) {
    const std::string name = string_member(args, "pchName");
    Achievement* achievement = session.profile().find_achievement(name);
    if (achievement == nullptr) {
        return from_state(Json(false));
    }
    achievement->achieved = true;
    session.note_achievement_set(name);
    return from_state(Json(true));
}

Answer h_num_achievements(Session& session, const Json&) {
    return from_state(Json(static_cast<std::int64_t>(session.profile().achievements.size())));
}

Answer h_achievement_name(Session& session, const Json& args) {
    const Json* requested = json_member(args, "iAchievement");
    if (requested == nullptr) {
        // A call that does not say which achievement is a call with no index, and Steam
        // fails it and leaves the caller's buffer as it found it. This used to read the
        // missing field as index 0, so a game that asked without saying which was handed
        // the first achievement's name as if it had named it.
        return from_state(Json(""));
    }
    // Anything the field says that is not a number is not an index either, so it fails the
    // range check below the same way an out-of-range one does.
    const std::int64_t index = to_int64(*requested, -1);
    if (index < 0 || index >= static_cast<std::int64_t>(session.profile().achievements.size())) {
        return from_state(Json(""));
    }
    return from_state(Json(session.profile().achievements[static_cast<std::size_t>(index)].name));
}

// Calls answered from session state. Everything absent here is either scripted
// by the scenario or reported as "no opinion", which makes the stub fall back to
// the value it would use with Steam not running.
struct HandlerEntry {
    const char* name;
    Answer (*handler)(Session&, const Json&);
};

constexpr HandlerEntry kHandlers[] = {
    {"SteamAPI_GetHSteamUser", &h_const_one},
    {"SteamAPI_GetHSteamPipe", &h_const_one},
    {"SteamAPI_GetSteamInstallPath", &h_install_path},
    {"SteamAPI_ISteamUser_GetSteamID", &h_steam_id},
    {"SteamAPI_ISteamFriends_GetPersonaName", &h_persona_name},
    // The friends list this identity's scenario declares. A name the world can place comes from
    // the world first, so what is answered here is the friend a run has not heard from.
    {"SteamAPI_ISteamFriends_GetFriendCount", &h_friend_count},
    {"SteamAPI_ISteamFriends_GetFriendByIndex", &h_friend_by_index},
    {"SteamAPI_ISteamFriends_GetFriendPersonaName", &h_friend_persona_name},
    {"SteamAPI_ISteamUtils_GetAppID", &h_app_id},
    // The language a game is running in is ISteamApps' call in every SDK this has
    // been read from, and the layouts put that name on the wire: answering the
    // ISteamUtils spelling instead - which no SDK declares - is a call nobody can
    // make and a game that is told nothing.
    {"SteamAPI_ISteamApps_GetCurrentGameLanguage", &h_language},
    {"SteamAPI_ISteamUtils_GetSteamUILanguage", &h_ui_language},
    {"SteamAPI_ISteamUtils_GetSecondsSinceAppActive", &h_seconds_since_active},
    {"SteamAPI_ISteamUtils_GetServerRealTime", &h_server_real_time},
    {"SteamAPI_ISteamApps_GetAppBuildId", &h_build_id},
    {"SteamAPI_ISteamUserStats_RequestCurrentStats", &h_request_current_stats},
    {"SteamAPI_ISteamUserStats_StoreStats", &h_true},
    // The overloads are named after their types only from 1.51 on; every SDK read
    // here spells the integer one `GetStat`, and it is that flat name the layouts
    // travel under.
    {"SteamAPI_ISteamUserStats_GetStat", &h_get_stat},
    {"SteamAPI_ISteamUserStats_SetStat", &h_set_stat},
    {"SteamAPI_ISteamUserStats_GetAchievement", &h_get_achievement},
    {"SteamAPI_ISteamUserStats_SetAchievement", &h_set_achievement},
    {"SteamAPI_ISteamUserStats_GetNumAchievements", &h_num_achievements},
    {"SteamAPI_ISteamUserStats_GetAchievementName", &h_achievement_name},
};

}  // namespace

// ---------------------------------------------------------------------------
//  Profile
// ---------------------------------------------------------------------------

Profile Profile::from_json(const std::string& profile_name, const Json& data) {
    Profile profile;
    profile.name = profile_name;
    if (!data.is_object()) {
        return profile;
    }

    if (const Json* value = json_member(data, "app_id")) {
        profile.app_id = to_int64(*value, profile.app_id);
    }
    if (const Json* value = json_member(data, "steam_id")) {
        profile.steam_id = static_cast<std::uint64_t>(
            to_int64(*value, static_cast<std::int64_t>(profile.steam_id)));
    }
    if (const Json* value = json_member(data, "persona_name")) {
        profile.persona_name = to_text(*value, profile.persona_name);
    }
    if (const Json* value = json_member(data, "language")) {
        profile.language = to_text(*value, profile.language);
    }
    if (const Json* value = json_member(data, "ui_language")) {
        profile.ui_language = to_text(*value, profile.ui_language);
    }
    if (const Json* value = json_member(data, "install_path")) {
        profile.install_path = to_text(*value, profile.install_path);
    }
    if (const Json* value = json_member(data, "build_id")) {
        profile.build_id = to_int64(*value, profile.build_id);
    }

    if (const Json* value = json_member(data, "stats"); value != nullptr && value->is_object()) {
        for (const auto& [stat_name, stat_value] : value->items()) {
            profile.stats.emplace_back(stat_name, to_int64(stat_value, 0));
        }
    }

    if (const Json* value = json_member(data, "achievements");
        value != nullptr && value->is_array()) {
        for (const Json& entry : *value) {
            if (!entry.is_object()) {
                continue;
            }
            Achievement achievement;
            if (const Json* entry_name = json_member(entry, "name")) {
                achievement.name = to_text(*entry_name, std::string());
            }
            if (const Json* achieved = json_member(entry, "achieved")) {
                achievement.achieved = to_bool(*achieved, false);
            }
            profile.achievements.push_back(std::move(achievement));
        }
    }

    if (const Json* value = json_member(data, "friends"); value != nullptr && value->is_array()) {
        for (const Json& entry : *value) {
            if (entry.is_string()) {
                // A profile in the same scenario, by name. The id and the name are filled
                // in once every profile has been read, because that is the only moment
                // the file can be checked for naming somebody who is not in it.
                Friend friend_entry;
                friend_entry.profile = as_string(entry);
                profile.friends.push_back(std::move(friend_entry));
                continue;
            }
            if (!entry.is_object()) {
                continue;
            }
            // An id and a name spelled out: a friend this scenario has no profile for, which
            // is what a friend who is simply not in this run looks like.
            Friend friend_entry;
            if (const Json* id = json_member(entry, "steam_id")) {
                friend_entry.steam_id =
                    static_cast<std::uint64_t>(to_int64(*id, static_cast<std::int64_t>(0)));
            }
            if (const Json* name = json_member(entry, "persona_name")) {
                friend_entry.persona_name = to_text(*name, std::string());
            }
            profile.friends.push_back(std::move(friend_entry));
        }
    }

    if (const Json* value = json_member(data, "scripted"); value != nullptr && value->is_object()) {
        for (const auto& [call_name, script] : value->items()) {
            profile.scripted.emplace_back(call_name, script);
        }
    }

    return profile;
}

bool Profile::find_friend(std::uint64_t friend_id, std::string& out) const noexcept {
    for (const Friend& friend_entry : friends) {
        if (friend_entry.steam_id == friend_id) {
            out = friend_entry.persona_name;
            return true;
        }
    }
    return false;
}

bool Profile::find_stat(const std::string& key, std::int64_t& out) const noexcept {
    for (const auto& [stored_key, stored_value] : stats) {
        if (stored_key == key) {
            out = stored_value;
            return true;
        }
    }
    return false;
}

void Profile::set_stat(const std::string& key, std::int64_t value) {
    // The lookup and the write are the same loop rather than a call to find_stat() and a
    // write through what it returned: `emplace_back` below can reallocate `stats`, and a
    // pointer into it that outlived the lookup is exactly what used to be handed out here.
    for (auto& [stored_key, stored_value] : stats) {
        if (stored_key == key) {
            stored_value = value;
            return;
        }
    }
    stats.emplace_back(key, value);
}

int Profile::achievement_index(const std::string& achievement_name) const noexcept {
    for (std::size_t index = 0; index < achievements.size(); ++index) {
        if (achievements[index].name == achievement_name) {
            return static_cast<int>(index);
        }
    }
    return -1;
}

Achievement* Profile::find_achievement(const std::string& achievement_name) noexcept {
    const int index = achievement_index(achievement_name);
    return index < 0 ? nullptr : &achievements[static_cast<std::size_t>(index)];
}

const Json* Profile::scripted_for(const std::string& call) const noexcept {
    for (const auto& [call_name, script] : scripted) {
        if (call_name == call) {
            return &script;
        }
    }
    return nullptr;
}

// ---------------------------------------------------------------------------
//  Session
// ---------------------------------------------------------------------------

Session::Session(std::string id, const Json& hello, Profile profile)
    : _id(std::move(id)), _profile(std::move(profile)), _started(std::chrono::system_clock::now()) {
    if (hello.is_object()) {
        if (const Json* pid = json_member(hello, "pid")) {
            _pid = to_int64(*pid, 0);
        }
        if (const Json* exe = json_member(hello, "exe")) {
            _exe = exe->is_string() ? as_string(*exe) : std::string();
        }
        if (const Json* arch = json_member(hello, "arch")) {
            _arch = arch->is_string() ? as_string(*arch) : std::string();
        }
    }
}

void Session::note_stat_written(const std::string& key, std::int64_t value) {
    for (auto& [stored_key, stored_value] : _stats_written) {
        if (stored_key == key) {
            stored_value = value;
            return;
        }
    }
    _stats_written.emplace_back(key, value);
}

void Session::note_achievement_set(const std::string& name) {
    for (const std::string& existing : _achievements_set) {
        if (existing == name) {
            return;
        }
    }
    _achievements_set.push_back(name);
}

std::string Session::describe() const {
    return "session " + _id + " (" + (_exe.empty() ? std::string("unknown exe") : _exe) + ", pid " +
           std::to_string(_pid) + ", profile '" + _profile.name + "')";
}

Answer Session::handle(const std::string& name, const Json& args) {
    for (const HandlerEntry& entry : kHandlers) {
        if (name == entry.name) {
            return entry.handler(*this, args);
        }
    }
    return Answer{};
}

std::vector<std::string> state_handled_calls() {
    std::vector<std::string> names;
    names.reserve(std::size(kHandlers));
    for (const HandlerEntry& entry : kHandlers) {
        names.emplace_back(entry.name);
    }
    return names;
}

}  // namespace steammock
