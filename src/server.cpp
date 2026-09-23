#include "bridge/server.hpp"

#include <atomic>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <string>
#include <utility>

#include "bridge/frame.hpp"
#include "bridge/protocol.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>

namespace steammock {
namespace {

using socket_t = SOCKET;
constexpr socket_t kInvalidSocket = INVALID_SOCKET;

void ensure_winsock_started() noexcept {
    static bool started = false;
    if (!started) {
        WSADATA data{};
        (void)WSAStartup(MAKEWORD(2, 2), &data);
        started = true;
    }
}

socket_t as_socket(std::uintptr_t value) noexcept { return static_cast<socket_t>(value); }

void close_socket(socket_t handle) noexcept {
    if (handle == kInvalidSocket) {
        return;
    }
    (void)closesocket(handle);
}

// Half-closing wakes a thread parked in recv/accept without tearing the handle
// out from under it, which is what makes stop() unblock cleanly.
void shutdown_socket(socket_t handle) noexcept {
    if (handle == kInvalidSocket) {
        return;
    }
    (void)::shutdown(handle, SD_BOTH);
}

bool send_all(socket_t handle, const char* data, std::size_t size) noexcept {
    std::size_t sent = 0;
    while (sent < size) {
        const std::size_t remaining = size - sent;
        const int chunk = static_cast<int>(remaining > 0x7FFFFFFFu ? 0x7FFFFFFFu : remaining);
        const int written = ::send(handle, data + sent, chunk, 0);
        if (written <= 0) {
            return false;
        }
        sent += static_cast<std::size_t>(written);
    }
    return true;
}

bool recv_all(socket_t handle, char* data, std::size_t size) noexcept {
    std::size_t received = 0;
    while (received < size) {
        const std::size_t remaining = size - received;
        const int chunk = static_cast<int>(remaining > 0x7FFFFFFFu ? 0x7FFFFFFFu : remaining);
        const int got = ::recv(handle, data + received, chunk, 0);
        if (got <= 0) {
            return false;
        }
        received += static_cast<std::size_t>(got);
    }
    return true;
}

bool send_frame(socket_t handle, const std::string& payload) noexcept {
    if (payload.empty() || payload.size() > kMaxFrameBytes) {
        return false;
    }
    char header[4] = {};
    write_frame_length(header, static_cast<std::uint32_t>(payload.size()));
    return send_all(handle, header, sizeof(header)) &&
           send_all(handle, payload.data(), payload.size());
}

// Reads exactly one frame. False means the peer went away, or sent something
// that is not a frame - either way this connection is finished.
bool recv_frame(socket_t handle, std::string& payload) noexcept {
    try {
        char header[4] = {};
        if (!recv_all(handle, header, sizeof(header))) {
            return false;
        }
        const std::uint32_t length = read_frame_length(header);
        if (length == 0u || length > kMaxFrameBytes) {
            return false;
        }
        payload.assign(length, '\0');
        return recv_all(handle, payload.data(), length);
    } catch (...) {
        // A frame this process cannot hold is a frame it cannot serve, so the
        // connection ends - which is what a false return already means.
        return false;
    }
}

std::string peer_name(socket_t handle) {
    sockaddr_storage address{};
    socklen_t length = sizeof(address);
    if (::getpeername(handle, reinterpret_cast<sockaddr*>(&address), &length) != 0) {
        return "unknown peer";
    }
    char host[INET6_ADDRSTRLEN] = {};
    unsigned port = 0;
    if (address.ss_family == AF_INET) {
        const auto* v4 = reinterpret_cast<const sockaddr_in*>(&address);
        (void)::inet_ntop(AF_INET, &v4->sin_addr, host, sizeof(host));
        port = ntohs(v4->sin_port);
    } else if (address.ss_family == AF_INET6) {
        const auto* v6 = reinterpret_cast<const sockaddr_in6*>(&address);
        (void)::inet_ntop(AF_INET6, &v6->sin6_addr, host, sizeof(host));
        port = ntohs(v6->sin6_port);
    } else {
        return "unknown peer";
    }
    return std::string(host) + ":" + std::to_string(port);
}

std::int64_t unix_milliseconds_now() noexcept {
    const auto now = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
}

// A session id only has to be unique within one run and readable in a log, so it
// is a random prefix plus a counter rather than a UUID.
std::string make_session_id() {
    static std::atomic<std::uint64_t> counter{0};
    static const std::uint64_t seed =
        static_cast<std::uint64_t>(std::chrono::steady_clock::now().time_since_epoch().count()) ^
        static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(&counter));
    std::uint64_t value =
        (seed ^ (counter.fetch_add(1) + 0x9E3779B97F4A7C15ull)) * 0xBF58476D1CE4E5B9ull;
    value ^= value >> 27;
    char buffer[16] = {};
    std::snprintf(buffer, sizeof(buffer), "%012llx",
                  static_cast<unsigned long long>(value & 0xFFFFFFFFFFFFull));
    return std::string(buffer);
}

const char* level_name(LogLevel level) noexcept {
    switch (level) {
        case LogLevel::error: return "error";
        case LogLevel::warn: return "warn";
        case LogLevel::info: return "info";
        case LogLevel::debug: return "debug";
    }
    return "info";
}

std::string text_member(const Json& value, const char* key) {
    if (const Json* member = value.find(key)) {
        return member->is_string() ? member->as_string() : std::string();
    }
    return std::string();
}

}  // namespace

