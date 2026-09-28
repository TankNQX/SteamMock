#pragma once

#include <cstdint>
#include <string>

#include "bridge/json_read.hpp"

namespace steammock {

// ---------------------------------------------------------------------------
//  The messages both ends agree on.
// ---------------------------------------------------------------------------
//  The framing itself is in bridge/frame.hpp. This is the layer above it: the
//  version stamp, the reply to a call and the handshake's answer.
//
//  There used to be a Python mirror of this file (python/steammock/protocol.py)
//  and the two were kept in step by hand. Now the stub and the backend link the
//  same translation unit, so they cannot drift.

// Bumped when a message changes shape in a way an older peer would misread.
// Every message carries it.
constexpr int kProtocolVersion = 1;

// Which end of a process a payload belongs to, when both ends of one process would have a
// claim on it. A process that hosts is a customer and a game server at once, and each of
// them asks Steam to check tickets: the game server about the players connecting to it,
// the customer about the peers it is about to play with. Both answers are a
// ValidateAuthTicketResponse_t, and the object that should hear one is the one belonging to
// the end that asked - which the payload cannot say for itself, because both answers carry
// the same fields. So it says it in the event's "side", and a payload without one is the
// customer's: that is every payload but these, since a room, a packet and a connection are
// all a customer's business.
constexpr const char* kSideGameServer = "game server";
constexpr const char* kSideClient = "client";

// An `out` object is only carried when it actually says something: an empty one,
// or null, means "no out-parameters", and the protocol leaves the key out rather
// than sending an empty object for a reader to ignore. The transcript applies
// the same rule, so the two never disagree about whether a call had any.
bool carries_out(const Json& out) noexcept;

// The answer to one call. `answered == false` is the whole point of the
// protocol: the backend has no opinion, and the stub then uses the value a game
// would see with Steam not running.
Json make_reply(std::int64_t seq, bool answered, const Json& ret, const Json& out);

// The answer to the handshake: it names the session and the profile the game was
// matched to, so a game's log can say which debugging identity it got.
Json make_welcome(const std::string& session_id, const std::string& profile_name);

}  // namespace steammock
