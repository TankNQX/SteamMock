#include "bridge/client.hpp"

#include <cstdlib>
#include <string>

#include "bridge/log.hpp"
#include "bridge/protocol.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace steammock {
namespace {

constexpr unsigned kDefaultPort = 50990u;
constexpr unsigned kDefaultTimeoutMs = 2000u;

std::string environment(const char* name) {
    const char* value = std::getenv(name);
    return value != nullptr ? std::string(value) : std::string();
}

std::string executable_path() {
    char buffer[MAX_PATH] = {};
    if (GetModuleFileNameA(nullptr, buffer, sizeof(buffer)) == 0) {
        return std::string();
    }
    return std::string(buffer);
}

std::string file_name_of(const std::string& path) {
    const std::size_t slash = path.find_last_of("\\/");
    return slash == std::string::npos ? path : path.substr(slash + 1u);
}

unsigned parse_unsigned(const std::string& text, unsigned fallback) noexcept {
    if (text.empty()) {
        return fallback;
    }
    unsigned value = 0;
    for (const char ch : text) {
        if (ch < '0' || ch > '9') {
            return fallback;
        }
        value = value * 10u + static_cast<unsigned>(ch - '0');
        if (value > 65535u) {
            return fallback;
        }
    }
    return value;
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
Client::Client() noexcept : _transport(new TcpTransport()), _mutex(new std::mutex()) {}

Client::~Client() {
    _transport->close();
    delete _mutex;
}

// Deliberately not noexcept: this allocates, and a failure here is worth
// catching in call(), which answers the game with a default. A noexcept here
// would turn that same failure into std::terminate - the opposite of what this
// harness promises a game.
void Client::configure() {
    if (_configured) {
        return;
    }
    _configured = true;

    const std::string path = executable_path();
    _exe_name = file_name_of(path);
    log_configure(path.c_str());

    if (environment("STEAMMOCK_OFF") == "1") {
        _enabled = false;
        log_write(LogLevel::info, "STEAMMOCK_OFF=1 - every call answers with its default");
        return;
    }

    _host = environment("STEAMMOCK_HOST");
    if (_host.empty()) {
        _host = "127.0.0.1";
    }
    _port = static_cast<std::uint16_t>(parse_unsigned(environment("STEAMMOCK_PORT"), kDefaultPort));
    _timeout_ms = parse_unsigned(environment("STEAMMOCK_TIMEOUT_MS"), kDefaultTimeoutMs);
    _transport->set_timeout_ms(_timeout_ms);

    log_write(LogLevel::debug, "backend target " + _host + ":" + std::to_string(_port));
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
    hello.set("type", Json::string("hello"));
    hello.set("v", Json::integer(kProtocolVersion));
    hello.set("exe", Json::string(_exe_name));
    hello.set("arch", Json::string(sizeof(void*) == 8u ? "x64" : "x86"));
    hello.set("module", Json::string("steam_api stub"));
    hello.set("pid", Json::integer(static_cast<std::int64_t>(GetCurrentProcessId())));

    std::string response;
    if (!_transport->exchange(hello.dump(), response)) {
        _transport->close();
        return false;
    }
    Json welcome;
    const bool parsed = Json::parse(response, welcome);
    const Json* session = parsed ? welcome.find("session") : nullptr;
    if (session == nullptr) {
        log_write(LogLevel::warn, "the backend did not answer the handshake - ignoring it");
        _transport->close();
        return false;
    }
    _session_id = session->as_string();
    _logged_offline = false;
    log_write(LogLevel::info, "connected to the backend, session " + _session_id);
    return true;
}

bool Client::call(std::string_view name, const Json& args, Json& reply) noexcept {
    try {
        configure();
        if (!_enabled) {
            return false;
        }

        // Game calls arrive on whatever thread the game uses, so one round trip
        // is held under the lock. Steam callbacks are cheap and the transport is
        // loopback, so serialising them costs microseconds - and it buys a
        // reply/request pairing that cannot get confused.
        std::lock_guard<std::mutex> lock(*_mutex);

        if (!ensure_connected()) {
            return false;
        }

        Json request = Json::object();
        request.set("type", Json::string("call"));
        request.set("v", Json::integer(kProtocolVersion));
        request.set("seq", Json::integer(static_cast<std::int64_t>(++_sequence)));
        request.set("name", Json::string(std::string(name)));
        request.set("args", args);
        request.set("session", Json::string(_session_id));

        std::string response;
        if (!_transport->exchange(request.dump(), response)) {
            log_write(LogLevel::warn, "the backend went away mid-call; using defaults again");
            _transport->close();
            return false;
        }

        Json message;
        if (!Json::parse(response, message)) {
            log_write(LogLevel::warn, "ignoring an unparsable reply from the backend");
            return false;
        }
        const Json* sequence = message.find("seq");
        if (sequence == nullptr || sequence->as_int64() != static_cast<std::int64_t>(_sequence)) {
            log_write(LogLevel::warn, "ignoring a reply that does not match the request");
            return false;
        }

        ++_call_count;
        const Json* answer = message.find("answer");
        if (answer == nullptr || answer->as_string() != "handled") {
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

bool Client::backend_connected() noexcept {
    try {
        configure();
        if (!_enabled) {
            return false;
        }
        std::lock_guard<std::mutex> lock(*_mutex);
        return ensure_connected();
    } catch (...) {
        return false;
    }
}

bool invoke(std::string_view name, const Json& args, Json& reply) noexcept {
    return Client::instance().call(name, args, reply);
}

}  // namespace steammock
