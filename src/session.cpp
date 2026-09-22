#include "bridge/session.hpp"

#include <chrono>
#include <string>
#include <utility>

namespace steambridge {
namespace {

// ---------------------------------------------------------------------------
//  Tolerant readers.
// ---------------------------------------------------------------------------
//  A scenario is hand written, so a value can arrive as the "wrong" JSON kind -
//  an app id spelled as a string, say. Python's int()/str() coerced those; these
//  do the same, and fall back to the default instead of raising, so a typo in a
//  scenario cannot take the backend down in the middle of a run.

std::int64_t to_int64(const Json& value, std::int64_t fallback) noexcept {
    switch (value.kind()) {
        case Json::Kind::number: return value.as_int64();
        case Json::Kind::boolean: return value.as_bool() ? 1 : 0;
        case Json::Kind::string: {
            const std::string& text = value.as_string();
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
        default: return fallback;
    }
}

std::string to_text(const Json& value, const std::string& fallback) {
    switch (value.kind()) {
        case Json::Kind::string: return value.as_string();
        case Json::Kind::number:
            // Only an integral number has a text form worth reading as a name;
            // anything else is more likely a mistake than a name. The cast says
            // out loud what the comparison used to do implicitly.
            return static_cast<double>(value.as_int64()) == value.as_double()
                       ? std::to_string(value.as_int64())
                       : fallback;
        default: return fallback;
    }
}

std::string string_member(const Json& args, const char* key) {
    const Json* value = args.find(key);
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

Answer h_const_one(Session&, const Json&) { return from_state(Json::integer(1)); }

Answer h_install_path(Session& session, const Json&) {
    return from_state(Json::string(session.profile().install_path));
}

Answer h_steam_id(Session& session, const Json&) {
    return from_state(Json::integer(static_cast<std::int64_t>(session.profile().steam_id)));
}

Answer h_persona_name(Session& session, const Json&) {
    return from_state(Json::string(session.profile().persona_name));
}

Answer h_app_id(Session& session, const Json&) {
    return from_state(Json::integer(session.profile().app_id));
}

Answer h_language(Session& session, const Json&) {
    return from_state(Json::string(session.profile().language));
}

Answer h_ui_language(Session& session, const Json&) {
    return from_state(Json::string(session.profile().ui_language));
}

Answer h_seconds_since_active(Session& session, const Json&) {
    const auto elapsed = std::chrono::system_clock::now() - session.started();
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
    return from_state(Json::integer(static_cast<std::int64_t>(seconds)));
}

Answer h_server_real_time(Session&, const Json&) {
    // Steam hands out UTC seconds; a game only ever uses it for clock sanity.
    const auto now = std::chrono::system_clock::now().time_since_epoch();
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now).count();
    return from_state(Json::integer(static_cast<std::int64_t>(seconds)));
}

Answer h_build_id(Session& session, const Json&) {
    return from_state(Json::integer(session.profile().build_id));
}

Answer h_true(Session&, const Json&) { return from_state(Json::boolean(true)); }

Answer h_get_stat(Session& session, const Json& args) {
    const std::int64_t* value = session.profile().find_stat(string_member(args, "pchName"));
    if (value == nullptr) {
        // Steam reports failure for a name it does not know, and leaves the
        // caller's variable alone - which is exactly what we do here too.
        return from_state(Json::boolean(false));
    }
    Json out = Json::object();
    out.set("pData", Json::integer(*value));
    return from_state_out(Json::boolean(true), std::move(out));
}

Answer h_set_stat(Session& session, const Json& args) {
    // Setting an unknown name is accepted and remembered, so a game can invent a
    // stat locally without the scenario having listed it first.
    const std::string key = string_member(args, "pchName");
    const Json* value = args.find("nData");
    const std::int64_t number = value != nullptr ? to_int64(*value, 0) : 0;
    session.profile().set_stat(key, number);
    session.note_stat_written(key, number);
    return from_state(Json::boolean(true));
}

Answer h_get_achievement(Session& session, const Json& args) {
    const int index = session.profile().achievement_index(string_member(args, "pchName"));
    if (index < 0) {
        return from_state(Json::boolean(false));
    }
    Json out = Json::object();
    out.set(
        "pbAchieved",
        Json::boolean(session.profile().achievements[static_cast<std::size_t>(index)].achieved));
    return from_state_out(Json::boolean(true), std::move(out));
}

Answer h_set_achievement(Session& session, const Json& args) {
    const std::string name = string_member(args, "pchName");
    Achievement* achievement = session.profile().find_achievement(name);
    if (achievement == nullptr) {
        return from_state(Json::boolean(false));
    }
    achievement->achieved = true;
    session.note_achievement_set(name);
    return from_state(Json::boolean(true));
}

Answer h_num_achievements(Session& session, const Json&) {
    return from_state(
        Json::integer(static_cast<std::int64_t>(session.profile().achievements.size())));
}

Answer h_achievement_name(Session& session, const Json& args) {
    const Json* requested = args.find("iAchievement");
    const std::int64_t index = requested != nullptr ? to_int64(*requested, 0) : 0;
    if (index < 0 || index >= static_cast<std::int64_t>(session.profile().achievements.size())) {
        return from_state(Json::string(""));
    }
    return from_state(
        Json::string(session.profile().achievements[static_cast<std::size_t>(index)].name));
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
    {"SteamAPI_ISteamUtils_GetAppID", &h_app_id},
    {"SteamAPI_ISteamUtils_GetCurrentGameLanguage", &h_language},
    {"SteamAPI_ISteamUtils_GetSteamUILanguage", &h_ui_language},
    {"SteamAPI_ISteamUtils_GetSecondsSinceAppActive", &h_seconds_since_active},
    {"SteamAPI_ISteamUtils_GetServerRealTime", &h_server_real_time},
    {"SteamAPI_ISteamApps_GetAppBuildId", &h_build_id},
    {"SteamAPI_ISteamUserStats_RequestCurrentStats", &h_true},
    {"SteamAPI_ISteamUserStats_StoreStats", &h_true},
    {"SteamAPI_ISteamUserStats_GetStatInt32", &h_get_stat},
    {"SteamAPI_ISteamUserStats_SetStatInt32", &h_set_stat},
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

    if (const Json* value = data.find("app_id")) {
        profile.app_id = to_int64(*value, profile.app_id);
    }
    if (const Json* value = data.find("steam_id")) {
        profile.steam_id = static_cast<std::uint64_t>(
            to_int64(*value, static_cast<std::int64_t>(profile.steam_id)));
    }
    if (const Json* value = data.find("persona_name")) {
        profile.persona_name = to_text(*value, profile.persona_name);
    }
    if (const Json* value = data.find("language")) {
        profile.language = to_text(*value, profile.language);
    }
    if (const Json* value = data.find("ui_language")) {
        profile.ui_language = to_text(*value, profile.ui_language);
    }
    if (const Json* value = data.find("install_path")) {
        profile.install_path = to_text(*value, profile.install_path);
    }
    if (const Json* value = data.find("build_id")) {
        profile.build_id = to_int64(*value, profile.build_id);
    }

    if (const Json* value = data.find("stats"); value != nullptr && value->is_object()) {
        for (const auto& member : value->members()) {
            profile.stats.emplace_back(member.first, to_int64(member.second, 0));
        }
    }

    if (const Json* value = data.find("achievements"); value != nullptr && value->is_array()) {
        for (const Json& entry : value->items()) {
            if (!entry.is_object()) {
                continue;
            }
            Achievement achievement;
            if (const Json* entry_name = entry.find("name")) {
                achievement.name = to_text(*entry_name, std::string());
            }
            if (const Json* achieved = entry.find("achieved")) {
                achievement.achieved = achieved->as_bool();
            }
            profile.achievements.push_back(std::move(achievement));
        }
    }

    if (const Json* value = data.find("scripted"); value != nullptr && value->is_object()) {
        for (const auto& member : value->members()) {
            profile.scripted.emplace_back(member.first, member.second);
        }
    }

    return profile;
}

std::int64_t* Profile::find_stat(const std::string& key) noexcept {
    for (auto& entry : stats) {
        if (entry.first == key) {
            return &entry.second;
        }
    }
    return nullptr;
}

const std::int64_t* Profile::find_stat(const std::string& key) const noexcept {
    for (const auto& entry : stats) {
        if (entry.first == key) {
            return &entry.second;
        }
    }
    return nullptr;
}

void Profile::set_stat(const std::string& key, std::int64_t value) {
    if (std::int64_t* existing = find_stat(key)) {
        *existing = value;
        return;
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
    for (const auto& entry : scripted) {
        if (entry.first == call) {
            return &entry.second;
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
        if (const Json* pid = hello.find("pid")) {
            _pid = to_int64(*pid, 0);
        }
        if (const Json* exe = hello.find("exe")) {
            _exe = exe->is_string() ? exe->as_string() : std::string();
        }
        if (const Json* arch = hello.find("arch")) {
            _arch = arch->is_string() ? arch->as_string() : std::string();
        }
    }
}

void Session::note_stat_written(const std::string& key, std::int64_t value) {
    for (auto& entry : _stats_written) {
        if (entry.first == key) {
            entry.second = value;
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
    names.reserve(sizeof(kHandlers) / sizeof(kHandlers[0]));
    for (const HandlerEntry& entry : kHandlers) {
        names.emplace_back(entry.name);
    }
    return names;
}

}  // namespace steambridge
