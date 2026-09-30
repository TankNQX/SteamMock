#pragma once

// ---------------------------------------------------------------------------
//  The socket plumbing both ends of the loopback link need.
// ---------------------------------------------------------------------------
//  Winsock wants a process-wide startup, the same "handles live in an integer"
//  trick, and a send that finishes what it started. The backend's server and the
//  stub's transport each carried their own copy of every one of those, which is
//  two places for the same handle lore to go stale and - for the startup, once per
//  process rather than once per translation unit - one place where a plain flag
//  read and set was not the same thing as a once.
//
//  The receive side is deliberately *not* here. The two want different things from
//  it: the client blocks in recv() and lets its socket's own timeout end the wait,
//  while the server reads in slices so it can look at the stopping flag between
//  them. A single function would have to grow a deadline the client already has,
//  and that is a worse thing to share than two functions are to keep apart.

#include <cstddef>
#include <cstdint>

#ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>

#include "bridge/log.hpp"

namespace steammock {
namespace socket_io {

// Winsock's own handle type. It is a pointer-sized unsigned integer, which is why
// the bridge carries one as a std::uintptr_t wherever it has to cross a header that
// does not include winsock.
using socket_t = SOCKET;

inline constexpr socket_t kInvalidSocket = INVALID_SOCKET;

// Once, however many threads arrive here: a function-local static's initialisation
// is the one thing the language already serialises. It answers whether the startup
// worked, because every caller that got a true here can still fail to make a socket
// work and has to be able to say which of the two it was: a WSAStartup that failed
// used to be a line in the log and a `true` on the way out, so a server whose socket
// layer never came up reported "cannot bind" - or, with a port of 0, said "listening"
// and then never accepted anything. Once only, deliberately: a startup that failed is
// not something the next call should try again.
inline bool ensure_winsock_started() noexcept {
    static const bool started = []() noexcept {
        WSADATA data{};
        if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
            log_write(LogLevel::error,
                      "WSAStartup failed: no socket will work in this process, and none "
                      "will be attempted again");
            return false;
        }
        return true;
    }();
    return started;
}

inline socket_t as_socket(std::uintptr_t value) noexcept { return static_cast<socket_t>(value); }

inline void close_socket(socket_t handle) noexcept {
    if (handle == kInvalidSocket) {
        return;
    }
    (void)closesocket(handle);
}

// Sends everything or nothing: a partial frame desyncs the stream for every call
// that follows it on this connection, so a short write is a failure here rather
// than something the caller has to finish.
//
// A `send` that the OS interrupted before it wrote anything is not a failure of the
// connection: WSAEINTR means "ask again". It used to be read as one, which drops a
// perfectly good connection - and, on the client, is the difference between a call
// that answered and a game that falls back to its defaults for no reason.
inline bool send_all(socket_t handle, const char* data, std::size_t size) noexcept {
    std::size_t sent = 0;
    while (sent < size) {
        const std::size_t remaining = size - sent;
        const int chunk = static_cast<int>(remaining > 0x7FFFFFFFu ? 0x7FFFFFFFu : remaining);
        const int written = ::send(handle, data + sent, chunk, 0);
        if (written > 0) {
            sent += static_cast<std::size_t>(written);
            continue;
        }
        if (written < 0 && WSAGetLastError() == WSAEINTR) {
            continue;  // interrupted before a byte went out: the same call made again
        }
        return false;  // a closed socket, a timeout, or a real error
    }
    return true;
}

// Half-closing wakes a thread parked in recv/accept without tearing the handle out
// from under it, which is what makes a clean stop possible at all.
//
// SD_RECEIVE and not SD_BOTH: what the stop path wants is to end the wait, and the
// send direction is not its to abandon - a worker that is mid-reply when the server
// stops still gets to finish writing it. A full shutdown would fail that send instead,
// for no gain: the socket is closed by its own thread a moment later either way.
inline void shutdown_socket(socket_t handle) noexcept {
    if (handle == kInvalidSocket) {
        return;
    }
    (void)::shutdown(handle, SD_RECEIVE);
}

}  // namespace socket_io
}  // namespace steammock
