// The leaderboards this run has, and the calls that read and write one. See
// include/bridge/leaderboard.hpp for what a board is and why it is not a session's.
//
// The shape is the lobby world's, deliberately: a world answers only what it has grounds
// for, says which calls those are, and hands a game the payload that completes the call
// it just made. A call it declines is reported as unanswered, which is how the scenario
// and the session keep their say.

#include "bridge/leaderboard.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace steammock {
namespace {

// The calls this world answers, by the names the SDK's own surface sends. Every one of
// them is an ISteamUserStats call: a leaderboard is stats, and the calls that carry a
// statistic rather than a board (the achievements, the plain stats) are not here because
// nothing about a board is what they are.
constexpr const char* kFindOrCreateLeaderboard = "SteamAPI_ISteamUserStats_FindOrCreateLeaderboard";
constexpr const char* kFindLeaderboard = "SteamAPI_ISteamUserStats_FindLeaderboard";
constexpr const char* kGetLeaderboardName = "SteamAPI_ISteamUserStats_GetLeaderboardName";
constexpr const char* kGetLeaderboardEntryCount =
    "SteamAPI_ISteamUserStats_GetLeaderboardEntryCount";
constexpr const char* kGetLeaderboardSortMethod =
    "SteamAPI_ISteamUserStats_GetLeaderboardSortMethod";
constexpr const char* kGetLeaderboardDisplayType =
    "SteamAPI_ISteamUserStats_GetLeaderboardDisplayType";
constexpr const char* kDownloadLeaderboardEntries =
    "SteamAPI_ISteamUserStats_DownloadLeaderboardEntries";
constexpr const char* kGetDownloadedLeaderboardEntry =
    "SteamAPI_ISteamUserStats_GetDownloadedLeaderboardEntry";
constexpr const char* kUploadLeaderboardScore = "SteamAPI_ISteamUserStats_UploadLeaderboardScore";

// The SDK's enumerations written out, because this tree carries no Valve enumeration and
// a value typed as a number is what the wire has anyway.
//
// Ascending is the fast one: the lowest number wins, which is what a "quickest win" board
// means. Descending is therefore what this file calls "not ascending", because there is no
// third sort method for it to be confused with.
constexpr std::int32_t kSortMethodAscending = 1;

// An upload is one of three things, and two of them are named because they are the ones that
// decide something: told not to write at all, or told to write whatever was sent. The third,
// keep-best, is the game asking this world to judge - replace the score only if the new one is
// better - and that is what happens to every upload that is not one of these two.
constexpr std::int32_t kUploadScoreNone = 0;
constexpr std::int32_t kUploadScoreForceUpdate = 2;
constexpr std::int32_t kDataRequestGlobal = 0;
constexpr std::int32_t kDataRequestGlobalAroundUser = 1;
constexpr std::int32_t kDataRequestFriends = 2;

// Everything this world answers is labelled "leaderboard", so a transcript says which
// calls came out of a board rather than out of a scenario, a session or a room.
constexpr const char* kLeaderboardVia = "leaderboard";

// --- reading what a call was given -----------------------------------------

std::uint64_t id_member(const Json& object, const char* key) {
    const Json* value = json_member(object, key);
    if (value == nullptr || !value->is_number()) {
        return 0;
    }
    return as_uint64(*value);
}

std::int64_t int_member(const Json& object, const char* key, std::int64_t fallback) {
    const Json* value = json_member(object, key);
    if (value == nullptr || !value->is_number()) {
        return fallback;
    }
    return as_int64(*value);
}

std::string text_member(const Json& object, const char* key) {
    const Json* value = json_member(object, key);
    if (value == nullptr || !value->is_string()) {
        return std::string();
    }
    return std::string(as_string(*value));
}

Json id_value(std::uint64_t id) { return Json(static_cast<std::int64_t>(id)); }

// --- answers ---------------------------------------------------------------

Answer answered(Json ret) {
    Answer answer;
    answer.answered = true;
    answer.ret = std::move(ret);
    return answer;
}

Answer from_leaderboard(Json ret) {
    Answer answer = answered(std::move(ret));
    answer.via = kLeaderboardVia;
    return answer;
}

// An answer that also completes the call it came with. A game files the payload under the
// handle the answer returned - the same rule a scripted "then" entry follows - and that
// handle is what ties "the board you asked for" or "the rows you asked for" to the call
// that asked.
Answer from_leaderboard_calling(Json ret, Json event) {
    event["call"] = ret;
    Answer answer = answered(std::move(ret));
    answer.via = kLeaderboardVia;
    answer.events = Json::array();
    answer.events.push_back(std::move(event));
    return answer;
}

Json payload_of(const char* name, Json fields) {
    Json event = Json::object();
    event["event"] = Json(name);
    event["in"] = std::move(fields);
    return event;
}

Json find_result_payload(std::uint64_t board, bool found) {
    Json fields = Json::object();
    fields["m_hSteamLeaderboard"] = id_value(board);
    fields["m_bLeaderboardFound"] = Json(found ? 1 : 0);
    return payload_of("LeaderboardFindResult_t", std::move(fields));
}

Json scores_downloaded_payload(std::uint64_t board, std::uint64_t entries, std::int32_t count) {
    Json fields = Json::object();
    fields["m_hSteamLeaderboard"] = id_value(board);
    fields["m_hSteamLeaderboardEntries"] = id_value(entries);
    fields["m_cEntryCount"] = Json(static_cast<std::int64_t>(count));
    return payload_of("LeaderboardScoresDownloaded_t", std::move(fields));
}

Json score_uploaded_payload(std::uint64_t board, std::int32_t score, bool changed,
                            std::int32_t rank_new, std::int32_t rank_previous) {
    Json fields = Json::object();
    fields["m_bSuccess"] = Json(1);
    fields["m_hSteamLeaderboard"] = id_value(board);
    fields["m_nScore"] = Json(static_cast<std::int64_t>(score));
    fields["m_bScoreChanged"] = Json(changed ? 1 : 0);
    fields["m_nGlobalRankNew"] = Json(static_cast<std::int64_t>(rank_new));
    fields["m_nGlobalRankPrevious"] = Json(static_cast<std::int64_t>(rank_previous));
    return payload_of("LeaderboardScoreUploaded_t", std::move(fields));
}

// Where a row went, as the answer to a download of one row: the fields of
// LeaderboardEntry_t the SDK declares, which is what the stub writes into the game's own
// structure - the one thing a call here fills in through a pointer the game owns.
Json row_fields(const LeaderboardRow& row) {
    Json fields = Json::object();
    fields["m_steamIDUser"] = id_value(row.steam_id);
    fields["m_nGlobalRank"] = Json(static_cast<std::int64_t>(row.global_rank));
    fields["m_nScore"] = Json(static_cast<std::int64_t>(row.score));
    // The two the game did not ask this run about: a details array is the game's own
    // numbers beside a score, and a UGC handle is an item - neither of which a board of
    // scores has anything to say about, and both of which are zero rather than missing.
    fields["m_cDetails"] = Json(0);
    fields["m_hUGC"] = id_value(0);
    return fields;
}

}  // namespace

