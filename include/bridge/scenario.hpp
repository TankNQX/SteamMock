#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <tuple>
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
//  1. the scenario's own `overrides` block, which is how a testing scenario
//     states what it is testing                                 (via: override)
//  2. a `scripted` entry for that call in the game's profile   (via: scripted)
//  3. the session state machine - identity, stats, achievements (via: state)
//  4. nobody: the backend reports "no opinion", and the stub uses its own
//     default                                                   (via: none)
//
//  That last line is the important one. A call nobody answers behaves exactly as
//  it would with Steam not running, so a game cannot be handed a success it did
//  not ask a scenario for - and the transcript always says which of the five
//  happened.
//
//  Both overrides sit above the profile's `scripted` entries, because pretending a
//  call failed is what an override is for and a `scripted` entry cannot do that in
//  the face of a world that answers the call itself. The two differ in who wrote
//  them: rung 1 is the file a person hands to somebody else, and rung 0 is the same
//  thing typed while the run is going. The live one wins, so an override that works
//  at the keyboard can be written down afterwards.
//
//  Every override also says which games it answers *for*, because a run has several
//  and one of them failing is usually the point. A scenario's entry carries
//  `"for": "second_player"` and the live view names the same thing in its scope
//  chooser; an entry with no scope answers every game. Rungs 0 and 1 therefore hold
//  four entries between them, and the order they are asked in is:
//
//    this game's live entry, the live entry for every game, this game's file entry,
//    the file's entry for every game
//
//  A scope does not move an entry between the rungs: what the keyboard wrote still
//  speaks before the file, which is the rule above and not a second one.
//
//  There is deliberately no "arbitrary code as a hook" rung any more. The Python
//  backend had one; it is the only thing that could not survive the move to C++,
//  and a scenario that says what it means is easier to hand to someone else than
//  a lambda buried in a script. An override is the replacement: a `scripted` entry
//  answered by the same code as one read from a file, which the file itself can
//  carry. Rung 0 is the *backend's* rather than this class's, because it is the
//  live view that holds the running process; the backend asks for rung 1 through
//  `override_for`.

// ---------------------------------------------------------------------------
//  The overrides in force, and which game each of them answers for.
// ---------------------------------------------------------------------------
//  Both rungs above are this container - the file's block and the live view's own set - so the two
//  cannot come to disagree about what a scoped entry means.
//
//  A scope is a profile name: what a run names its players by, what the live view's Games table
//  shows, and what a scenario already spells elsewhere in the file. The empty name is the scope
//  that answers every game in the run.
class Overrides
{
  public:
    // One entry as a person reads it: the identity it answers for - empty for every game in the
    // run - the call it answers, and the entry itself.
    using Entry = std::tuple<std::string, std::string, Json>;
    using List = std::vector<Entry>;
    // The entry `call` answers with for a game on `profile`: that identity's own if it has one,
    // then the entry written for every game, then nullptr - which is "nothing here says anything
    // about this call" rather than an answer.
    const Json* find(const std::string& profile, const std::string& call) const noexcept
    {
        const Json* scoped = find_in(profile, call);
        return scoped != nullptr ? scoped : find_in(std::string(), call);
    }

    // An entry that says nothing means the same as no entry: `{}` is a call nothing can be read
    // out of, and typing it in the live view means the same as blanking the box.
    void set(const std::string& profile, const std::string& call, Json entry)
    {
        if (entry.is_object() && entry.empty())
        {
            clear(profile, call);
            return;
        }
        _by_profile[profile][call] = std::move(entry);
    }

    void clear(const std::string& profile, const std::string& call)
    {
        const auto scoped = _by_profile.find(profile);
        if (scoped == _by_profile.end())
        {
            return;
        }
        scoped->second.erase(call);
        if (scoped->second.empty())
        {
            // A scope left behind with nothing in it would put a profile in the live view's
            // chooser that has nothing set for it.
            _by_profile.erase(scoped);
        }
    }

    void clear_all() { _by_profile.clear(); }

    std::size_t size() const noexcept
    {
        std::size_t total = 0;
        for (const auto& scope : _by_profile)
        {
            total += scope.second.size();
        }
        return total;
    }

    // Every entry in force, as (profile, call, entry), copied out: the caller is a window that
    // draws what it is told while calls are still arriving, and handing it the container itself
    // would be a copy of the map taken under a lock nobody took.
    List entries() const
    {
        List all;
        all.reserve(size());
        for (const auto& [profile, by_call] : _by_profile)
        {
            for (const auto& [call, entry] : by_call)
            {
                all.emplace_back(profile, call, entry);
            }
        }
        return all;
    }

  private:
    const Json* find_in(const std::string& profile, const std::string& call) const noexcept
    {
        const auto scoped = _by_profile.find(profile);
        if (scoped == _by_profile.end())
        {
            return nullptr;
        }
        const auto found = scoped->second.find(call);
        return found == scoped->second.end() ? nullptr : &found->second;
    }

    // Profile first: the empty name is the scope that answers every game.
    std::map<std::string, std::map<std::string, Json>> _by_profile;
};

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

    // The `overrides` block a scenario declares, which is rung 1 of the order above. It is
    // asked rather than answered here because the rung has to sit above the worlds, and the
    // worlds are the backend's: `Server` asks for one before it asks any of them.
    //
    // Null when the file says nothing about this call for a game on `profile`, which is the empty
    // name for a game the scenario has no profile of its own for. There is no lock and no copy:
    // the block is read once, in `configure`, and never written again.
    const Json* override_for(const std::string& profile, const std::string& call) const noexcept;

    // How long a scripted entry says this call should take to answer; 0 for every call that
    // does not say, which is every call in every scenario written so far. It is asked
    // separately from `answer` because it has to be known *before* the call is resolved: a
    // delay like this is what a slow backend looks like to the game, so the round trip is what
    // has to take the time - see Server::handle_call, which waits outside its state lock.
    std::int64_t delay_for(const Session& session, const std::string& name) const;

    const std::vector<MatchRule>& match_rules() const noexcept { return _match; }
    const std::string& default_profile() const noexcept { return _default_profile; }

    // Every entry the `overrides` block declares, as (profile, call, entry): what a window draws
    // beside the overrides set at the keyboard, so both are visible without reading the file.
    Overrides::List overrides() const;

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
    // The `overrides` block, by the identity each entry answers for. Unlike a profile's `scripted`
    // entries these are not read out of the profile the game was matched to: what is being tested
    // is the call, so an entry with no `for` means it for every game in the run and
    // `"for": "second_player"` means it for that identity alone.
    Overrides _overrides;
    std::string _default_profile = "default";
    std::string _error;
};

} // namespace steammock
