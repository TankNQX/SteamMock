#include "bridge/lobby.hpp"

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace steammock {

// ---------------------------------------------------------------------------
//  The rooms themselves.
// ---------------------------------------------------------------------------

const std::string* LobbyMember::find(const std::string& key) const noexcept {
    for (const auto& entry : data) {
        if (entry.first == key) {
            return &entry.second;
        }
    }
    return nullptr;
}

void LobbyMember::set(std::string key, std::string value) {
    for (auto& entry : data) {
        if (entry.first == key) {
            entry.second = std::move(value);
            return;
        }
    }
    data.emplace_back(std::move(key), std::move(value));
}

LobbyMember* Lobby::find_member(std::uint64_t steam_id) noexcept {
    for (auto& member : members) {
        if (member.steam_id == steam_id) {
            return &member;
        }
    }
    return nullptr;
}

const LobbyMember* Lobby::find_member(std::uint64_t steam_id) const noexcept {
    for (const auto& member : members) {
        if (member.steam_id == steam_id) {
            return &member;
        }
    }
    return nullptr;
}

const std::string* Lobby::find_data(const std::string& key) const noexcept {
    for (const auto& entry : data) {
        if (entry.first == key) {
            return &entry.second;
        }
    }
    return nullptr;
}

void Lobby::set_data(std::string key, std::string value) {
    for (auto& entry : data) {
        if (entry.first == key) {
            entry.second = std::move(value);
            return;
        }
    }
    data.emplace_back(std::move(key), std::move(value));
}

namespace {

// --- the wire, read the way the state handlers read it ---------------------

std::string string_member(const Json& object, const char* key) {
    const Json* value = json_member(object, key);
    if (value == nullptr || !value->is_string()) {
        return std::string();
    }
    return as_string(*value);
}

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

Json id_value(std::uint64_t id) { return Json(static_cast<std::int64_t>(id)); }

// --- answers ---------------------------------------------------------------

Answer answered(Json ret) {
    Answer answer;
    answer.answered = true;
    answer.ret = std::move(ret);
    return answer;
}

// Everything the world answers is labelled "lobby", so a transcript says which
// calls came out of rooms games made rather than out of a scenario or a session.
constexpr const char* kLobbyVia = "lobby";

Answer from_lobby(Json ret) {
    Answer answer = answered(std::move(ret));
    answer.via = kLobbyVia;
    return answer;
}

// An answer that also completes the call it came with. The stub has to know which
// call a payload belongs to, and that is the handle this answer returns - the same
// rule a scripted "then" entry follows, where the handle is filled in from the
// entry's own "ret".
Answer from_lobby_calling(Json ret, Json event) {
    event["call"] = ret;
    Answer answer = answered(std::move(ret));
    answer.via = kLobbyVia;
    answer.events = Json::array();
    answer.events.push_back(std::move(event));
    return answer;
}

Json lobby_created_payload(std::uint64_t lobby) {
    Json fields = Json::object();
    fields["m_eResult"] = Json(1);  // k_EResultOK
    fields["m_ulSteamIDLobby"] = id_value(lobby);

    Json event = Json::object();
    event["event"] = Json("LobbyCreated_t");
    event["in"] = std::move(fields);
    return event;
}

Json lobby_match_list_payload(std::size_t count) {
    Json fields = Json::object();
    fields["m_nLobbiesMatching"] = Json(static_cast<std::int64_t>(count));

    Json event = Json::object();
    event["event"] = Json("LobbyMatchList_t");
    event["in"] = std::move(fields);
    return event;
}

Json lobby_enter_payload(std::uint64_t lobby) {
    Json fields = Json::object();
    fields["m_ulSteamIDLobby"] = id_value(lobby);
    fields["m_rgfChatPermissions"] = Json(0);
    fields["m_bLocked"] = Json(false);
    fields["m_EChatRoomEnterResponse"] = Json(1);  // k_EChatRoomEnterResponseSuccess

    Json event = Json::object();
    event["event"] = Json("LobbyEnter_t");
    event["in"] = std::move(fields);
    return event;
}

// A room changed under somebody who did not ask. This one has no call to complete,
// so the stub delivers it to whoever registered a callback of its size - which is
// what the real SDK does with a callback nobody called.
Json chat_update_payload(std::uint64_t lobby, std::uint64_t who, std::uint64_t making_change,
                         std::uint32_t state_change) {
    Json fields = Json::object();
    fields["m_ulSteamIDLobby"] = id_value(lobby);
    fields["m_ulSteamIDUserChanged"] = id_value(who);
    fields["m_ulSteamIDMakingChange"] = id_value(making_change);
    fields["m_rgfChatMemberStateChange"] = Json(static_cast<std::int64_t>(state_change));

    Json event = Json::object();
    event["event"] = Json("LobbyChatUpdate_t");
    event["in"] = std::move(fields);
    return event;
}

// "go and re-read this room": Steam sends both when a roster changes, and a game
// that only listens for one of them is a game that never notices. Spacewar listens
// for this one.
Json data_update_payload(std::uint64_t lobby, std::uint64_t member) {
    Json fields = Json::object();
    fields["m_ulSteamIDLobby"] = id_value(lobby);
    fields["m_ulSteamIDMember"] = id_value(member);
    fields["m_bSuccess"] = Json(true);

    Json event = Json::object();
    event["event"] = Json("LobbyDataUpdate_t");
    event["in"] = std::move(fields);
    return event;
}

// "this room has a game server, and here it is": what the other members are told once
// one of them puts a server up, which is what a client connects to.
Json lobby_game_created_payload(const Lobby& lobby) {
    Json fields = Json::object();
    fields["m_ulSteamIDLobby"] = id_value(lobby.id);
    fields["m_unIP"] = Json(static_cast<std::int64_t>(lobby.game_server_ip));
    fields["m_usPort"] = Json(static_cast<std::int64_t>(lobby.game_server_port));
    fields["m_ulSteamIDGameServer"] = id_value(lobby.game_server_id);

    Json event = Json::object();
    event["event"] = Json("LobbyGameCreated_t");
    event["in"] = std::move(fields);
    return event;
}

// EChatMemberStateChange
constexpr std::uint32_t kMemberEntered = 0x0001;
constexpr std::uint32_t kMemberLeft = 0x0002;

// The id an anonymous game server is known by, told from a player's by the account type in
// the top bits - which is the only thing the wire carries about either.
constexpr std::uint64_t kGameServerAccountType = 4;

bool is_game_server_id(std::uint64_t id) noexcept {
    return ((id >> 52) & 0xFu) == kGameServerAccountType;
}

// "somebody wants to talk to you": the callback a game answers by accepting the session,
// which is what a server does before it will serve whoever just knocked.
Json session_request_payload(std::uint64_t remote) {
    Json fields = Json::object();
    fields["m_steamIDRemote"] = id_value(remote);

    Json event = Json::object();
    event["event"] = Json("P2PSessionRequest_t");
    event["in"] = std::move(fields);
    return event;
}

constexpr const char* kCreateLobby = "SteamAPI_ISteamMatchmaking_CreateLobby";
constexpr const char* kRequestLobbyList = "SteamAPI_ISteamMatchmaking_RequestLobbyList";
constexpr const char* kGetLobbyByIndex = "SteamAPI_ISteamMatchmaking_GetLobbyByIndex";
constexpr const char* kJoinLobby = "SteamAPI_ISteamMatchmaking_JoinLobby";
constexpr const char* kLeaveLobby = "SteamAPI_ISteamMatchmaking_LeaveLobby";
constexpr const char* kGetNumLobbyMembers = "SteamAPI_ISteamMatchmaking_GetNumLobbyMembers";
constexpr const char* kGetLobbyMemberByIndex = "SteamAPI_ISteamMatchmaking_GetLobbyMemberByIndex";
constexpr const char* kGetLobbyOwner = "SteamAPI_ISteamMatchmaking_GetLobbyOwner";
constexpr const char* kGetLobbyData = "SteamAPI_ISteamMatchmaking_GetLobbyData";
constexpr const char* kSetLobbyData = "SteamAPI_ISteamMatchmaking_SetLobbyData";
constexpr const char* kGetLobbyDataCount = "SteamAPI_ISteamMatchmaking_GetLobbyDataCount";
constexpr const char* kGetLobbyMemberData = "SteamAPI_ISteamMatchmaking_GetLobbyMemberData";
constexpr const char* kSetLobbyMemberData = "SteamAPI_ISteamMatchmaking_SetLobbyMemberData";
constexpr const char* kGetLobbyMemberLimit = "SteamAPI_ISteamMatchmaking_GetLobbyMemberLimit";
constexpr const char* kSetLobbyMemberLimit = "SteamAPI_ISteamMatchmaking_SetLobbyMemberLimit";
constexpr const char* kSetLobbyJoinable = "SteamAPI_ISteamMatchmaking_SetLobbyJoinable";
constexpr const char* kSetLobbyType = "SteamAPI_ISteamMatchmaking_SetLobbyType";
constexpr const char* kRequestLobbyData = "SteamAPI_ISteamMatchmaking_RequestLobbyData";
constexpr const char* kSetLobbyGameServer = "SteamAPI_ISteamMatchmaking_SetLobbyGameServer";
constexpr const char* kGetLobbyGameServer = "SteamAPI_ISteamMatchmaking_GetLobbyGameServer";
constexpr const char* kGetFriendPersonaName = "SteamAPI_ISteamFriends_GetFriendPersonaName";
constexpr const char* kGameServerInit = "SteamInternal_GameServer_Init";
constexpr const char* kGameServerGetSteamID = "SteamAPI_ISteamGameServer_GetSteamID";
constexpr const char* kSendP2PPacket = "SteamAPI_ISteamNetworking_SendP2PPacket";
constexpr const char* kIsP2PPacketAvailable = "SteamAPI_ISteamNetworking_IsP2PPacketAvailable";
constexpr const char* kReadP2PPacket = "SteamAPI_ISteamNetworking_ReadP2PPacket";
constexpr const char* kAcceptP2PSession = "SteamAPI_ISteamNetworking_AcceptP2PSessionWithUser";
constexpr const char* kCloseP2PSession = "SteamAPI_ISteamNetworking_CloseP2PSessionWithUser";

// The handle a process's game server answers under, which is what tells its
// ISteamNetworking from the customer's: one version string, one set of calls, and this
// as the only thing between them. The customer's handle is not here - a session already
// answers SteamAPI_GetHSteamUser, and the world is only where the second one comes from.
constexpr const char* kGameServerGetHSteamUser = "SteamGameServer_GetHSteamUser";
constexpr const char* kGameServerGetHSteamPipe = "SteamGameServer_GetHSteamPipe";

// 127.0.0.1 the way Steam hands addresses around: host order, so a game that reads it and
// passes it to inet_ntoa or htonl sees the machine it is standing on.
constexpr std::uint32_t kLoopback = 0x7F000001u;

}  // namespace

// ---------------------------------------------------------------------------
//  Answering from the rooms.
// ---------------------------------------------------------------------------

std::vector<std::string> LobbyWorld::handled_calls() {
    return {
        kCreateLobby,
        kRequestLobbyList,
        kGetLobbyByIndex,
        kJoinLobby,
        kLeaveLobby,
        kGetNumLobbyMembers,
        kGetLobbyMemberByIndex,
        kGetLobbyOwner,
        kGetLobbyData,
        kSetLobbyData,
        kGetLobbyDataCount,
        kGetLobbyMemberData,
        kSetLobbyMemberData,
        kGetLobbyMemberLimit,
        kSetLobbyMemberLimit,
        kSetLobbyJoinable,
        kSetLobbyType,
        kRequestLobbyData,
        kSetLobbyGameServer,
        kGetLobbyGameServer,
        kGetFriendPersonaName,
        kGameServerGetSteamID,
        kSendP2PPacket,
        kIsP2PPacketAvailable,
        kReadP2PPacket,
        kAcceptP2PSession,
        kCloseP2PSession,
        kGameServerGetHSteamUser,
        kGameServerGetHSteamPipe,
    };
}

Lobby* LobbyWorld::find_lobby(std::uint64_t id) noexcept {
    for (auto& lobby : _lobbies) {
        if (lobby.id == id) {
            return &lobby;
        }
    }
    return nullptr;
}

const Lobby* LobbyWorld::find_lobby(std::uint64_t id) const noexcept {
    for (const auto& lobby : _lobbies) {
        if (lobby.id == id) {
            return &lobby;
        }
    }
    return nullptr;
}

const LobbyMember* LobbyWorld::find_member_anywhere(std::uint64_t steam_id) const noexcept {
    for (const auto& lobby : _lobbies) {
        if (const LobbyMember* member = lobby.find_member(steam_id); member != nullptr) {
            return member;
        }
    }
    return nullptr;
}

std::uint64_t LobbyWorld::known_game_server_id(std::uint64_t user) const noexcept {
    for (const auto& entry : _game_server_ids) {
        if (entry.first == user) {
            return entry.second;
        }
    }
    return 0;
}

std::uint64_t LobbyWorld::game_server_id_of(std::uint64_t user) {
    const std::uint64_t known = known_game_server_id(user);
    if (known != 0) {
        return known;
    }
    // Minted on the first ask, so a second server in the same run cannot be handed the id
    // the first one is already known by.
    const std::uint64_t minted = kFirstGameServerId + _game_server_ids.size();
    _game_server_ids.emplace_back(user, minted);
    return minted;
}

std::uint64_t LobbyWorld::user_of(std::uint64_t id) const noexcept {
    for (const auto& entry : _game_server_ids) {
        if (entry.second == id) {
            return entry.first;
        }
    }
    return id;
}

bool LobbyWorld::needs_session_request(std::uint64_t from, std::uint64_t to) {
    for (const auto& pair : _contacts) {
        if (pair.first == from && pair.second == to) {
            return false;  // this direction has already been announced
        }
        if (pair.first == to && pair.second == from) {
            return false;  // they have talked, so Steam would not ask again
        }
    }
    _contacts.emplace_back(from, to);
    return true;
}

void LobbyWorld::queue_packet(std::uint64_t to, std::uint64_t from, std::int32_t channel,
                              const std::string& bytes) {
    P2PPacket packet;
    packet.remote = from;
    packet.channel = channel;
    packet.bytes = bytes;
    for (auto& entry : _packets) {
        if (entry.first == to) {
            entry.second.push_back(std::move(packet));
            return;
        }
    }
    _packets.emplace_back(to, std::vector<P2PPacket>{std::move(packet)});
}

std::uint64_t LobbyWorld::endpoint_of(std::uint64_t user, std::int32_t hSteamUser) const noexcept {
    // A session that hosts is a customer and a game server at once and has an id for
    // each, which are the two queues a packet can be waiting in. Which of them a call is
    // for is the handle it was made through, and that is the only thing that survives the
    // trip: the two interfaces answer to one name and neither takes a user.
    if (hSteamUser == kGameServerHSteamUser) {
        const std::uint64_t server = known_game_server_id(user);
        if (server != 0) {
            return server;
        }
    }
    // A handle this world never gave out, or none at all: the session's own end, which is
    // the one thing a process without a game server in it has.
    return user;
}

const P2PPacket* LobbyWorld::peek_packet(std::uint64_t user, std::int32_t hSteamUser,
                                         std::int32_t channel) const noexcept {
    const std::uint64_t end = endpoint_of(user, hSteamUser);
    for (const auto& entry : _packets) {
        if (entry.first != end) {
            continue;
        }
        for (const P2PPacket& packet : entry.second) {
            if (packet.channel == channel) {
                return &packet;
            }
        }
    }
    return nullptr;
}

void LobbyWorld::drop_packet(std::uint64_t user, std::int32_t hSteamUser, std::int32_t channel) {
    const std::uint64_t end = endpoint_of(user, hSteamUser);
    for (auto& entry : _packets) {
        if (entry.first != end) {
            continue;
        }
        for (auto packet = entry.second.begin(); packet != entry.second.end(); ++packet) {
            if (packet->channel == channel) {
                entry.second.erase(packet);
                return;
            }
        }
    }
}

void LobbyWorld::notify_member_change(
    const Lobby& lobby, const LobbyMember& member, std::uint64_t making_change,
    std::uint32_t state_change, std::vector<std::pair<std::uint64_t, Json>>& notifications) const {
    // Everyone still in the room hears about it, including whoever did it: Steam
    // tells the game that changed its own membership too, and a client that only
    // heard about the others would have to special-case itself. Both callbacks go
    // out, because which one a game listens for is the game's business.
    for (const LobbyMember& other : lobby.members) {
        notifications.emplace_back(
            other.steam_id,
            chat_update_payload(lobby.id, member.steam_id, making_change, state_change));
        notifications.emplace_back(other.steam_id, data_update_payload(lobby.id, member.steam_id));
    }
}

bool LobbyWorld::answer(const Session& session, const std::string& call, const Json& args,
                        Answer& out, std::vector<std::pair<std::uint64_t, Json>>& notifications) {
    const std::uint64_t me = session.profile().steam_id;

    if (call == kGameServerInit) {
        // Not answered here - the scenario decides whether a game server comes up at all -
        // but listened to, because the port a game asked for when it started a server is the
        // port its server has to be reached on when it publishes one without saying where.
        const std::int64_t port = int_member(args, "usGamePort", 0);
        if (port > 0) {
            bool known = false;
            for (auto& entry : _game_ports) {
                if (entry.first == me) {
                    entry.second = static_cast<std::uint16_t>(port);
                    known = true;
                    break;
                }
            }
            if (!known) {
                _game_ports.emplace_back(me, static_cast<std::uint16_t>(port));
            }
        }
        return false;
    }

    if (call == kCreateLobby) {
        Lobby lobby;
        lobby.id = _next_lobby_id++;
        lobby.owner = me;
        lobby.max_members = int_member(args, "cMaxMembers", 4);
        if (lobby.max_members <= 0) {
            lobby.max_members = 4;
        }
        LobbyMember owner;
        owner.steam_id = me;
        owner.persona = session.profile().persona_name;
        lobby.members.push_back(std::move(owner));
        _lobbies.push_back(std::move(lobby));
        out = from_lobby_calling(id_value(_next_call++), lobby_created_payload(_lobbies.back().id));
        return true;
    }

    if (call == kRequestLobbyList) {
        out = from_lobby_calling(id_value(_next_call++), lobby_match_list_payload(_lobbies.size()));
        return true;
    }

    if (call == kGetLobbyByIndex) {
        const std::int64_t index = int_member(args, "iLobby", 0);
        if (index < 0 || index >= static_cast<std::int64_t>(_lobbies.size())) {
            // Steam hands back an invalid id for an index it does not have, which is
            // what fills a browser with nothing rather than with stale rooms.
            out = from_lobby(id_value(0));
            return true;
        }
        out = from_lobby(id_value(_lobbies[static_cast<std::size_t>(index)].id));
        return true;
    }

    if (call == kJoinLobby) {
        const std::uint64_t lobby_id = id_member(args, "steamIDLobby");
        Lobby* lobby = find_lobby(lobby_id);
        if (lobby == nullptr) {
            // A room the run has never seen: no opinion rather than an invented one.
            return false;
        }
        if (lobby->find_member(me) == nullptr) {
            LobbyMember member;
            member.steam_id = me;
            member.persona = session.profile().persona_name;
            // Steam keeps a "name" for every member whether or not the game set one,
            // and a roster that has to ask for a name is the common case.
            member.set("name", session.profile().persona_name);
            lobby->members.push_back(std::move(member));
            notify_member_change(*lobby, lobby->members.back(), me, kMemberEntered, notifications);
        }
        out = from_lobby_calling(id_value(_next_call++), lobby_enter_payload(lobby->id));
        return true;
    }

    if (call == kLeaveLobby) {
        const std::uint64_t lobby_id = id_member(args, "steamIDLobby");
        Lobby* lobby = find_lobby(lobby_id);
        if (lobby == nullptr) {
            return false;
        }
        for (std::size_t i = 0; i < lobby->members.size(); ++i) {
            if (lobby->members[i].steam_id == me) {
                const LobbyMember leaving = lobby->members[i];
                lobby->members.erase(lobby->members.begin() + static_cast<std::ptrdiff_t>(i));
                notify_member_change(*lobby, leaving, me, kMemberLeft, notifications);
                break;
            }
        }
        out = from_lobby(Json(true));
        return true;
    }

    if (call == kGetNumLobbyMembers) {
        const Lobby* lobby = find_lobby(id_member(args, "steamIDLobby"));
        if (lobby == nullptr) {
            return false;
        }
        out = from_lobby(Json(static_cast<std::int64_t>(lobby->members.size())));
        return true;
    }

    if (call == kGetLobbyMemberByIndex) {
        const Lobby* lobby = find_lobby(id_member(args, "steamIDLobby"));
        if (lobby == nullptr) {
            return false;
        }
        const std::int64_t index = int_member(args, "iMember", 0);
        if (index < 0 || index >= static_cast<std::int64_t>(lobby->members.size())) {
            out = from_lobby(id_value(0));
            return true;
        }
        out = from_lobby(id_value(lobby->members[static_cast<std::size_t>(index)].steam_id));
        return true;
    }

    if (call == kGetLobbyOwner) {
        const Lobby* lobby = find_lobby(id_member(args, "steamIDLobby"));
        if (lobby == nullptr) {
            return false;
        }
        out = from_lobby(id_value(lobby->owner));
        return true;
    }

    if (call == kGetLobbyData) {
        const Lobby* lobby = find_lobby(id_member(args, "steamIDLobby"));
        if (lobby == nullptr) {
            return false;
        }
        const std::string* value = lobby->find_data(string_member(args, "pchKey"));
        out = from_lobby(Json(value != nullptr ? *value : std::string()));
        return true;
    }

    if (call == kSetLobbyData) {
        Lobby* lobby = find_lobby(id_member(args, "steamIDLobby"));
        if (lobby == nullptr) {
            return false;
        }
        lobby->set_data(string_member(args, "pchKey"), string_member(args, "pchValue"));
        out = from_lobby(Json(true));
        return true;
    }

    if (call == kGetLobbyDataCount) {
        const Lobby* lobby = find_lobby(id_member(args, "steamIDLobby"));
        if (lobby == nullptr) {
            return false;
        }
        out = from_lobby(Json(static_cast<std::int64_t>(lobby->data.size())));
        return true;
    }

    if (call == kGetLobbyMemberData) {
        const Lobby* lobby = find_lobby(id_member(args, "steamIDLobby"));
        if (lobby == nullptr) {
            return false;
        }
        // Steam's member-data calls are about the member the caller is: the SDK's own
        // declaration carries no user id, and the wire shows games not sending one. The
        // roster is the exception, so a named member wins where there is one.
        const std::uint64_t named = id_member(args, "steamIDUser");
        const LobbyMember* member = lobby->find_member(named != 0 ? named : me);
        if (member == nullptr) {
            return false;
        }
        const std::string key = string_member(args, "pchKey");
        if (const std::string* value = member->find(key); value != nullptr) {
            out = from_lobby(Json(*value));
            return true;
        }
        // A key nobody set: the member's own name is the answer a roster is after,
        // and the alternative is a list of blank rows. Steam would say "" here, so
        // this is the one place the world is more generous than the real thing.
        out = from_lobby(Json(member->persona));
        return true;
    }

    if (call == kSetLobbyMemberData) {
        Lobby* lobby = find_lobby(id_member(args, "steamIDLobby"));
        if (lobby == nullptr) {
            return false;
        }
        const std::uint64_t named = id_member(args, "steamIDUser");
        LobbyMember* member = lobby->find_member(named != 0 ? named : me);
        if (member == nullptr) {
            return false;
        }
        member->set(string_member(args, "pchKey"), string_member(args, "pchValue"));
        out = from_lobby(Json(true));
        return true;
    }

    if (call == kGetLobbyMemberLimit) {
        const Lobby* lobby = find_lobby(id_member(args, "steamIDLobby"));
        if (lobby == nullptr) {
            return false;
        }
        out = from_lobby(Json(lobby->max_members));
        return true;
    }

    if (call == kSetLobbyMemberLimit) {
        Lobby* lobby = find_lobby(id_member(args, "steamIDLobby"));
        if (lobby == nullptr) {
            return false;
        }
        const std::int64_t limit = int_member(args, "cMaxMembers", lobby->max_members);
        if (limit > 0) {
            lobby->max_members = limit;
        }
        out = from_lobby(Json(true));
        return true;
    }

    if (call == kSetLobbyJoinable || call == kSetLobbyType) {
        // The room is recorded, but neither of these changes what the run can be
        // asked: nothing here filters a list or refuses a join yet, and a game that
        // is told "not joinable" and then joins anyway would be a worse lie.
        if (find_lobby(id_member(args, "steamIDLobby")) == nullptr) {
            return false;
        }
        out = from_lobby(Json(true));
        return true;
    }

    if (call == kRequestLobbyData) {
        if (find_lobby(id_member(args, "steamIDLobby")) == nullptr) {
            return false;
        }
        out = from_lobby(Json(true));
        return true;
    }

    if (call == kSetLobbyGameServer) {
        Lobby* lobby = find_lobby(id_member(args, "steamIDLobby"));
        if (lobby == nullptr) {
            return false;
        }
        lobby->has_game_server = true;
        lobby->game_server_ip = static_cast<std::uint32_t>(int_member(args, "unGameServerIP", 0));
        lobby->game_server_port =
            static_cast<std::uint16_t>(int_member(args, "unGameServerPort", 0));
        lobby->game_server_id = id_member(args, "steamIDGameServer");

        // A game is allowed to publish its server without saying where it is and expect
        // Steam to fill that in. Spacewar does exactly that, and the client it then sends
        // to has nothing to dial - which is what "Failed sending data to server" is. The
        // address a run is reachable at is the machine it is standing on, and the port is
        // the one the game itself asked for when it started the server.
        if (lobby->game_server_ip == 0) {
            lobby->game_server_ip = kLoopback;
        }
        if (lobby->game_server_port == 0) {
            for (const auto& entry : _game_ports) {
                if (entry.first == me) {
                    lobby->game_server_port = entry.second;
                    break;
                }
            }
        }
        if (lobby->game_server_id == 0) {
            // The id is the other half of the same thing. A client addresses its packets to
            // the game server's own id, and a payload that leaves it zero leaves the client
            // it reaches with nothing to dial - which is why a guest that was told about a
            // game server sat on its lobby screen and never tried.
            lobby->game_server_id = game_server_id_of(me);
        }

        // Everyone in the room hears about it, including the game that put the server up:
        // it is a client as well as a host, and the one thing a client needs to connect is
        // the address in this payload. Steam tells the setter too, and a host that only
        // heard about the others would have to special-case itself out of its own room.
        for (const LobbyMember& other : lobby->members) {
            notifications.emplace_back(other.steam_id, lobby_game_created_payload(*lobby));
        }
        out = from_lobby(Json(true));
        return true;
    }

    if (call == kGetLobbyGameServer) {
        const Lobby* lobby = find_lobby(id_member(args, "steamIDLobby"));
        if (lobby == nullptr || !lobby->has_game_server) {
            // A room with no server yet is not an error: it is a room whose owner has
            // not started the game, and Steam reports exactly that.
            return false;
        }
        Json values = Json::object();
        values["punGameServerIP"] = Json(static_cast<std::int64_t>(lobby->game_server_ip));
        values["punGameServerPort"] = Json(static_cast<std::int64_t>(lobby->game_server_port));
        values["psteamIDGameServer"] = id_value(lobby->game_server_id);
        Answer answer = from_lobby(Json(true));
        answer.out = std::move(values);
        out = std::move(answer);
        return true;
    }

    if (call == kGetFriendPersonaName) {
        // The roster asks for the people in it by Steam id, and the only ids this
        // run knows the names of are the ones that joined something.
        const LobbyMember* member = find_member_anywhere(id_member(args, "steamIDFriend"));
        if (member == nullptr || member->persona.empty()) {
            return false;
        }
        out = from_lobby(Json(member->persona));
        return true;
    }

    if (call == kGameServerGetSteamID) {
        // The identity a server is known by, which is what a client addresses its packets to
        // and what the room published as its game server.
        out = from_lobby(id_value(game_server_id_of(me)));
        return true;
    }

    if (call == kSendP2PPacket) {
        // The bytes arrive as hex, which is how the wire carries a buffer - see the layouts'
        // "bytes" kind and the marshalling in bridge/synth.hpp. Nothing is checked about the
        // destination: Steam would refuse a peer it does not know, but a run that did would
        // be inventing a rule the games cannot see anyway.
        const std::uint64_t to = id_member(args, "steamIDRemote");
        const std::int32_t channel = static_cast<std::int32_t>(int_member(args, "nChannel", 0));
        // Who the receiver is told this came from: a client is a player to the server it
        // talks to, and a server is the game server to the client that dialled it - the id
        // the other side knows it by, and the one it can answer to.
        const std::uint64_t mine = is_game_server_id(to) ? me : known_game_server_id(me);
        const std::uint64_t from = mine != 0 ? mine : me;
        const std::uint64_t recipient = user_of(to);

        queue_packet(to, from, channel, string_member(args, "pubData"));

        // Steam asks a game whether it will talk to a peer it has not heard from before, and
        // a server that is never asked has no client to hand the packet to - which is what
        // "Received unknown message on our listen socket" is.
        if (needs_session_request(from, recipient)) {
            notifications.emplace_back(recipient, session_request_payload(from));
        }
        out = from_lobby(Json(true));
        return true;
    }

    if (call == kGameServerGetHSteamUser || call == kGameServerGetHSteamPipe) {
        // The handle this process's game server answers under. Real Steam gives the game
        // server a user of its own beside the customer's, and that handle is what a packet
        // read through ISteamNetworking can be attributed back to. The customer's own end
        // is answered by the session, which is where it has always been answered.
        out = from_lobby(Json(static_cast<std::int64_t>(kGameServerHSteamUser)));
        return true;
    }

    if (call == kIsP2PPacketAvailable) {
        const std::int32_t channel = static_cast<std::int32_t>(int_member(args, "nChannel", 0));
        const std::int32_t user = static_cast<std::int32_t>(int_member(args, "hSteamUser", 0));
        const P2PPacket* packet = peek_packet(me, user, channel);
        if (packet == nullptr) {
            // An empty queue is not an opinion about anything: the stub's own default
            // already answers "nothing waiting", which is what a game expects.
            return false;
        }
        Json values = Json::object();
        values["pcubMsgSize"] = Json(static_cast<std::int64_t>(packet->bytes.size() / 2u));
        Answer answer = from_lobby(Json(true));
        answer.out = std::move(values);
        out = std::move(answer);
        return true;
    }

    if (call == kReadP2PPacket) {
        const std::int32_t channel = static_cast<std::int32_t>(int_member(args, "nChannel", 0));
        const std::int32_t user = static_cast<std::int32_t>(int_member(args, "hSteamUser", 0));
        const P2PPacket* packet = peek_packet(me, user, channel);
        if (packet == nullptr) {
            return false;
        }
        // Copied out before the queue drops it, because the next read is a different
        // packet: the bytes go back as hex and the stub writes them into the game's buffer.
        Json values = Json::object();
        values["pubDest"] = Json(packet->bytes);
        values["pcubMsgSize"] = Json(static_cast<std::int64_t>(packet->bytes.size() / 2u));
        values["psteamIDRemote"] = id_value(packet->remote);
        drop_packet(me, user, channel);
        Answer answer = from_lobby(Json(true));
        answer.out = std::move(values);
        out = std::move(answer);
        return true;
    }

    if (call == kAcceptP2PSession || call == kCloseP2PSession) {
        // Nothing here refuses a peer or holds a session open. A packet either reaches the
        // game it was addressed to or it does not, and Steam's session bookkeeping is not
        // something a run has to model to be believed.
        out = from_lobby(Json(true));
        return true;
    }

    return false;
}

}  // namespace steammock
