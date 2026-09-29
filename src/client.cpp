#include "bridge/client.hpp"

#include <cstdlib>
#include <string>

#include "bridge/defaults.hpp"
#include "bridge/log.hpp"
#include "bridge/protocol.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace steammock {
namespace {

// The port and the timeout a game gets when nothing says otherwise, and the
// largest either may be: a sanity bound rather than a protocol one, so a typo in
// the environment is a fallback to the default instead of a game parked on a
// socket for an hour. Both live beside kDefaultPort, which the backend and both
// front ends also read - see bridge/defaults.hpp.
constexpr unsigned kMaxPort = 65535u;
constexpr unsigned kDefaultTimeoutMs = 2000u;
constexpr unsigned kMaxTimeoutMs = 600000u;

std::string environment(const char* name) {
    const char* value = std::getenv(name);
    return value != nullptr ? std::string(value) : std::string();
}

unsigned environment_number(const char* name, unsigned ceiling, unsigned fallback) {
    unsigned value = 0;
    return parse_number(environment(name), ceiling, value) ? value : fallback;
}

std::string executable_path() {
    char buffer[MAX_PATH] = {};
    // The length it returns, not a zero test: GetModuleFileNameA answers with the
    // buffer size when the path did not fit, and the path it wrote is then a
    // truncated one that names a file that is not this DLL's - so it is treated as
    // a failure rather than used for the log prefix and the exe name a scenario
    // matches on.
    const DWORD length = GetModuleFileNameA(nullptr, buffer, sizeof(buffer));
    if (length == 0 || length >= sizeof(buffer)) {
        return std::string();
    }
    return std::string(buffer, length);
}

std::string file_name_of(const std::string& path) {
    const std::size_t slash = path.find_last_of("\\/");
    return slash == std::string::npos ? path : path.substr(slash + 1u);
}

}  // namespace

Client& Client::instance() noexcept {
    // Never destroyed on purpose: the process is ending anyway, and a leak is
    // cheaper than a shutdown-order race inside a DLL - a static local would be
    // destroyed under the loader lock.
    //
    // If this allocation fails there is nothing to fall back to: the function
    // has to return a reference. Terminating is the only outcome available, so
    // the suppression below records that decision rather than hiding it.
    // NOLINTNEXTLINE(bugprone-unhandled-exception-at-new)
    static Client* client = new Client();
    return *client;
}

// Reached only from instance(), so a failed allocation here is the same
// fatal-by-design case as the one above.
// NOLINTNEXTLINE(bugprone-unhandled-exception-at-new)
Client::Client() noexcept : _transport(new TcpTransport()) {}

Client::~Client() { _transport->close(); }

// Deliberately not noexcept: this allocates, and a failure here is worth
// catching in call(), which answers the game with a default. A noexcept here
// would turn that same failure into std::terminate - the opposite of what this
// harness promises a game.
void Client::configure() {
    if (_configured) {
        return;
    }
    // `_configured` is set at the end of each path below, not here: everything in
    // between allocates (the environment strings, the log line), and `call()` catches
    // a failure and answers the game with a default. Recording the client as
    // configured before it had a host, a port and a timeout left every later call
    // short-circuiting through here and then running on an empty host and port 0 -
    // one failed allocation, and the bridge was silently dead for the whole run.
    // Setting the flag last means a first configure that failed is simply tried again.

    const std::string path = executable_path();
    _exe_name = file_name_of(path);
    log_configure(path.c_str());

    if (environment("STEAMMOCK_OFF") == "1") {
        _enabled = false;
        _configured = true;
        log_write(LogLevel::info, "STEAMMOCK_OFF=1 - every call answers with its default");
        return;
    }

    _host = environment("STEAMMOCK_HOST");
    if (_host.empty()) {
        _host = kDefaultHost;
    }
    _port =
        static_cast<std::uint16_t>(environment_number("STEAMMOCK_PORT", kMaxPort, kDefaultPort));
    _timeout_ms = environment_number("STEAMMOCK_TIMEOUT_MS", kMaxTimeoutMs, kDefaultTimeoutMs);
    if (_timeout_ms == 0u) {
        // A zero is not "wait forever" to this transport - it is a deadline that has
        // already passed, so nothing would ever connect. The variable's contract is a
        // number of milliseconds a game is never blocked for longer than, and its
        // fallback is the default, so a zero reads as that default rather than as a
        // bridge that is silently dead.
        _timeout_ms = kDefaultTimeoutMs;
    }
    _transport->set_timeout_ms(_timeout_ms);

    // Built before the flag is set, because building it is the last thing that can
    // fail: a log line about a target the client never finished adopting is worse
    // than no line at all.
    const std::string target = "backend target " + _host + ":" + std::to_string(_port);
    _configured = true;
    log_write(LogLevel::debug, target);
}