// --- the boards ------------------------------------------------------------

const LeaderboardRow* Leaderboard::find(std::uint64_t steam_id) const noexcept {
    for (const auto& row : rows) {
        if (row.steam_id == steam_id) {
            return &row;
        }
    }
    return nullptr;
}

std::int32_t Leaderboard::rank_of(std::uint64_t steam_id) const noexcept {
    const LeaderboardRow* row = find(steam_id);
    // Nobody on the board has no rank, and that is zero on the wire - which is what a
    // game checks for when it asks whether a player has ever posted a score.
    return row != nullptr ? row->global_rank : 0;
}

Leaderboard* LeaderboardWorld::find_board(std::uint64_t id) noexcept {
    for (auto& board : _boards) {
        if (board.id == id) {
            return &board;
        }
    }
    return nullptr;
}

Leaderboard* LeaderboardWorld::find_board(const std::string& name) noexcept {
    for (auto& board : _boards) {
        if (board.name == name) {
            return &board;
        }
    }
    return nullptr;
}

bool LeaderboardWorld::is_better(std::int32_t sort_method, std::int32_t score,
                                 std::int32_t than) const noexcept {
    // Which end of the numbers wins is the board's, not this world's: a time is best when
    // it is smallest, and everything else is best when it is largest.
    return sort_method == kSortMethodAscending ? score < than : score > than;
}

