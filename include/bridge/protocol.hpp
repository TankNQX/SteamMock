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
