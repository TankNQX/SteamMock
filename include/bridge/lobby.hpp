#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "bridge/json.hpp"
#include "bridge/session.hpp"

namespace steammock {

// ---------------------------------------------------------------------------
//  The lobbies this run has, and who is in them.
// ---------------------------------------------------------------------------
//  A lobby is the one thing a single game cannot have on its own: "find the one
//  somebody else made" is the entire point of one, and nothing held inside a
//  session can answer it. So this is the only backend state that is not per game.
//  It outlives a session - a game that disconnects stays in its lobby until it
//  leaves - and it is what makes two instances agree about the same room instead
//  of each being handed a convenient story.
//
//  It answers only from what games have actually done. A lobby it has not seen,
//  or a call it does not know, is declined, so the scenario and the session still
//  get their say in that order: a scenario that wants to lie about a lobby goes on
//  winning, because the world only speaks about rooms that exist.

struct LobbyMember {
    std::uint64_t steam_id = 0;
    std::string persona;
    std::vector<std::pair<std::string, std::string>> data;

    const std::string* find(const std::string& key) const noexcept;
    void set(std::string key, std::string value);
};

struct Lobby {
    std::uint64_t id = 0;
    std::uint64_t owner = 0;
    std::int64_t max_members = 4;
    std::vector<std::pair<std::string, std::string>> data;
    std::vector<LobbyMember> members;

    // What the room says about its game server, which is how a game that has one lets
    // the others find it - and, for a client, the only thing needed to connect. Steam
    // hands the address over in host order and does not interpret it, and neither does
    // this: it is the game's to make sense of.
    bool has_game_server = false;
    std::uint32_t game_server_ip = 0;
    std::uint16_t game_server_port = 0;
    std::uint64_t game_server_id = 0;

    LobbyMember* find_member(std::uint64_t steam_id) noexcept;
    const LobbyMember* find_member(std::uint64_t steam_id) const noexcept;
    const std::string* find_data(const std::string& key) const noexcept;
    void set_data(std::string key, std::string value);
};

// What one lobby call resolved to, plus what the *other* members have to be told.
//
// A notification names a Steam id rather than a session because only the server
// knows which connection belongs to which identity, and only the server knows who
// is still connected to hear it.
class LobbyWorld {
public:
    // A lobby id the way Steam hands them out: the lobby type and universe in the
    // top bits, so a game is passed something that looks like a CSteamID rather
    // than a small counter it might mistake for an index.
    static constexpr std::uint64_t kFirstLobbyId = 109775240917155840ull;

    // Call handles are ours to choose - a game only ever hands one back - but they
    // must not collide within a run, because that is what a call result is filed
    // under.
    static constexpr std::uint64_t kFirstCallHandle = 1000;

    // The calls this world answers, so a test can prove every one of them is a
    // name the stub can actually send.
    static std::vector<std::string> handled_calls();

    bool answer(const Session& session, const std::string& call, const Json& args, Answer& out,
                std::vector<std::pair<std::uint64_t, Json>>& notifications);

    std::size_t lobby_count() const noexcept { return _lobbies.size(); }

private:
    Lobby* find_lobby(std::uint64_t id) noexcept;
    const Lobby* find_lobby(std::uint64_t id) const noexcept;
    const LobbyMember* find_member_anywhere(std::uint64_t steam_id) const noexcept;

    // What a member joining or leaving means for everyone else in the room.
    void notify_member_change(const Lobby& lobby, const LobbyMember& member,
                              std::uint64_t making_change, std::uint32_t state_change,
                              std::vector<std::pair<std::uint64_t, Json>>& notifications) const;

    std::vector<Lobby> _lobbies;
    std::uint64_t _next_lobby_id = kFirstLobbyId;
    std::uint64_t _next_call = kFirstCallHandle;
};

}  // namespace steammock