void LeaderboardWorld::place(Leaderboard& board, std::uint64_t steam_id, std::int32_t score) {
    bool known = false;
    for (auto& row : board.rows) {
        if (row.steam_id == steam_id) {
            row.score = score;
            known = true;
            break;
        }
    }
    if (!known) {
        LeaderboardRow row;
        row.steam_id = steam_id;
        row.score = score;
        board.rows.push_back(row);
    }

    // Stable, so two players who posted the same score keep the order they arrived in - a
    // real board has a rule for a tie as well, and the one nobody has to be told is "the
    // one who was there first".
    const bool ascending = board.sort_method == kSortMethodAscending;
    std::stable_sort(board.rows.begin(), board.rows.end(),
                     [ascending](const LeaderboardRow& left, const LeaderboardRow& right) {
                         return ascending ? left.score < right.score : left.score > right.score;
                     });
    for (std::size_t index = 0; index < board.rows.size(); ++index) {
        board.rows[index].global_rank = static_cast<std::int32_t>(index) + 1;
    }
}

// --- the calls -------------------------------------------------------------

std::vector<std::string> LeaderboardWorld::handled_calls() {
    return {
        kFindOrCreateLeaderboard,    kFindLeaderboard,
        kGetLeaderboardName,         kGetLeaderboardEntryCount,
        kGetLeaderboardSortMethod,   kGetLeaderboardDisplayType,
        kDownloadLeaderboardEntries, kGetDownloadedLeaderboardEntry,
        kUploadLeaderboardScore,
    };
}

