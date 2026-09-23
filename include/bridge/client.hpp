#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>

#include "bridge/json.hpp"
#include "bridge/transport.hpp"

namespace steammock {

// ---------------------------------------------------------------------------
//  The process-wide bridge client behind every exported stub.
// ---------------------------------------------------------------------------
//  Configuration comes from the environment, read once on first use:
//
//    STEAMMOCK_HOST        default 127.0.0.1
//    STEAMMOCK_PORT        default 50990
//    STEAMMOCK_TIMEOUT_MS  default 2000 - a game is never blocked for longer
//    STEAMMOCK_OFF         1 disables the bridge entirely
//
//  The connection is lazy and retried, so a backend started after the game gets
//  picked up on the next call. When no backend answers, the call reports "not
//  handled" and the stub falls back to its local default - the same values a
//  game sees when Steam is not running. Nothing here ever throws: an exception
//  must not cross an exported boundary into the game.
class Client {
public:
    static Client& instance() noexcept;

    // Sends one call. True means the backend answered it, and `reply` holds the
    // reply body ("ret" and "out"). False means "use your default".
    bool call(std::string_view name, const Json& args, Json& reply) noexcept;

    // Connects if needed, so the harness can probe the DLL before the game has
    // made a real API call.
    bool backend_connected() noexcept;

    const std::string& session_id() const noexcept { return _session_id; }
    unsigned call_count() const noexcept { return _call_count; }
    unsigned unhandled_count() const noexcept { return _unhandled_count; }

private:
    Client() noexcept;
    ~Client();

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    void configure();
    bool ensure_connected();

    // Which transport is in use is decided once, here, and every call goes
    // through the interface - so swapping in another one is a different object
    // to construct rather than a change to the call path.
    std::unique_ptr<Transport> _transport;
    std::mutex* _mutex = nullptr;

    std::string _host;
    std::uint16_t _port = 0;
    unsigned _timeout_ms = 0;
    bool _enabled = true;
    bool _configured = false;
    bool _logged_offline = false;

    std::string _exe_name;
    std::string _session_id;
    unsigned _sequence = 0;
    unsigned _call_count = 0;
    unsigned _unhandled_count = 0;
};

// The one call a generated trampoline makes.
bool invoke(std::string_view name, const Json& args, Json& reply) noexcept;

}  // namespace steammock