// Not noexcept, for the same reason as configure().
bool Client::ensure_connected() {
    if (_transport->is_connected()) {
        return true;
    }
    if (!_transport->connect(_host, _port)) {
        if (!_logged_offline) {
            log_write(LogLevel::info, "no backend listening on " + _host + ":" +
                                          std::to_string(_port) +
                                          " - every call will answer with its default");
            _logged_offline = true;
        }
        return false;
    }

    Json hello = Json::object();
    hello["type"] = Json("hello");
    hello["v"] = Json(kProtocolVersion);
    hello["exe"] = Json(_exe_name);
    hello["arch"] = Json(sizeof(void*) == 8u ? "x64" : "x86");
    hello["module"] = Json("steam_api stub");
    hello["pid"] = Json(static_cast<std::int64_t>(GetCurrentProcessId()));
    // Which game this one is pretending to be, when a single scenario has to
    // describe two of them running at once: the exe name cannot tell two Spacewars
    // apart, and a pid cannot be written down in advance.
    char wanted[64] = {};
    const DWORD wanted_length =
        GetEnvironmentVariableA("STEAMMOCK_PROFILE", wanted, sizeof(wanted));
    if (wanted_length > 0 && wanted_length < sizeof(wanted)) {
        hello["profile"] = Json(wanted);
    }

    std::string response;
    if (!_transport->exchange(hello.dump(), response)) {
        log_write(LogLevel::warn, "hello failed - ignoring it");
        _transport->close();
        return false;
    }
    Json welcome;
    const bool parsed = parse(response, welcome);
    const Json* session = parsed ? json_member(welcome, "session") : nullptr;
    if (session == nullptr) {
        log_write(LogLevel::warn, "the backend did not answer the handshake - ignoring it");
        _transport->close();
        return false;
    }
    _session_id = as_string(*session);
    _logged_offline = false;
    log_write(LogLevel::info, "connected to the backend, session " + _session_id);
    return true;
}

bool Client::call(std::string_view name, const Json& args, Json& reply) noexcept {
    try {

        // Game calls arrive on whatever thread the game uses, so one round trip
        // is held under the lock. Steam callbacks are cheap and the transport is
        // loopback, so serialising them costs microseconds - and it buys a
        // reply/request pairing that cannot get confused.
        std::lock_guard<std::mutex> lock(_mutex);

        configure();

        if (!_enabled) {
            return false;
        }

        if (!ensure_connected()) {
            return false;
        }

        Json request = Json::object();
        request["type"] = Json("call");
        request["v"] = Json(kProtocolVersion);
        request["seq"] = Json(static_cast<std::int64_t>(++_sequence));
        request["name"] = Json(std::string(name));
        request["args"] = args;
        request["session"] = Json(_session_id);

        std::string response;
        if (!_transport->exchange(request.dump(), response)) {
            log_write(LogLevel::warn, "the backend went away mid-call; using defaults again");
            _transport->close();
            return false;
        }

        Json message;
        if (!parse(response, message)) {
            log_write(LogLevel::warn, "ignoring an unparsable reply from the backend");
            return false;
        }
        const Json* sequence = json_member(message, "seq");
        if (sequence == nullptr || as_int64(*sequence) != static_cast<std::int64_t>(_sequence)) {
            log_write(LogLevel::warn, "ignoring a reply that does not match the request");
            return false;
        }

        ++_call_count;

        // What the backend wants done to the game, if anything, arrives with the reply
        // that says so - and it arrives whether or not the backend had an opinion about
        // the call it came back on. A game is told things while it is asking about
        // something else: a lobby host sits on calls nobody answers, and a payload that
        // rode back on one of those used to be dropped along with the reply.
        std::size_t waiting = 0;
        if (const Json* events = json_member(message, "events");
            events != nullptr && events->is_array()) {
            // The queue's own lock, inside the round trip's - never the other way round,
            // so a game's pump does not wait for a call to come back before it can be
            // told anything. See the note on the two locks in bridge/client.hpp.
            const std::lock_guard<std::mutex> queued(_events_mutex);
            for (const Json& event : *events) {
                _events.push_back(event);
            }
            waiting = _events.size();
        }
        if (waiting != 0u) {
            log_write(LogLevel::debug, "the backend sent " + std::to_string(waiting) +
                                           " payload(s) for the game to be given next");
        }

        const Json* answer = json_member(message, "answer");
        if (answer == nullptr || as_string(*answer) != "handled") {
            ++_unhandled_count;
            return false;
        }

        reply = std::move(message);
        return true;
    } catch (...) {
        // Allocation failure, or anything else: a game must never see an
        // exception thrown across the exported API.
        log_write(LogLevel::error, "the bridge hit an unexpected exception");
        return false;
    }
}

bool Client::take_event(Json& out) noexcept {
    try {
        // The queue's own lock, and it is not held while the caller dispatches: taking
        // one copies it out and returns, so a game that calls back into the bridge from
        // inside a callback cannot deadlock on itself.
        std::lock_guard<std::mutex> lock(_events_mutex);
        if (_events.empty()) {
            return false;
        }
        out = std::move(_events.front());
        _events.pop_front();
        return true;
    } catch (...) {
        return false;
    }
}

Client::Counts Client::counts() const noexcept {
    // Locked, like session_id(): the counts are written inside call()'s critical
    // section, so this is a state the client was actually in rather than one assembled
    // from two reads that straddle a call. A lock that cannot be taken answers zero,
    // for the same reason as everywhere else here - the caller is an exported
    // diagnostic and a game must not see an exception.
    try {
        const std::lock_guard<std::mutex> lock(_mutex);
        return Counts{_call_count.load(), _unhandled_count.load()};
    } catch (...) {
        return Counts{};
    }
}

std::string Client::session_id() const noexcept {
    // The lock can fail where an allocation cannot be allowed to: this returns a value
    // to a game, so a mutex that cannot be taken answers "no session" rather than
    // ending the process this DLL is loaded into - the same decision the allocation
    // in instance() records.
    try {
        std::lock_guard<std::mutex> lock(_mutex);
        return _session_id;
    } catch (...) {
        return std::string();
    }
}

bool Client::backend_connected() noexcept {
    try {
        // Under the lock, like call(): configure() decides whether this client is on at
        // all, and it is not something a second thread may read half-written.
        std::lock_guard<std::mutex> lock(_mutex);

        configure();

        if (!_enabled) {
            return false;
        }
        return ensure_connected();
    } catch (...) {
        return false;
    }
}

bool invoke(std::string_view name, const Json& args, Json& reply) noexcept {
    return Client::instance().call(name, args, reply);
}

}  // namespace steammock
