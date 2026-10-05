#include "bridge/scenario.hpp"

#include <algorithm>
#include <cstdio>
#include <string>
#include <utility>

namespace steammock
{
namespace
{

// How long a scenario may say an answer takes. A minute is not a service level - it is the
// point past which a delay stops being something a game can be watched through.
constexpr std::int64_t kMaxDelayMs = 60000;

std::string lower_ascii(std::string text)
{
    for (char& ch : text)
    {
        if (ch >= 'A' && ch <= 'Z')
        {
            ch = static_cast<char>(ch - 'A' + 'a');
        }
    }
    return text;
}

std::string text_of(const Json& value)
{
    return value.is_string() ? as_string(value) : std::string();
}

bool read_file(const std::string& path, std::string& out)
{
    std::FILE* file = std::fopen(path.c_str(), "rb");
    if (file == nullptr)
    {
        return false;
    }
    char buffer[4096];
    std::size_t got = 0;
    while ((got = std::fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        out.append(buffer, got);
    }
    std::fclose(file);
    return true;
}

} // namespace

std::string MatchRule::describe() const
{
    std::string text = "{";
    bool first = true;
    const auto comma = [&]()
    {
        if (!first)
        {
            text += ", ";
        }
        first = false;
    };
    if (has_exe_contains)
    {
        comma();
        text += "\"exe_contains\": " + Json(exe_contains).dump();
    }
    if (has_exe)
    {
        comma();
        text += "\"exe\": " + Json(exe).dump();
    }
    if (has_pid)
    {
        comma();
        text += "\"pid\": " + std::to_string(pid);
    }
    if (!profile.empty())
    {
        comma();
        text += "\"profile\": " + Json(profile).dump();
    }
    text += "}";
    return text;
}

bool Dispatcher::load_file(const std::string& path, Dispatcher& out, std::string& error)
{
    std::string text;
    if (!read_file(path, text))
    {
        error = "cannot read " + path;
        return false;
    }
    Json scenario;
    if (!parse(text, scenario) || !scenario.is_object())
    {
        error = path + " is not a JSON object";
        return false;
    }
    out.configure(scenario, &error);
    return true;
}

void Dispatcher::configure(const Json& scenario, std::string* error)
{
    _profiles.clear();
    _match.clear();
    _overrides.clear_all();
    _default_profile = "default";
    _error.clear();

    if (!scenario.is_object())
    {
        _profiles.emplace_back("default", Profile{});
        return;
    }

    if (const Json* profiles = json_member(scenario, "profiles");
        profiles != nullptr && profiles->is_object())
    {
        for (const auto& [profile_name, profile_json] : profiles->items())
        {
            if (profile_json.is_object())
            {
                _profiles.emplace_back(profile_name,
                                       Profile::from_json(profile_name, profile_json));
            }
        }
    }
    if (find_profile("default") == nullptr)
    {
        _profiles.emplace_back("default", Profile{});
    }

    // A friend written as a name is a profile in this same file, filled in here rather than when
    // it was read, because this is the first moment every profile exists. A name that is not one
    // is a scenario that cannot mean what it says, and it is refused rather than dropped, the
    // same way a `default_profile` that is not there is refused rather than substituted.
    for (auto& [profile_name, profile] : _profiles)
    {
        for (Friend& friend_entry : profile.friends)
        {
            if (friend_entry.profile.empty())
            {
                continue; // an id and a name spelled out in the file
            }
            const Profile* other = find_profile(friend_entry.profile);
            if (other == nullptr)
            {
                _error = profile_name + " is friends with '" + friend_entry.profile +
                         "', which is not a profile in this scenario";
                if (error != nullptr)
                {
                    *error = _error;
                }
                // Everything goes, the default profile included: a table with no profiles at all
                // refuses every game, which is louder than the blank identity a leftover default
                // would happily run as.
                _profiles.clear();
                _match.clear();
                return;
            }
            friend_entry.steam_id = other->steam_id;
            friend_entry.persona_name = other->persona_name;
        }
    }

    if (const Json* default_profile = json_member(scenario, "default_profile");
        default_profile != nullptr && default_profile->is_string())
    {
        _default_profile = as_string(*default_profile);
    }

    if (const Json* match = json_member(scenario, "match"); match != nullptr && match->is_array())
    {
        for (const Json& entry : *match)
        {
            if (!entry.is_object())
            {
                continue;
            }
            MatchRule rule;
            if (const Json* value = json_member(entry, "exe_contains");
                value != nullptr && value->is_string())
            {
                rule.has_exe_contains = true;
                rule.exe_contains = as_string(*value);
            }
            if (const Json* value = json_member(entry, "exe");
                value != nullptr && value->is_string())
            {
                rule.has_exe = true;
                rule.exe = as_string(*value);
            }
            if (const Json* value = json_member(entry, "pid");
                value != nullptr && value->is_number())
            {
                rule.has_pid = true;
                rule.pid = as_int64(*value);
            }
            if (const Json* value = json_member(entry, "profile");
                value != nullptr && value->is_string())
            {
                rule.profile = as_string(*value);
            }
            _match.push_back(std::move(rule));
        }
    }

    // The `overrides` block: what a call answers from now on, whatever the profile and the
    // session would have said. Each entry may name the identity it answers for, so a scenario can
    // describe one game's failure as well as the run's. Refused rather than skipped when the
    // block is not an object, and each entry with it, because a scenario that cannot mean what it
    // says is the mistake this whole file exists to report at startup.
    if (const Json* overrides = json_member(scenario, "overrides"); overrides != nullptr)
    {
        if (!overrides->is_object())
        {
            _error = "overrides is not an object: one entry per call name, such as "
                     "\"SteamAPI_Init\": {\"ret\": false}";
            if (error != nullptr)
            {
                *error = _error;
            }
            _profiles.clear();
            _match.clear();
            _overrides.clear_all();
            return;
        }
        for (const auto& [call, entry] : overrides->items())
        {
            // An entry that says nothing is the file saying nothing twice: leaving the call
            // out gets the same run, and an entry with no `ret`, no `out` and no `then` is
            // empty on purpose by nobody.
            if (!entry.is_object() || entry.empty())
            {
                _error = "overrides." + call + " says nothing: an entry carries ret, out or then";
                if (error != nullptr)
                {
                    *error = _error;
                }
                _profiles.clear();
                _match.clear();
                _overrides.clear_all();
                return;
            }
            // `for` is the one field that is not part of the answer: it names whose call this is,
            // and it is read here rather than by the answer so that an entry in force carries
            // `ret`, `out` and `then` however it was written.
            std::string scope;
            if (const Json* target = json_member(entry, "for"); target != nullptr)
            {
                if (!target->is_string())
                {
                    _error = "overrides." + call + ".for is not a profile name: it is a string, "
                                                   "such as \"second_player\"";
                    if (error != nullptr)
                    {
                        *error = _error;
                    }
                    _profiles.clear();
                    _match.clear();
                    _overrides.clear_all();
                    return;
                }
                scope = as_string(*target);
            }
            Json answer = entry;
            answer.erase("for");
            if (answer.empty())
            {
                _error = "overrides." + call + " says nothing but which game it is for";
                if (error != nullptr)
                {
                    *error = _error;
                }
                _profiles.clear();
                _match.clear();
                _overrides.clear_all();
                return;
            }
            _overrides.set(scope, call, std::move(answer));
        }
    }
}

const Json* Dispatcher::override_for(const std::string& profile,
                                     const std::string& call) const noexcept
{
    return _overrides.find(profile, call);
}

std::vector<std::string> Dispatcher::profile_names() const
{
    std::vector<std::string> names;
    names.reserve(_profiles.size());
    for (const auto& entry : _profiles)
    {
        names.push_back(entry.first);
    }
    std::sort(names.begin(), names.end());
    return names;
}

const Profile* Dispatcher::find_profile(const std::string& name) const noexcept
{
    for (const auto& [profile_name, profile] : _profiles)
    {
        if (profile_name == name)
        {
            return &profile;
        }
    }
    return nullptr;
}

bool Dispatcher::has_profile(const std::string& name) const
{
    return find_profile(name) != nullptr;
}

std::optional<Profile> Dispatcher::profile_for(const Json& hello, std::string* refused) const
{
    std::string exe;
    std::int64_t pid = 0;
    std::string requested;
    if (hello.is_object())
    {
        if (const Json* value = json_member(hello, "exe"))
        {
            exe = lower_ascii(text_of(*value));
        }
        if (const Json* value = json_member(hello, "pid"); value != nullptr && value->is_number())
        {
            pid = as_int64(*value);
        }
        if (const Json* value = json_member(hello, "profile");
            value != nullptr && value->is_string())
        {
            requested = as_string(*value);
        }
    }

    // A game that asked for a profile by name gets it, before any match rule. Two
    // copies of the same exe are indistinguishable by name, and a pid cannot be
    // written into a scenario in advance - so this is how one file describes two
    // instances of the same game running side by side.
    //
    // A name the scenario does not have is a mistake and not a request for the
    // default: three clients asking for three names and receiving two identities have
    // been running as the same player, which from outside looks like a lobby holding
    // two members, auth responses delivered to a session nobody waits in, and a guest
    // whose ticket never arrives. This used to fall through to the default profile and
    // say nothing, and that silence is what made the whole thing take a day to find.
    if (!requested.empty())
    {
        if (const Profile* profile = find_profile(requested))
        {
            return *profile;
        }
        if (refused != nullptr)
        {
            *refused = requested;
        }
        return std::nullopt;
    }

    for (const MatchRule& rule : _match)
    {
        if (rule.has_exe_contains &&
            exe.find(lower_ascii(rule.exe_contains)) == std::string::npos)
        {
            continue;
        }
        if (rule.has_exe && lower_ascii(rule.exe) != exe)
        {
            continue;
        }
        if (rule.has_pid && rule.pid != pid)
        {
            continue;
        }
        const std::string wanted = rule.profile.empty() ? _default_profile : rule.profile;
        if (const Profile* profile = find_profile(wanted))
        {
            return *profile;
        }
        // A rule that matched the game and names a profile the scenario does not have is
        // the same silence one step further along: falling through to the next rule, or to
        // the default, runs a player nobody asked for while every run still looks
        // plausible. It is the half of the bug that took a day to find that a name asked
        // for by name does not cover, so it is refused in the same way.
        if (refused != nullptr)
        {
            *refused = wanted;
        }
        return std::nullopt;
    }

    if (const Profile* profile = find_profile(_default_profile))
    {
        return *profile;
    }
    // A default the scenario does not have is the same silence one step further along: it
    // used to answer with a default-constructed Profile, so a game ran as a blank identity -
    // the app id nobody set, the persona nobody chose - while every run still looked
    // plausible. That is the failure this whole function refuses for a name asked for by
    // name and for a rule that named one, and there is no reason for the third way in to be
    // the one that stays quiet.
    if (refused != nullptr)
    {
        *refused = _default_profile;
    }
    return std::nullopt;
}

std::int64_t Dispatcher::delay_for(const Session& session, const std::string& name) const
{
    const Json* scripted = session.profile().scripted_for(name);
    if (scripted == nullptr || !scripted->is_object())
    {
        return 0;
    }
    const Json* delay = json_member(*scripted, "delay_ms");
    if (delay == nullptr || !delay->is_number())
    {
        return 0;
    }
    const std::int64_t wanted = as_int64(*delay);
    if (wanted <= 0)
    {
        return 0;
    }
    // Clamped, because a delay is a condition to test a game against and not a way to park a
    // run: a scenario asking for an hour is a scenario with a typo, and a backend that has
    // stopped answering for an hour is not a state anybody can read evidence out of.
    return wanted > kMaxDelayMs ? kMaxDelayMs : wanted;
}

Answer Dispatcher::answer_from_entry(const Json& scripted, const std::string& via) const
{
    Answer answer;
    answer.via = via;
    if (!scripted.is_object())
    {
        // An entry that is not an object cannot say anything, so it
        // declines - the same as an explicit "answer": "default".
        return answer;
    }
    const Json* mode = json_member(scripted, "answer");
    if (mode != nullptr && mode->is_string() && as_string(*mode) == "default")
    {
        return answer;
    }
    answer.answered = true;
    if (const Json* ret = json_member(scripted, "ret"))
    {
        answer.ret = *ret;
    }
    if (const Json* out = json_member(scripted, "out"))
    {
        answer.out = *out;
    }
    // A `then` list is what should happen to the game once this answer is on
    // its way: each entry names a payload and the fields to write into it, and
    // the call it completes is the handle this entry just returned - so a
    // scenario says "create the lobby" once rather than twice.
    if (const Json* then = json_member(scripted, "then"); then != nullptr && then->is_array())
    {
        Json events = Json::array();
        for (const Json& entry : *then)
        {
            if (!entry.is_object())
            {
                continue;
            }
            Json event = entry;
            if (json_member(event, "call") == nullptr && json_member(event, "id") == nullptr)
            {
                if (!answer.ret.is_number())
                {
                    // Nothing to route this by. A `then` entry is how a scenario says
                    // "and now complete the call I just returned" - so an entry with
                    // no call, no id and a return value that is not a handle has no
                    // one to complete, and the payload would be delivered to whoever
                    // registered for the event's own default id: some object that was
                    // waiting for something else, or nobody at all. It is not sent.
                    continue;
                }
                event["call"] = answer.ret;
            }
            events.push_back(std::move(event));
        }
        if (!events.empty())
        {
            answer.events = std::move(events);
        }
    }
    return answer;
}

Answer Dispatcher::answer(Session& session, const std::string& name, const Json& args) const
{
    if (const Json* scripted = session.profile().scripted_for(name))
    {
        return answer_from_entry(*scripted, "scripted");
    }

    Answer answer = session.handle(name, args);
    if (answer.answered)
    {
        return answer;
    }
    answer.via = "none";
    return answer;
}

} // namespace steammock