// The backend's own line format, the one the console used to print.
void stderr_log_sink(LogLevel level, const std::string& message) {
    const std::time_t now = std::time(nullptr);
    std::tm parts{};
    localtime_s(&parts, &now);
    char stamp[16] = {};
    std::snprintf(stamp, sizeof(stamp), "%02d:%02d:%02d", parts.tm_hour, parts.tm_min,
                  parts.tm_sec);
    std::fprintf(stderr, "%s %-7s %s\n", stamp, level_name(level), message.c_str());
    std::fflush(stderr);
}

// ---------------------------------------------------------------------------
//  A record, as the transcript spells it
// ---------------------------------------------------------------------------

Json CallRecord::to_json() const {
    Json json = Json::object();
    json.set("session", Json::string(session));
    json.set("seq", Json::integer(seq));
    json.set("call", Json::string(call));
    json.set("args", args);
    json.set("answered", Json::boolean(answered));
    json.set("via", Json::string(via));
    json.set("ms", Json::real(ms));
    if (answered) {
        json.set("ret", ret);
        if (carries_out(out)) {
            json.set("out", out);
        }
    }
    return json;
}

// ---------------------------------------------------------------------------
//  Server
// ---------------------------------------------------------------------------

Server::Server(Dispatcher dispatcher, ServerOptions options)
    : _dispatcher(std::move(dispatcher)), _options(std::move(options)) {
    if (!_options.log) {
        _options.log = &stderr_log_sink;
    }
}

Server::~Server() { stop(); }

void Server::log(LogLevel level, const std::string& message) const {
    if (!_options.log || level > _options.log_level) {
        return;
    }
    _options.log(level, message);
}

bool Server::start(std::string& error) {
    ensure_winsock_started();

    if (!_options.transcript.empty()) {
        _transcript = std::fopen(_options.transcript.c_str(), "ab");
        if (_transcript == nullptr) {
            error = "cannot open the transcript " + _options.transcript;
            return false;
        }
    }

    char service[8] = {};
    std::snprintf(service, sizeof(service), "%u", static_cast<unsigned>(_options.port));

    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    addrinfo* results = nullptr;
    const char* host = _options.host.empty() ? nullptr : _options.host.c_str();
    if (getaddrinfo(host, service, &hints, &results) != 0 || results == nullptr) {
        error = "cannot resolve " + _options.host;
        return false;
    }

    socket_t listener = kInvalidSocket;
    for (addrinfo* candidate = results; candidate != nullptr; candidate = candidate->ai_next) {
        listener = ::socket(candidate->ai_family, candidate->ai_socktype, candidate->ai_protocol);
        if (listener == kInvalidSocket) {
            continue;
        }
        const int enable = 1;
        (void)setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&enable),
                         sizeof(enable));
        if (::bind(listener, candidate->ai_addr, static_cast<int>(candidate->ai_addrlen)) == 0) {
            break;
        }
        close_socket(listener);
        listener = kInvalidSocket;
    }
    freeaddrinfo(results);

    if (listener == kInvalidSocket) {
        error = "cannot bind " + _options.host + ":" + std::to_string(_options.port);
        return false;
    }
    if (::listen(listener, 16) != 0) {
        close_socket(listener);
        error = "cannot listen on " + _options.host + ":" + std::to_string(_options.port);
        return false;
    }

    // Port 0 means "pick one", so the bound port has to be read back before
    // anyone can be told where to connect.
    sockaddr_in bound{};
    socklen_t bound_length = sizeof(bound);
    _port = _options.port;
    if (::getsockname(listener, reinterpret_cast<sockaddr*>(&bound), &bound_length) == 0) {
        _port = ntohs(bound.sin_port);
    }

    _listener = static_cast<std::uintptr_t>(listener);
    _accept_thread =
        std::thread([this, listener] { accept_loop(static_cast<std::uintptr_t>(listener)); });

    log(LogLevel::info, "listening on " + _options.host + ":" + std::to_string(_port));
    return true;
}

