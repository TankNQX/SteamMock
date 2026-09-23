#include "bridge/scenario.hpp"

#include <algorithm>
#include <cstdio>
#include <string>
#include <utility>

namespace steammock {
namespace {

std::string lower_ascii(std::string text) {
    for (char& ch : text) {
        if (ch >= 'A' && ch <= 'Z') {
            ch = static_cast<char>(ch - 'A' + 'a');
        }
    }
    return text;
}

std::string text_of(const Json& value) {
    return value.is_string() ? value.as_string() : std::string();
}

bool read_file(const std::string& path, std::string& out) {
    std::FILE* file = std::fopen(path.c_str(), "rb");
    if (file == nullptr) {
        return false;
    }
    char buffer[4096];
    std::size_t got = 0;
    while ((got = std::fread(buffer, 1, sizeof(buffer), file)) > 0) {
        out.append(buffer, got);
    }
    std::fclose(file);
    return true;
}

}  // namespace

std::string MatchRule::describe() const {
    std::string text = "{";
    bool first = true;
    const auto comma = [&]() {
        if (!first) {
            text += ", ";
        }
        first = false;
    };
    if (has_exe_contains) {
        comma();
        text += "\"exe_contains\": " + Json::string(exe_contains).dump();
    }
    if (has_exe) {
        comma();
        text += "\"exe\": " + Json::string(exe).dump();
    }
    if (has_pid) {
        comma();
        text += "\"pid\": " + std::to_string(pid);
    }
    if (!profile.empty()) {
        comma();
        text += "\"profile\": " + Json::string(profile).dump();
    }
    text += "}";
    return text;
}

bool Dispatcher::load_file(const std::string& path, Dispatcher& out, std::string& error) {
    std::string text;
    if (!read_file(path, text)) {
        error = "cannot read " + path;
        return false;
    }
    Json scenario;
    if (!Json::parse(text, scenario) || !scenario.is_object()) {
        error = path + " is not a JSON object";
        return false;
    }
    out.configure(scenario);
    return true;
}

void Dispatcher::configure(const Json& scenario) {
    _profiles.clear();
    _match.clear();
    _default_profile = "default";

    if (!scenario.is_object()) {
        _profiles.emplace_back("default", Profile{});
        return;
    }

    if (const Json* profiles = scenario.find("profiles");
        profiles != nullptr && profiles->is_object()) {
        for (const auto& member : profiles->members()) {
            if (member.second.is_object()) {
                _profiles.emplace_back(member.first,
                                       Profile::from_json(member.first, member.second));
            }
        }
    }
    if (find_profile("default") == nullptr) {
        _profiles.emplace_back("default", Profile{});
    }

    if (const Json* default_profile = scenario.find("default_profile");
        default_profile != nullptr && default_profile->is_string()) {
        _default_profile = default_profile->as_string();
    }

    if (const Json* match = scenario.find("match"); match != nullptr && match->is_array()) {
        for (const Json& entry : match->items()) {
            if (!entry.is_object()) {
                continue;
            }
            MatchRule rule;
            if (const Json* value = entry.find("exe_contains");
                value != nullptr && value->is_string()) {
                rule.has_exe_contains = true;
                rule.exe_contains = value->as_string();
            }
            if (const Json* value = entry.find("exe"); value != nullptr && value->is_string()) {
                rule.has_exe = true;
                rule.exe = value->as_string();
            }
            if (const Json* value = entry.find("pid"); value != nullptr && value->is_number()) {
                rule.has_pid = true;
                rule.pid = value->as_int64();
            }
            if (const Json* value = entry.find("profile"); value != nullptr && value->is_string()) {
                rule.profile = value->as_string();
            }
            _match.push_back(std::move(rule));
        }
    }
}

std::vector<std::string> Dispatcher::profile_names() const {
    std::vector<std::string> names;
    names.reserve(_profiles.size());
    for (const auto& entry : _profiles) {
        names.push_back(entry.first);
    }
    std::sort(names.begin(), names.end());
    return names;
}

const Profile* Dispatcher::find_profile(const std::string& name) const noexcept {
    for (const auto& entry : _profiles) {
        if (entry.first == name) {
            return &entry.second;
        }
    }
    return nullptr;
}

bool Dispatcher::has_profile(const std::string& name) const {
    return find_profile(name) != nullptr;
}

Profile Dispatcher::profile_for(const Json& hello) const {
    std::string exe;
    std::int64_t pid = 0;
    std::string requested;
    if (hello.is_object()) {
        if (const Json* value = hello.find("exe")) {
            exe = lower_ascii(text_of(*value));
        }
        if (const Json* value = hello.find("pid"); value != nullptr && value->is_number()) {
            pid = value->as_int64();
        }
        if (const Json* value = hello.find("profile"); value != nullptr && value->is_string()) {
            requested = value->as_string();
        }
    }

    // A game that asked for a profile by name gets it, before any match rule. Two
    // copies of the same exe are indistinguishable by name, and a pid cannot be
    // written into a scenario in advance - so this is how one file describes two
    // instances of the same game running side by side.
    if (!requested.empty()) {
        if (const Profile* profile = find_profile(requested)) {
            return *profile;
        }
    }

    for (const MatchRule& rule : _match) {
        if (rule.has_exe_contains &&
            exe.find(lower_ascii(rule.exe_contains)) == std::string::npos) {
            continue;
        }
        if (rule.has_exe && lower_ascii(rule.exe) != exe) {
            continue;
        }
        if (rule.has_pid && rule.pid != pid) {
            continue;
        }
        const std::string wanted = rule.profile.empty() ? _default_profile : rule.profile;
        if (const Profile* profile = find_profile(wanted)) {
            return *profile;
        }
    }

    if (const Profile* profile = find_profile(_default_profile)) {
        return *profile;
    }
    return Profile{};
}

Answer Dispatcher::answer(Session& session, const std::string& name, const Json& args) const {
    if (const Json* scripted = session.profile().scripted_for(name)) {
        Answer answer;
        answer.via = "scripted";
        if (!scripted->is_object()) {
            // A scripted entry that is not an object cannot say anything, so it
            // declines - the same as an explicit "answer": "default".
            return answer;
        }
        const Json* mode = scripted->find("answer");
        if (mode != nullptr && mode->is_string() && mode->as_string() == "default") {
            return answer;
        }
        answer.answered = true;
        if (const Json* ret = scripted->find("ret")) {
            answer.ret = *ret;
        }
        if (const Json* out = scripted->find("out")) {
            answer.out = *out;
        }
        // A `then` list is what should happen to the game once this answer is on
        // its way: each entry names a payload and the fields to write into it, and
        // the call it completes is the handle this entry just returned - so a
        // scenario says "create the lobby" once rather than twice.
        if (const Json* then = scripted->find("then"); then != nullptr && then->is_array()) {
            Json events = Json::array();
            for (const Json& entry : then->items()) {
                if (!entry.is_object()) {
                    continue;
                }
                Json event = entry;
                if (event.find("call") == nullptr && event.find("id") == nullptr &&
                    answer.ret.is_number()) {
                    event.set("call", answer.ret);
                }
                events.push(std::move(event));
            }
            if (!events.items().empty()) {
                answer.events = std::move(events);
            }
        }
        return answer;
    }

    Answer answer = session.handle(name, args);
    if (answer.answered) {
        return answer;
    }
    answer.via = "none";
    return answer;
}

}  // namespace steammock