bool LeaderboardWorld::answer(const Session& session, const std::string& call, const Json& args,
                              Answer& out) {
    const std::uint64_t me = session.profile().steam_id;

    if (call == kFindOrCreateLeaderboard || call == kFindLeaderboard) {
        const std::string name = text_member(args, "pchLeaderboardName");
        if (name.empty()) {
            // A board with no name is not a board, and a game that asks for one has a bug
            // this world should not paper over with an invented board.
            return false;
        }
        Leaderboard* board = find_board(name);
        if (board == nullptr) {
            if (call == kFindLeaderboard) {
                // Find, not find-or-create: a name nobody has asked for yet is a board that
                // does not exist, which is exactly what m_bLeaderboardFound says. A game
                // that wants one made asks the other call.
                out =
                    from_leaderboard_calling(id_value(_next_call++), find_result_payload(0, false));
                return true;
            }
            Leaderboard created;
            created.id = _next_board_id++;
            created.name = name;
            created.sort_method =
                static_cast<std::int32_t>(int_member(args, "eLeaderboardSortMethod", 0));
            created.display_type =
                static_cast<std::int32_t>(int_member(args, "eLeaderboardDisplayType", 0));
            _boards.push_back(std::move(created));
            board = &_boards.back();
        }
        out =
            from_leaderboard_calling(id_value(_next_call++), find_result_payload(board->id, true));
        return true;
    }

    if (call == kGetLeaderboardName) {
        Leaderboard* board = find_board(id_member(args, "hSteamLeaderboard"));
        if (board == nullptr) {
            // A handle this run never gave out. A name is not invented for it: the call is
            // reported as unanswered and the game draws what it draws for no name at all.
            return false;
        }
        out = from_leaderboard(Json(board->name));
        return true;
    }

    if (call == kGetLeaderboardEntryCount || call == kGetLeaderboardSortMethod ||
        call == kGetLeaderboardDisplayType) {
        Leaderboard* board = find_board(id_member(args, "hSteamLeaderboard"));
        if (board == nullptr) {
            return false;
        }
        if (call == kGetLeaderboardEntryCount) {
            out = from_leaderboard(Json(static_cast<std::int64_t>(board->rows.size())));
        } else if (call == kGetLeaderboardSortMethod) {
            out = from_leaderboard(Json(static_cast<std::int64_t>(board->sort_method)));
        } else {
            out = from_leaderboard(Json(static_cast<std::int64_t>(board->display_type)));
        }
        return true;
    }

    if (call == kDownloadLeaderboardEntries) {
        Leaderboard* board = find_board(id_member(args, "hSteamLeaderboard"));
        if (board == nullptr) {
            return false;
        }
        const std::int32_t request =
            static_cast<std::int32_t>(int_member(args, "eLeaderboardDataRequest", 0));
        const std::int32_t start = static_cast<std::int32_t>(int_member(args, "nRangeStart", 0));
        const std::int32_t end = static_cast<std::int32_t>(int_member(args, "nRangeEnd", 0));

        // Which rows the range selects, in the ranks the game asked about. A global range is
        // absolute (1 is the top of the board, which is what Spacewar asks for), one around
        // the user is relative to where that user sits, and a friends range is a ranking of
        // its own - which here is the whole board, because every player this run has heard
        // from is a player the others are friends with: that is what answering a roster row
        // for any of them means.
        std::vector<LeaderboardRow> selected;
        if (request == kDataRequestGlobal || request == kDataRequestFriends) {
            for (const auto& row : board->rows) {
                if (row.global_rank >= start && row.global_rank <= end) {
                    selected.push_back(row);
                }
            }
        } else if (request == kDataRequestGlobalAroundUser) {
            const std::int32_t mine = board->rank_of(me);
            if (mine > 0) {
                for (const auto& row : board->rows) {
                    const std::int32_t offset = row.global_rank - mine;
                    if (offset >= start && offset <= end) {
                        selected.push_back(row);
                    }
                }
            }
        } else {
            // A request for a list of users is the one kind this world cannot honour: the
            // ids travel in a list the layouts describe as opaque, so which users were
            // meant never arrives.
            return false;
        }

        const std::uint64_t handle = _next_entries++;
        _downloads.emplace_back(handle, std::move(selected));
        if (_downloads.size() > kMaxDownloads) {
            _downloads.erase(_downloads.begin());
        }
        const auto count = static_cast<std::int32_t>(_downloads.back().second.size());
        out = from_leaderboard_calling(id_value(_next_call++),
                                       scores_downloaded_payload(board->id, handle, count));
        return true;
    }

    if (call == kGetDownloadedLeaderboardEntry) {
        const std::uint64_t handle = id_member(args, "hSteamLeaderboardEntries");
        const auto index = int_member(args, "index", -1);
        const std::vector<LeaderboardRow>* rows = nullptr;
        for (const auto& download : _downloads) {
            if (download.first == handle) {
                rows = &download.second;
                break;
            }
        }
        if (rows == nullptr || index < 0 || index >= static_cast<std::int64_t>(rows->size())) {
            // Past the end of what was handed over: no row, which is what the call's own
            // false means - not a row of zeroes, which a game would draw.
            return false;
        }
        Json values = Json::object();
        values["pLeaderboardEntry"] = row_fields((*rows)[static_cast<std::size_t>(index)]);
        if (int_member(args, "cDetailsMax", 0) > 0) {
            // A details array is the game's own numbers beside a score and this world keeps
            // none of them. The first slot is written as zero so that a game which asked for
            // them gets the count it asked for rather than whatever was in its own buffer.
            values["pDetails"] = Json(0);
        }
        Answer answer = from_leaderboard(Json(true));
        answer.out = std::move(values);
        out = std::move(answer);
        return true;
    }

    if (call == kUploadLeaderboardScore) {
        Leaderboard* board = find_board(id_member(args, "hSteamLeaderboard"));
        if (board == nullptr) {
            return false;
        }
        const std::int32_t method =
            static_cast<std::int32_t>(int_member(args, "eLeaderboardUploadScoreMethod", 0));
        const std::int32_t score = static_cast<std::int32_t>(int_member(args, "nScore", 0));
        const std::int32_t before = board->rank_of(me);

        if (method == kUploadScoreNone) {
            // Told not to write: the answer still says what the board looks like for this
            // player, and nothing on it changes.
            out = from_leaderboard_calling(
                id_value(_next_call++),
                score_uploaded_payload(board->id, score, false, before, before));
            return true;
        }

        const LeaderboardRow* existing = board->find(me);
        const bool better =
            existing == nullptr || is_better(board->sort_method, score, existing->score);
        const bool write = method == kUploadScoreForceUpdate || better;
        if (write) {
            place(*board, me, score);
        }
        const std::int32_t stored = board->rank_of(me) > 0 ? board->find(me)->score : score;
        out = from_leaderboard_calling(
            id_value(_next_call++),
            score_uploaded_payload(board->id, stored, write, board->rank_of(me), before));
        return true;
    }

    // Not a leaderboard call, or one about a board this run has never been asked for.
    return false;
}

}  // namespace steammock