void Server::stop() {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_stopping && _listener == kNoSocket) {
            return;  // already stopped
        }
        _stopping = true;
    }

    if (_listener != kNoSocket) {
        // Shutting down first is what unblocks accept() on both platforms.
        shutdown_socket(as_socket(_listener));
        close_socket(as_socket(_listener));
        _listener = kNoSocket;
    }
    if (_accept_thread.joinable()) {
        _accept_thread.join();
    }

    // No new connection can arrive now, so both lists are stable and the workers
    // are waiting on their own sockets.
    {
        std::lock_guard<std::mutex> lock(_mutex);
        for (const std::uintptr_t client : _clients) {
            shutdown_socket(as_socket(client));
        }
    }
    for (std::thread& worker : _workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    _workers.clear();

    std::lock_guard<std::mutex> lock(_mutex);
    _clients.clear();
    if (_transcript != nullptr) {
        std::fflush(_transcript);
        std::fclose(_transcript);
        _transcript = nullptr;
    }
}

std::uint16_t Server::port() const noexcept { return _port; }

std::string Server::summary() const {
    std::lock_guard<std::mutex> lock(_mutex);
    const std::size_t answered = _total_calls - _unanswered_calls;
    return std::to_string(_sessions.size()) + " game session(s), " + std::to_string(_total_calls) +
           " call(s), " + std::to_string(answered) + " answered, " +
           std::to_string(_unanswered_calls) + " left to the stub's defaults";
}

std::vector<SessionSnapshot> Server::sessions() const {
    std::lock_guard<std::mutex> lock(_mutex);
    std::vector<SessionSnapshot> snapshots;
    snapshots.reserve(_sessions.size());
    for (const std::unique_ptr<Session>& session : _sessions) {
        SessionSnapshot snapshot;
        snapshot.id = session->id();
        snapshot.exe = session->exe();
        snapshot.arch = session->arch();
        snapshot.pid = session->pid();
        snapshot.profile = session->profile().name;
        snapshot.stats = session->profile().stats;
        snapshot.achievements = session->profile().achievements;
        snapshot.connected_at_unix_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                                            session->started().time_since_epoch())
                                            .count();
        snapshot.call_count = session->call_count();
        snapshot.connected = session->connected();
        snapshots.push_back(std::move(snapshot));
    }
    return snapshots;
}

std::vector<CallRecord> Server::records() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _records;
}

std::size_t Server::record_count() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _records.size();
}

std::vector<CallRecord> Server::records_since(std::size_t index) const {
    std::lock_guard<std::mutex> lock(_mutex);
    std::vector<CallRecord> tail;
    if (index >= _records.size()) {
        return tail;
    }
    tail.reserve(_records.size() - index);
    for (std::size_t position = index; position < _records.size(); ++position) {
        tail.push_back(_records[position]);
    }
    return tail;
}

std::size_t Server::call_count() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _total_calls;
}

std::size_t Server::unanswered_count() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _unanswered_calls;
}

void Server::accept_loop(std::uintptr_t listener) {
    const socket_t socket = as_socket(listener);
    for (;;) {
        const socket_t client = ::accept(socket, nullptr, nullptr);
        if (client == kInvalidSocket) {
            return;  // the listener was closed: we are stopping
        }

        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_stopping) {
                close_socket(client);
                return;
            }
            _clients.push_back(static_cast<std::uintptr_t>(client));
            _workers.emplace_back([this, client] {
                const std::string peer = peer_name(client);
                serve(static_cast<std::uintptr_t>(client), peer);
            });
        }
    }
}

