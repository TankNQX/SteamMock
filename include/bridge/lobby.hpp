#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "bridge/json_read.hpp"
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

// What one game sent another over Steam's own network: the bytes, the channel they belong
// to, and who sent them - which is what a reader is told beside the payload.
struct P2PPacket {
    std::uint64_t remote = 0;
    std::int32_t channel = 0;
    std::string bytes;
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

    // The first game server's Steam id: the account type in the top bits says "anonymous
    // game server", which is what tells a server apart from the players in its room.
    //
    // The universe has to be a real one as well, and that is not decoration: a game
    // deciding whether the address a lobby published is worth connecting to asks
    // CSteamID::IsValid(), which refuses an invalid universe before it looks at
    // anything else. Spacewar's client is exactly that game - it receives the lobby's
    // game server, asks IsValid(), and quietly does nothing when the answer is no,
    // which is a client that never connects and says nothing about why.
    //
    // The two names below are the SDK's k_EUniversePublic and
    // k_EAccountTypeAnonGameServer written out: this tree carries no Valve
    // enumeration, so the values say what they are here instead.
    static constexpr std::uint64_t kUniversePublic = 1;
    static constexpr std::uint64_t kAnonymousGameServer = 4;
    static constexpr std::uint64_t kFirstGameServerId =
        (kUniversePublic << 56) | (kAnonymousGameServer << 52) | 1ull;

    // The user handles the two ends of a process answer under. Steam runs a game
    // server as a user of its own on the same pipe as the customer it sits beside, and
    // a game asks for an interface under the handle it means. That is the only thing
    // that tells a customer's ISteamNetworking from a game server's: both are handed
    // the same version string and both then make the same calls. The world is where
    // those handles are given out, so it is also where a call can be read back to the
    // end that made it.
    static constexpr std::int32_t kCustomerHSteamUser = 1;
    static constexpr std::int32_t kCustomerHSteamPipe = 1;
    static constexpr std::int32_t kGameServerHSteamUser = 2;
    static constexpr std::int32_t kGameServerHSteamPipe = 2;

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

    // The identity this session answers to as a game server: minted the first time it is
    // asked for, and found rather than minted when a packet is being routed.
    std::uint64_t game_server_id_of(std::uint64_t user);
    std::uint64_t known_game_server_id(std::uint64_t user) const noexcept;

    // One game's packets, by the Steam id they were addressed to. A session that hosts
    // is a customer and a game server at once and has an id for each, and a packet
    // addressed to either of them is for this process - but not for both of its ends,
    // which is why every read says which handle it was made through.
    void queue_packet(std::uint64_t to, std::uint64_t from, std::int32_t channel,
                      const std::string& bytes);
    const P2PPacket* peek_packet(std::uint64_t user, std::int32_t hSteamUser,
                                 std::int32_t channel) const noexcept;
    void drop_packet(std::uint64_t user, std::int32_t hSteamUser, std::int32_t channel);

    // The id a call made through this user handle is addressed to: the game server this
    // world minted for the session when the handle is a game server's, and the
    // session's own id otherwise - which is what a process without one of those is.
    std::uint64_t endpoint_of(std::uint64_t user, std::int32_t hSteamUser) const noexcept;

    // The session behind a Steam id, which is the id itself unless it names a game server,
    // and whether this pair has already talked - the question Steam's session request hangs
    // on.
    std::uint64_t user_of(std::uint64_t id) const noexcept;
    bool needs_session_request(std::uint64_t from, std::uint64_t to);

    std::vector<Lobby> _lobbies;
    std::uint64_t _next_lobby_id = kFirstLobbyId;
    std::uint64_t _next_call = kFirstCallHandle;

    // Where each session's game server says it would be, by Steam id. A game that starts
    // a server tells the SDK its game port and often passes zero for the address when it
    // publishes it, expecting Steam to fill both in - so the run has to know both to do
    // the same. Recorded from the calls that carry them, not asked for.
    std::vector<std::pair<std::uint64_t, std::uint16_t>> _game_ports;

    // The identity each session answers to as a game server. Minted the first time it asks,
    // which is what lets a packet addressed to a server be routed to the session running it
    // - and keeps two servers in one run from being handed the same id.
    std::vector<std::pair<std::uint64_t, std::uint64_t>> _game_server_ids;

    // What one game has sent to another and the other has not read yet, by Steam id of the
    // recipient. Steam moves these itself and never shows them to anyone, so the only part
    // that has to be modelled is that they arrive whole and in the order they were sent.
    std::vector<std::pair<std::uint64_t, std::vector<P2PPacket>>> _packets;

    // Which pairs have talked, so a game is told the first time somebody it has not spoken
    // to writes to it - which is what Steam's session request is for.
    std::vector<std::pair<std::uint64_t, std::uint64_t>> _contacts;
};

}  // namespace steammock
