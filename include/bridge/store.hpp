#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "bridge/inventory.hpp"
#include "bridge/leaderboard.hpp"
#include "bridge/session.hpp"

namespace steammock
{

// ---------------------------------------------------------------------------
//  The state that outlives a run.
// ---------------------------------------------------------------------------
//  Everything else in this backend is a function of the scenario and of what the
//  games in *this* run have done. A profile's stats, a board's rows and what a
//  player holds all go away when the process does, which is what makes a run
//  repeatable and is also the whole of what someone debugging a game is missing:
//  an achievement unlocked on Tuesday is locked again on Wednesday.
//
//  This is the one place that keeps what a game wrote, so the next run starts
//  from where the last one left off. It is a state *file*, opened only when a
//  person asks for one by name - `steammock --state <path>` - so a run with no
//  state file behaves exactly as it did before there was one.
//
//  What it is deliberately not is a second source of truth for what a scenario
//  already says. A scenario stays the file a person reads, edits and hands to
//  somebody else; the store holds what games *did* to it, and the two are
//  reconciled in one direction:
//
//    * a value a game wrote is the store's, and stands - which is the whole
//      reason for keeping anything;
//    * a value only a scenario has ever written is the scenario's, and is
//      refreshed from it - so editing a scenario still changes the next run;
//    * nothing is ever deleted for being absent from either, so a row written by
//      hand into the file survives everything.
//
//  The line this draws around a *session* is worth saying out loud. Two games
//  matched to one profile are handed copies of it, and with no store that means
//  they cannot see each other's writes. With one they can, because the store is
//  the record and both merge from it when they connect - an identity with two
//  sets of stats is not what keeping state is supposed to mean. A run that needs
//  the copies kept apart is a run with no state file, and that is the default.
//
//  A store never throws and never fails a call: a write that cannot be made is
//  recorded in `error()` and the run carries on with the state it has in memory.
//  That is the one thing about a state file a person would never notice on their
//  own - everything works and nothing is being kept - so it is reported rather
//  than swallowed.

// One board as the store has it. A row arrives without its rank: a rank is the
// board's own answer about an order rather than a fact about a score, so the board
// computes it as the rows go back in. That is what keeps a stored board and a board
// that grew during a run from being able to disagree about the order of one.
struct StoredBoard
{
    std::string name;
    // The game's own choices when it first asked for the board: which end of the
    // numbers wins, and what the numbers mean.
    std::int32_t sort_method = 0;
    std::int32_t display_type = 0;
    std::vector<std::pair<std::uint64_t, std::int32_t>> scores;
};

class Store
{
  public:
    // Opens the state file at `path`, making one and its schema if it is not there.
    // Null means it could not be opened - a path that cannot be written, or a file
    // that is not this harness's - and `error` says what happened.
    //
    // No special names. `:memory:` is SQLite's own spelling for a database that is
    // not a file, and it means the same thing here: the schema without the keeping.
    static std::unique_ptr<Store> open(const std::string& path, std::string& error);

    Store() = default;
    Store(const Store&) = delete;
    Store& operator=(const Store&) = delete;
    virtual ~Store() = default;

    // The first thing that has gone wrong since this store was opened, or an empty
    // string while nothing has. Never cleared: the first failure is the one worth
    // knowing about, and the ones after it are the same failure arriving again.
    virtual const std::string& error() const noexcept = 0;

    // --- what one identity has ----------------------------------------------

    // Writes what a scenario authored: keys the store has never been told about,
    // and keys only a scenario has ever written. A key a game wrote is left as the
    // game left it. Called for a profile once, before `merge_into`.
    virtual void seed(const Profile& profile) = 0;

    // Folds everything the store knows about `profile.name` into the profile itself:
    // its stats and its achievements, with a stored value standing over what the
    // scenario declared, and the friends it does not already have.
    //
    // Friends are the one exception to that, and it is on purpose. Nothing writes a
    // friends list at runtime, so a scenario remains the record for the friends it
    // names - an author who renames a friend should see the new name, not the one a
    // previous run wrote down. What the store adds is the friend no scenario names,
    // which is how a list grows without the scenario being edited.
    virtual void merge_into(Profile& profile) const = 0;

    // What a game wrote. Called from the state machine as it changes its own copy of
    // the profile, so that the change a transcript shows and the change left behind
    // for the next run are the same change rather than two that could drift.
    virtual void write_stat(const std::string& profile, const std::string& name,
                            std::int64_t value) = 0;
    virtual void write_achievement(const std::string& profile, const std::string& name,
                                   bool achieved) = 0;

    // --- what the run's players have done -----------------------------------

    // Every board the store holds, and the scores on them. Read once, when the world
    // that owns the boards starts, because a board is not per session and loading one
    // the first time it is asked about would be the same read with a longer story.
    virtual void load_boards(std::vector<StoredBoard>& boards) const = 0;
    virtual void save_board(const Leaderboard& board) = 0;
    virtual void save_row(const std::string& board, const LeaderboardRow& row) = 0;

    // Every inventory the store holds, by player, read once for the same reason. A
    // player who appears here and never connects again keeps their items: what an
    // inventory is for is outliving the screens that asked for it.
    virtual void
    load_inventories(std::map<std::uint64_t, std::vector<ItemDetails>>& inventories) const = 0;
    // An item id is saved with the item because a game was handed it: an item that
    // came back under a new id would be one the game is told it has two of.
    virtual void save_item(std::uint64_t steam_id, const ItemDetails& item) = 0;
};

} // namespace steammock
