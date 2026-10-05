#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "bridge/json_read.hpp"

namespace steammock
{

// Defined in bridge/store.hpp, which includes this header: the store speaks about a
// Profile, so it cannot be included from here without a cycle. Nothing below needs more
// than the name.
class Store;

// ---------------------------------------------------------------------------
//  The per-game state machine.
// ---------------------------------------------------------------------------
//  One Session per connected game process. It mirrors the parts of Steam the
//  harness can model locally - identity, language, app id, stats and
//  achievements - so a game can boot, read, write and store as it normally
//  would, and so a transcript shows what it asked and what it was told.
//
//  Two things it deliberately does not do:
//
//  * answer policy calls (SteamAPI_Init, SteamAPI_IsSteamRunning, the interface
//    getters). Those decide whether a game believes Steam is present, so they
//    stay "no opinion" unless a scenario scripts them.
//  * answer anything about ownership, entitlement or licensing.

struct Achievement
{
    std::string name;
    bool achieved = false;
    // What a game draws the achievement with. Steam hands this text out per key through
    // `GetAchievementDisplayAttribute`, and the two keys every game asks for are "name" and
    // "desc". A scenario is where it comes from, because no call writes it. Empty by default,
    // which is what a scenario that describes an achievement by id alone gets.
    std::string display_name{};
    std::string display_description{};
};

// One person a profile is friends with: the id an invite is addressed to, and the name a
// roster or an invite list draws them with.
//
// `profile` is set only when the scenario named a profile in this same file instead of
// spelling an id out, which is how a run says that two of its own identities are friends.
// The Dispatcher fills the id and the name in from that profile once every profile has been
// read, so a friends list written as names cannot drift from the identities it names.
struct Friend
{
    std::uint64_t steam_id = 0;
    std::string persona_name;
    std::string profile;
};

// What one game is told about the world, from a scenario file.
//
// The stat and achievement lists are ordered rather than hashed: a scenario's
// order is kept, which is what makes the backend's view of a game match the file
// a person wrote. Counts here are a handful, so a lookup is a short scan.
class Profile
{
  public:
    std::string name = "default";
    std::int64_t app_id = 0;
    std::uint64_t steam_id = 76561197960287930ull;
    std::string persona_name = "DebugPlayer";
    std::string language = "english";
    std::string ui_language = "english";
    std::string install_path = "C:\\Program Files (x86)\\Steam";
    std::int64_t build_id = 1;

    std::vector<std::pair<std::string, std::int64_t>> stats;
    std::vector<Achievement> achievements;
    std::vector<std::pair<std::string, Json>> scripted;
    std::vector<Friend> friends;

    static Profile from_json(const std::string& profile_name, const Json& data);

    // By value, with a bool for "the scenario has it": this used to hand back a pointer
    // into `stats`, and `set_stat` appends to that vector - so a pointer held across a
    // write dangles. Reading is a value now and every write goes through `set_stat`, which
    // is what leaves no pointer to keep.
    bool find_stat(const std::string& key, std::int64_t& out) const noexcept;
    void set_stat(const std::string& key, std::int64_t value);

    int achievement_index(const std::string& achievement_name) const noexcept;
    Achievement* find_achievement(const std::string& achievement_name) noexcept;

    const Json* scripted_for(const std::string& call) const noexcept;

    // The name of one friend, for a roster that asks for a name by Steam id. A short
    // scan, like the stats above, and false when this profile has no such friend. The
    // parameter is not called `steam_id` because the profile has one of those.
    //
    // Not `noexcept`, and that is the whole of the difference between this and `find_stat`
    // above: a stat is an integer, so writing one out cannot fail, while this copies a
    // string and an allocation can. `noexcept` here would turn a name that could not be
    // copied into a process that ends - a great deal more than the name was worth - so the
    // failure travels to the caller instead, which is where every other failure inside a
    // handler goes. A `noexcept` boundary that returns to a *game* is a different thing and
    // stays one; see the exception-escape note in .clang-tidy.
    bool find_friend(std::uint64_t friend_id, std::string& out) const;
};

// What one call resolved to, and which rule resolved it.
//
// `via` is the honest part: "scripted", "state" or "none" says where an answer
// came from, and "none" means nobody had an opinion and the stub used its own
// default. A transcript records it, so a stale scenario is visible rather than
// mysterious.
struct Answer
{
    bool answered = false;
    Json ret;
    Json out;
    // What the backend wants done to the game once this answer is on its way: the
    // payloads a scripted entry asked for, with the call handle that entry
    // returned filled in. They travel with the reply, and the stub hands them over
    // on the game's next RunCallbacks - see bridge/synth.hpp.
    Json events;
    std::string via = "none";
};

class Session
{
  public:
    // `store`, when there is one, is where what this session writes is also kept. The
    // profile a session holds is what the scenario and the store agreed on when it
    // connected, so a stat set or an achievement unlocked here has to be written back or
    // the next run would never hear about it.
    //
    // Null - the default, and what every run without a state file passes - is the harness
    // exactly as it was before there was a store: a game's writes show up in the
    // transcript and go away with the run. Not owned; the server that made this session
    // outlives it.
    Session(std::string id, const Json& hello, Profile profile, Store* store = nullptr);

    const std::string& id() const noexcept { return _id; }
    std::int64_t pid() const noexcept { return _pid; }
    const std::string& exe() const noexcept { return _exe; }
    const std::string& arch() const noexcept { return _arch; }

    Profile& profile() noexcept { return _profile; }
    const Profile& profile() const noexcept { return _profile; }

    // What this session has changed, as opposed to what it was told: a game that
    // writes a stat or unlocks an achievement is worth seeing in a transcript.
    const std::vector<std::pair<std::string, std::int64_t>>& stats_written() const noexcept
    {
        return _stats_written;
    }
    const std::vector<std::string>& achievements_set() const noexcept { return _achievements_set; }

    // Called by the handlers as they change state, so the session keeps its own
    // account of what a game wrote rather than making a transcript diff it out - and,
    // when the run has a state file, so that the same change is written to it. One
    // call site for one change is what keeps the account a transcript shows and the
    // account left behind for the next run from ever drifting apart.
    void note_stat_written(const std::string& key, std::int64_t value);
    void note_achievement_set(const std::string& name);

    // One per call this session has handled, so a live view does not have to
    // count the transcript to say how busy a game is.
    void note_call() noexcept { ++_call_count; }
    std::size_t call_count() const noexcept { return _call_count; }

    // A game that has gone away keeps its session - the transcript and the run
    // summary are about the whole run, not just what is still attached - so this
    // is what tells a live view which rows are still live.
    void set_connected(bool connected) noexcept { _connected = connected; }
    bool connected() const noexcept { return _connected; }

    std::chrono::system_clock::time_point started() const noexcept { return _started; }
    std::string describe() const;

    // Answers one call, or reports that this session has no opinion.
    Answer handle(const std::string& name, const Json& args);

  private:
    std::string _id;
    std::int64_t _pid = 0;
    std::string _exe;
    std::string _arch;
    Profile _profile;
    // Where that profile's writes are kept, or null when this run has no state file.
    Store* _store = nullptr;
    std::chrono::system_clock::time_point _started;
    std::size_t _call_count = 0;
    bool _connected = true;
    std::vector<std::pair<std::string, std::int64_t>> _stats_written;
    std::vector<std::string> _achievements_set;
};

// The calls the state machine answers. Exposed so a test can prove every one of
// them appears in the generated surface - if the stub exports a call the
// backend has quietly stopped answering, that is exactly the drift the check
// exists to catch.
std::vector<std::string> state_handled_calls();

} // namespace steammock
