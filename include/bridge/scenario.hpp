#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "bridge/json_read.hpp"
#include "bridge/session.hpp"

namespace steammock
{

// ---------------------------------------------------------------------------
//  Turning a scenario file plus a session into an answer.
// ---------------------------------------------------------------------------
//  Resolution order for every call, first one that speaks wins:
//
//  0. a live override set from the live view - a `scripted` entry written
//     while the process runs                                     (via: live)
//  1. a `scripted` entry for that call in the game's profile   (via: scripted)
//  2. the session state machine - identity, stats, achievements (via: state)
//  3. nobody: the backend reports "no opinion", and the stub uses its own
//     default                                                   (via: none)
//
//  That last line is the important one. A call nobody answers behaves exactly as
//  it would with Steam not running, so a game cannot be handed a success it did
//  not ask a scenario for - and the transcript always says which of the four
//  happened.
//
//  There is deliberately no "arbitrary code as a hook" rung any more. The Python
//  backend had one; it is the only thing that could not survive the move to C++,
//  and a scenario that says what it means is easier to hand to someone else than
//  a lambda buried in a script. The live view is the replacement: an override is
//  a `scripted` entry entered while the process runs, answered by the same code
//  as one read from a file, and reported under its own rung so no transcript
//  mistakes it for the scenario. Rung 0 is the *backend's* rather than this
//  class's - the live view hands the entry to Server, which is what holds the
//  running process, and Server calls `answer_from_entry` with it.

// One rule that picks a profile for a connecting process. Rules are tried in
// order and the first match wins; a rule can look at the executable name or the
// process id.
struct MatchRule
{
    bool has_exe_contains = false;
    std::string exe_contains;
    bool has_exe = false;
    std::string exe;
    bool has_pid = false;
    std::int64_t pid = 0;
    std::string profile; // empty means "the default profile"

    std::string describe() const;
};

class Dispatcher
{
  public:
    // A scenario with no name gives a dispatcher that knows only `default`, which
    // is what every call then falls back to. That is what the comment always said and
    // what the code did not do: `= default` left the profile list empty, and `profile_for`
    // answered out of a default-constructed Profile at the last moment instead. The blank
    // identity was real, but it arrived by a route that could not be told apart from a
    // scenario naming a `default_profile` it does not have - which is a typo, and is
    // refused now. Building the default the way an empty scenario is built keeps the two
    // meanings apart: no scenario at all has a default, and a scenario that names one it
    // does not have does not.
    Dispatcher() { configure(Json()); }
    explicit Dispatcher(const Json& scenario) { configure(scenario); }

    // Reads a scenario file. On failure `error` says what went wrong and the
    // dispatcher is left empty rather than half-configured.
    static bool load_file(const std::string& path, Dispatcher& out, std::string& error);

    std::vector<std::string> profile_names() const;
    bool has_profile(const std::string& name) const;

    // Picks the profile a connecting process should get. The profile is returned
    // by value, and every Session keeps its own copy: two games matched to the
    // same profile must not share stats.
    //
    // Nothing comes back when the scenario cannot serve this game: a name the handshake
    // asked for and the scenario does not have, or one a matching rule names and it does
    // not have. That is a refusal rather than a substitution - the reason the three-client
    // bug took a day to find. `refused`, when given, is filled with the name that could not
    // be served, so a caller can say which one it was.
    std::optional<Profile> profile_for(const Json& hello, std::string* refused = nullptr) const;

    Answer answer(Session& session, const std::string& name, const Json& args) const;

    // Answers a call from an entry already in hand, exactly as a `scripted` entry in a
    // profile is answered. `via` is the rung the answer is reported under, so a live
    // override says "live" rather than borrowing the scenario's name, and `scripted` is
    // the entry to answer from - a profile's own, or a live override, which is written in
    // the same words on purpose. Overrides come through here so that an override and a
    // scenario entry cannot answer one call two different ways.
    Answer answer_from_entry(const Json& scripted, const std::string& via) const;

    // How long a scripted entry says this call should take to answer; 0 for every call that
    // does not say, which is every call in every scenario written so far. It is asked
    // separately from `answer` because it has to be known *before* the call is resolved: a
    // delay like this is what a slow backend looks like to the game, so the round trip is what
    // has to take the time - see Server::handle_call, which waits outside its state lock.
    std::int64_t delay_for(const Session& session, const std::string& name) const;

    const std::vector<MatchRule>& match_rules() const noexcept { return _match; }
    const std::string& default_profile() const noexcept { return _default_profile; }

    // What a scenario got wrong, empty when it loaded. It is reported by `load_file` as well,
    // so a file that cannot be served says why at startup rather than only refusing games.
    const std::string& load_error() const noexcept { return _error; }

  private:
    // `error`, when given, is filled in with what a scenario got wrong and the dispatcher is left
    // as the empty one rather than half-configured - the same refusal `profile_for` makes at
    // match time, made here instead because a friends list naming somebody is checkable at load.
    void configure(const Json& scenario, std::string* error = nullptr);

    const Profile* find_profile(const std::string& name) const noexcept;

    std::vector<std::pair<std::string, Profile>> _profiles;
    std::vector<MatchRule> _match;
    std::string _default_profile = "default";
    std::string _error;
};

} // namespace steammock
