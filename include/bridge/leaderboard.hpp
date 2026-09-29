#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "bridge/json_read.hpp"
#include "bridge/session.hpp"

namespace steammock {

// ---------------------------------------------------------------------------
//  The leaderboards this run has, and who is on them.
// ---------------------------------------------------------------------------
//  A leaderboard is global: one board every player in the world is ranked on, and a
//  game reads a ranking it did not write. That is what makes it the second thing here
//  that no single session can answer - the first is a lobby - so it lives beside it,
//  outlives the sessions that made it, and answers from what games have actually done.
//
//  Nothing here invents a score. A board starts empty and a player appears on it by
//  uploading the score it just earned, which is also how a real board fills: the first
//  entries of a new leaderboard are the people who played. A run of four Spacewar
//  clients that each finish a round is a board with four real entries on it, and
//  Spacewar draws them with the players' own names.
//
//  What a board does not have is a clock, and it does not need one: a real board is a
//  service that ranks what it has been sent, and so is this. A score's rank is the
//  place it takes among the scores this run was told about.

// One row of a board: the player, the score, and the place it took. The place is kept
// beside the score rather than recomputed on the way out, because a download has to
// answer with the ranking as it was when the game asked - a row read a minute later is
// still the row that was handed over.
struct LeaderboardRow {
    std::uint64_t steam_id = 0;
    std::int32_t score = 0;
    std::int32_t global_rank = 0;
};

struct Leaderboard {
    std::uint64_t id = 0;
    std::string name;
    // k_ELeaderboardSortMethodAscending or Descending, and what the board's numbers
    // mean: a time in seconds, a plain number. Both are the game's choice when it first
    // asks for the board, and both are handed back as they were given.
    std::int32_t sort_method = 0;
    std::int32_t display_type = 0;
    // Best first, by whichever end of the ranking this board sorts to, with every row's
    // rank already in it. Kept in order rather than sorted per call, because a download
    // is a slice of this list.
    std::vector<LeaderboardRow> rows;

    const LeaderboardRow* find(std::uint64_t steam_id) const noexcept;
    std::int32_t rank_of(std::uint64_t steam_id) const noexcept;
};

class LeaderboardWorld {
public:
    // A board handle is the game's to hand back and nothing more: Steam's own is a
    // pointer the game never looks at, so this is a counter with a shape that is not a
    // Steam id - a game that printed one of these would see it is not a player.
    static constexpr std::uint64_t kFirstBoardId = 0x0B00000000000001ull;

    // Call handles are ours to choose, but two worlds answer the same game: the lobby
    // world hands calls out from 1000 up, so this one starts far enough away that a
    // handle means one thing inside a process. A collision would file this world's
    // payload under that world's call, which is a game told about something it never
    // asked for.
    static constexpr std::uint64_t kFirstCallHandle = 100000;
    static constexpr std::uint64_t kFirstEntriesHandle = 200000;

    // How many downloads stay readable. Steam's own handle is invalid the moment the
    // callback returns, and a game that reads an entry out of one it kept is reading
    // something a real Steam would have reused by then - but a run is long and a menu is
    // visited over and over, so the window is bounded rather than being the whole run.
    static constexpr std::size_t kMaxDownloads = 64;

    // The calls this world answers, so a test can prove every one of them is a name the
    // stub can actually send.
    static std::vector<std::string> handled_calls();

    // What one leaderboard call resolved to. False means this world has nothing to say
    // about it - not a leaderboard call at all, or a board this run has never been asked
    // for - which leaves the scenario and the session their say, in that order.
    bool answer(const Session& session, const std::string& call, const Json& args, Answer& out);

    // The boards, for a test and for a reader of the run: what a board is and who is on
    // it is not visible from the answer to one call.
    const std::vector<Leaderboard>& boards() const noexcept { return _boards; }

private:
    Leaderboard* find_board(std::uint64_t id) noexcept;
    Leaderboard* find_board(const std::string& name) noexcept;

    // Put a player's score where it belongs on the board and re-rank what moved. The
    // board's own sort method is what says which end is best: ascending means the
    // smallest number wins, which is what makes a "quickest win" board rank a fast round
    // first and a "most feet travelled" one rank a long round first.
    void place(Leaderboard& board, std::uint64_t steam_id, std::int32_t score);
    bool is_better(std::int32_t sort_method, std::int32_t score, std::int32_t than) const noexcept;

    std::vector<Leaderboard> _boards;
    std::uint64_t _next_board_id = kFirstBoardId;
    std::uint64_t _next_call = kFirstCallHandle;

    // The rows a download handle stands for, as the snapshot it was when the call was
    // answered. A game reads them out of its callback, and what it kept a handle to after
    // that is not something a real Steam would still be holding.
    std::vector<std::pair<std::uint64_t, std::vector<LeaderboardRow>>> _downloads;
    std::uint64_t _next_entries = kFirstEntriesHandle;
};

}  // namespace steammock
