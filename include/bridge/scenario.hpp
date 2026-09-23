#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "bridge/json.hpp"
#include "bridge/session.hpp"

namespace steammock {

// ---------------------------------------------------------------------------
//  Turning a scenario file plus a session into an answer.
// ---------------------------------------------------------------------------
//  Resolution order for every call, first one that speaks wins:
//
//  1. a `scripted` entry for that call in the game's profile   (via: scripted)
//  2. the session state machine - identity, stats, achievements (via: state)
//  3. nobody: the backend reports "no opinion", and the stub uses its own
//     default                                                   (via: none)
//
//  That last line is the important one. A call nobody answers behaves exactly as
//  it would with Steam not running, so a game cannot be handed a success it did
//  not ask a scenario for - and the transcript always says which of the three
//  happened.
//
//  There is deliberately no "arbitrary code as a hook" rung any more. The Python
//  backend had one; it is the only thing that could not survive the move to C++,
//  and a scenario that says what it means is easier to hand to someone else than
//  a lambda buried in a script. Live editing of the same structures from a GUI is
//  the replacement.

// One rule that picks a profile for a connecting process. Rules are tried in
// order and the first match wins; a rule can look at the executable name or the
// process id.
struct MatchRule {
    bool has_exe_contains = false;
    std::string exe_contains;
    bool has_exe = false;
    std::string exe;
    bool has_pid = false;
    std::int64_t pid = 0;
    std::string profile;  // empty means "the default profile"

    std::string describe() const;
};

class Dispatcher {
public:
    // A scenario with no name gives a dispatcher that knows only `default`, which
    // is what every call then falls back to.
    Dispatcher() = default;
    explicit Dispatcher(const Json& scenario) { configure(scenario); }

    // Reads a scenario file. On failure `error` says what went wrong and the
    // dispatcher is left empty rather than half-configured.
    static bool load_file(const std::string& path, Dispatcher& out, std::string& error);

    std::vector<std::string> profile_names() const;
    bool has_profile(const std::string& name) const;

    // Picks the profile a connecting process should get. The profile is returned
    // by value, and every Session keeps its own copy: two games matched to the
    // same profile must not share stats.
    Profile profile_for(const Json& hello) const;

    Answer answer(Session& session, const std::string& name, const Json& args) const;

    const std::vector<MatchRule>& match_rules() const noexcept { return _match; }
    const std::string& default_profile() const noexcept { return _default_profile; }

private:
    void configure(const Json& scenario);

    const Profile* find_profile(const std::string& name) const noexcept;

    std::vector<std::pair<std::string, Profile>> _profiles;
    std::vector<MatchRule> _match;
    std::string _default_profile = "default";
};

}  // namespace steammock
