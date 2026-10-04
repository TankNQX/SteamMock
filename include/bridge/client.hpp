#pragma once

#include <atomic>
#include <cstdint>
#include <deque>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

#include "bridge/json_read.hpp"
#include "bridge/transport.hpp"

namespace steammock
{

// ---------------------------------------------------------------------------
//  The process-wide bridge client behind every exported stub.
// ---------------------------------------------------------------------------
//  Configuration comes from the environment, read once on first use:
//
//    STEAMMOCK_HOST        default 127.0.0.1 (kDefaultHost)
//    STEAMMOCK_PORT        default 50990 (kDefaultPort)
//    STEAMMOCK_TIMEOUT_MS  default 2000 - a game is never blocked for longer, up to
//                          600000; anything else falls back to the default
//    STEAMMOCK_OFF         1 disables the bridge entirely
//
//  The connection is lazy and retried, so a backend started after the game gets
//  picked up on the next call. When no backend answers, the call reports "not
//  handled" and the stub falls back to its local default - the same values a
//  game sees when Steam is not running. Nothing here ever throws: an exception
//  must not cross an exported boundary into the game.
class Client
{
  public:
    static Client& instance() noexcept;

    // Sends one call. True means the backend answered it, and `reply` holds the
    // reply body ("ret" and "out"). False means "use your default".
    bool call(std::string_view name, const Json& args, Json& reply) noexcept;

    // Connects if needed, so the harness can probe the DLL before the game has
    // made a real API call.
    bool backend_connected() noexcept;

    // Events the backend sent with a reply and the game has not been handed yet:
    // they wait here until its own RunCallbacks pumps them. Taking one copies it
    // out and dispatches nothing, so a game that calls back into the bridge from
    // inside a callback cannot deadlock on itself.
    bool take_event(Json& out) noexcept;

    // The session id by value, not by reference: connecting is what replaces it, and
    // a reader holding the reference would be holding whatever the next connection
    // leaves there. Read under the same lock the connection writes it under, because
    // a string copied out while another thread reassigns it is not a copy of
    // anything. The counts are read from other threads - a diagnostic tool, a test -
    // and written from the one the calls run on.
    std::string session_id() const noexcept;

    // Both counts together, which is the only way they mean anything: a tool that read
    // them one at a time could see a call counted before the "left to the stub's
    // defaults" count it belongs to, and report a total that never existed. They are
    // written under `_mutex` by call(), so they are read under it here.
    struct Counts
    {
        unsigned calls = 0;
        unsigned unhandled = 0;
    };
    Counts counts() const noexcept;

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

    // Two locks, and the order between them is only ever this one: a call holds
    // `_mutex` for its round trip, and takes `_events_mutex` inside it to queue what
    // came back. Nothing takes them the other way - which is what keeps a game's own
    // pump from waiting behind another thread's call: `take_event` takes the queue's
    // lock alone, so a frame that pumps callbacks is not blocked for as long as some
    // other thread's socket is.
    //
    // `mutable`, because reading the session id is a logically-constant question that
    // still has to be asked under the same lock the connection writes it under.
    mutable std::mutex _mutex;
    std::mutex _events_mutex;

    std::string _host;
    std::uint16_t _port = 0;
    unsigned _timeout_ms = 0;
    bool _enabled = true;
    bool _configured = false;
    bool _logged_offline = false;

    std::string _exe_name;
    std::string _session_id;
    unsigned _sequence = 0;
    // Written where the calls run, read from wherever a tool asks - through counts(),
    // which takes the lock the writes happen under.
    std::atomic<unsigned> _call_count{0};
    std::atomic<unsigned> _unhandled_count{0};

    // The payloads waiting for the game's next pump, in the order they arrived.
    // Guarded by `_events_mutex` rather than by `_mutex` - see above. A deque rather
    // than a vector: taking one is a pop from the front, and a game that let a burst
    // queue up used to pay for shifting every remaining element on each take.
    std::deque<Json> _events;
};

// The one call a generated trampoline makes.
bool invoke(std::string_view name, const Json& args, Json& reply) noexcept;

} // namespace steammock
