#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "bridge/json.hpp"

namespace steammock {

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

struct Achievement {
    std::string name;
    bool achieved = false;
};

// What one game is told about the world, from a scenario file.
//
// The stat and achievement lists are ordered rather than hashed: a scenario's
// order is kept, which is what makes the backend's view of a game match the file
// a person wrote. Counts here are a handful, so a lookup is a short scan.
class Profile {
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

    static Profile from_json(const std::string& profile_name, const Json& data);

    std::int64_t* find_stat(const std::string& key) noexcept;
    const std::int64_t* find_stat(const std::string& key) const noexcept;
    void set_stat(const std::string& key, std::int64_t value);

    int achievement_index(const std::string& achievement_name) const noexcept;
    Achievement* find_achievement(const std::string& achievement_name) noexcept;

    const Json* scripted_for(const std::string& call) const noexcept;
};

// What one call resolved to, and which rule resolved it.
//
// `via` is the honest part: "scripted", "state" or "none" says where an answer
// came from, and "none" means nobody had an opinion and the stub used its own
// default. A transcript records it, so a stale scenario is visible rather than
// mysterious.
struct Answer {
    bool answered = false;
    Json ret;
    Json out;
    std::string via = "none";
};

class Session {
public:
    Session(std::string id, const Json& hello, Profile profile);

    const std::string& id() const noexcept { return _id; }
    std::int64_t pid() const noexcept { return _pid; }
    const std::string& exe() const noexcept { return _exe; }
    const std::string& arch() const noexcept { return _arch; }

    Profile& profile() noexcept { return _profile; }
    const Profile& profile() const noexcept { return _profile; }

    // What this session has changed, as opposed to what it was told: a game that
    // writes a stat or unlocks an achievement is worth seeing in a transcript.
    const std::vector<std::pair<std::string, std::int64_t>>& stats_written() const noexcept {
        return _stats_written;
    }
    const std::vector<std::string>& achievements_set() const noexcept { return _achievements_set; }

    // Called by the handlers as they change state, so the session keeps its own
    // account of what a game wrote rather than making a transcript diff it out.
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

}  // namespace steammock