void Server::serve(std::uintptr_t client, const std::string& peer) {
    const socket_t socket = as_socket(client);
    const auto connected_at = std::chrono::steady_clock::now();

    Session* session = nullptr;
    std::string session_id;

    std::string payload;
    if (!recv_frame(socket, payload)) {
        log(LogLevel::warn, "a connection from " + peer + " closed before it said hello");
        close_socket(socket);
        return;
    }

    Json hello;
    if (!Json::parse(payload, hello) || !hello.is_object()) {
        log(LogLevel::warn, "the first frame from " + peer + " was not a JSON object");
        close_socket(socket);
        return;
    }
    const std::string kind = text_member(hello, "type");
    if (kind != "hello") {
        log(LogLevel::warn, "first message from " + peer + " was '" + kind + "', not a hello");
        close_socket(socket);
        return;
    }

    {
        std::lock_guard<std::mutex> lock(_mutex);
        session_id = make_session_id();
        auto created = std::make_unique<Session>(session_id, hello, _dispatcher.profile_for(hello));
        session = created.get();
        _sessions.push_back(std::move(created));
    }
    log(LogLevel::info, "game connected: " + session->describe());

    if (send_frame(socket, make_welcome(session_id, session->profile().name).dump())) {
        for (;;) {
            if (!recv_frame(socket, payload)) {
                break;  // the normal way a game leaves
            }
            Json message;
            if (!Json::parse(payload, message) || !message.is_object()) {
                log(LogLevel::warn, "protocol error from " + session_id + ": an unparsable frame");
                break;
            }
            const std::string message_kind = text_member(message, "type");
            if (message_kind == "bye") {
                break;
            }
            if (message_kind != "call") {
                std::string ignored = "ignoring a '";
                ignored += message_kind;
                ignored += "' message from ";
                ignored += session_id;
                log(LogLevel::warn, ignored);
                continue;
            }
            if (!send_frame(socket, handle_call(*session, message))) {
                break;
            }
        }
    }

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                             std::chrono::steady_clock::now() - connected_at)
                             .count();
    const std::string description = session->describe();
    const std::size_t calls = session->call_count();
    {
        // The session stays: the transcript and the summary are about the whole
        // run, so a game that has left is still part of the picture. Only its
        // "live" flag changes.
        std::lock_guard<std::mutex> lock(_mutex);
        session->set_connected(false);
    }
    log(LogLevel::info, "game disconnected: " + description + " (" +
                            std::to_string(static_cast<double>(elapsed) / 1000.0) + "s, " +
                            std::to_string(calls) + " calls)");
    close_socket(socket);
}

std::string Server::handle_call(Session& session, const Json& message) {
    const std::string name = text_member(message, "name");

    Json args = Json::object();
    if (const Json* value = message.find("args"); value != nullptr && value->is_object()) {
        args = *value;
    } else if (value != nullptr && !value->is_null()) {
        log(LogLevel::warn, name + ": arguments were not an object; using none");
    }

    std::int64_t seq = 0;
    if (const Json* value = message.find("seq"); value != nullptr && value->is_number()) {
        seq = value->as_int64();
    }

    const auto started = std::chrono::steady_clock::now();
    Answer answer;
    {
        // Resolving a call reads the scenario and may write the session's stats,
        // so it happens under the lock; the socket work around it does not.
        std::lock_guard<std::mutex> lock(_mutex);
        answer = _dispatcher.answer(session, name, args);
        session.note_call();
    }
    const double elapsed_ms =
        std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - started)
            .count();

    CallRecord record;
    record.session = session.id();
    record.seq = seq;
    record.call = name;
    record.args = args;
    record.answered = answer.answered;
    record.via = answer.via;
    record.ms = elapsed_ms;
    record.ret = answer.ret;
    record.out = answer.out;
    record.at_unix_ms = unix_milliseconds_now();

    {
        std::lock_guard<std::mutex> lock(_mutex);
        ++_total_calls;
        if (!answer.answered) {
            ++_unanswered_calls;
        }
        _records.push_back(record);
        write_transcript(record);
    }

    if (answer.answered) {
        std::string line = "-> " + name + " = " + answer.ret.dump();
        if (carries_out(answer.out)) {
            line += " out=" + answer.out.dump();
        }
        log(LogLevel::debug, line);
    } else {
        log(LogLevel::debug, "-- " + name + ": no opinion, the stub uses its default");
    }

    // What the backend wants done to the game rides with the reply: the stub
    // queues it and hands it over on the game's own next RunCallbacks, which is
    // the only place a callback object may be called from.
    Json reply = make_reply(seq, answer.answered, answer.ret, answer.out);
    if (answer.events.is_array() && !answer.events.items().empty()) {
        reply.set("events", answer.events);
        log(LogLevel::debug, "   .. " + std::to_string(answer.events.items().size()) +
                                 " payload(s) for the game, on its next pump");
    }
    return reply.dump();
}

void Server::write_transcript(const CallRecord& record) {
    if (_transcript == nullptr) {
        return;
    }
    const std::string line = record.to_json().dump() + "\n";
    std::fwrite(line.data(), 1, line.size(), _transcript);
    std::fflush(_transcript);
}

}  // namespace steammock
