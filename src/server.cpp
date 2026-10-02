#include "bridge/server.hpp"

#include <atomic>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <string>
#include <utility>

#include "bridge/frame.hpp"
#include "bridge/protocol.hpp"
#include "socket_io.hpp"

namespace steammock {
namespace {

using socket_io::as_socket;
using socket_io::close_socket;
using socket_io::ensure_winsock_started;
using socket_io::kInvalidSocket;
using socket_io::send_all;
using socket_io::shutdown_socket;
using socket_io::socket_t;

// How long a read waits before looking up again. Long enough that an idle connection is
// not woken for nothing, short enough that stopping the server is over in a moment.
constexpr long kReadSliceMicroseconds = 200 * 1000;

// Reads exactly this many bytes off a socket that may have nothing to say yet.
//
// The wait is a select() slice rather than a plain blocking recv() because a blocked
// recv() cannot be called off on this platform: shutdown() on a connected socket is
// accepted and returns 0, and the thread reading it stays where it is. A stop() that
// then waits to join that thread waits for the game to speak again - which for a game
// sitting on a menu is forever - and a server that will not stop leaves its port bound
// for whoever runs next. A slice that ends with nothing to read is not an idle game
// dropped: it is the moment the reader looks up and asks whether it is still wanted.
//
// One recv() per slice rather than a loop to the end of the frame, because a partial
// frame must not be able to park this thread any longer than a slice either.
bool recv_all(socket_t handle, char* data, std::size_t size,
              const std::atomic<bool>& stopping) noexcept {
    std::size_t received = 0;
    while (received < size) {
        if (stopping.load(std::memory_order_acquire)) {
            return false;
        }
        fd_set readable;
        FD_ZERO(&readable);
        FD_SET(handle, &readable);
        timeval slice{};
        slice.tv_sec = 0;
        slice.tv_usec = kReadSliceMicroseconds;
        const int ready = ::select(0, &readable, nullptr, nullptr, &slice);
        if (ready == SOCKET_ERROR) {
            return false;
        }
        if (ready == 0) {
            continue;
        }
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

// Reads exactly one frame. False means the peer went away, the server is stopping, or
// something arrived that is not a frame - either way this connection is finished.
bool recv_frame(socket_t handle, std::string& payload, const std::atomic<bool>& stopping) noexcept {
    try {
        char header[4] = {};
        if (!recv_all(handle, header, sizeof(header), stopping)) {
            return false;
        }
        const std::uint32_t length = read_frame_length(header);
        if (length == 0u || length > kMaxFrameBytes) {
            return false;
        }
        payload.assign(length, '\0');
        return recv_all(handle, payload.data(), length, stopping);
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

}  // namespace

// The backend's own line format, the one the console used to print.
void stderr_log_sink(LogLevel level, const std::string& message) {
    const std::time_t now = std::time(nullptr);
    std::tm parts{};
    localtime_s(&parts, &now);
    char stamp[16] = {};
    std::snprintf(stamp, sizeof(stamp), "%02d:%02d:%02d", parts.tm_hour, parts.tm_min,
                  parts.tm_sec);
    std::fprintf(stderr, "%s %-7s %s\n", stamp, log_level_name(level), message.c_str());
    std::fflush(stderr);
}

// ---------------------------------------------------------------------------
//  A record, as the transcript spells it
// ---------------------------------------------------------------------------

Json CallRecord::to_json() const {
    Json json = Json::object();
    json["session"] = Json(session);
    json["seq"] = Json(seq);
    json["call"] = Json(call);
    json["args"] = args;
    json["answered"] = Json(answered);
    json["via"] = Json(via);
    json["ms"] = Json(ms);
    // When the call was handled, in unix milliseconds. `ms` is how long it took and
    // `seq` counts within one session, so without this there is no way to order two
    // sessions' calls against each other - and a race between sessions is exactly what
    // a transcript is read for once the two are behaving differently.
    json["at_unix_ms"] = Json(at_unix_ms);
    if (answered) {
        json["ret"] = ret;
        if (carries_out(out)) {
            json["out"] = out;
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
    try {
        _options.log(level, message);
        // NOLINTNEXTLINE(bugprone-empty-catch) - losing the line is the whole of the failure
    } catch (...) {
        // The sink is somebody else's code - a console, a window's log panel - and this
        // is called from the connection threads, where an exception has nothing to
        // return to and ends the process. Losing the line is the whole of what can go
        // wrong here, and it is what the rest of this tree does at every boundary it
        // does not own.
    }
}

bool Server::start(std::string& error) {
    // Before anything is opened, so a process whose socket layer never came up says
    // exactly that - it used to be a log line and a `true`, and the failure arrived
    // later as "cannot bind", which is a different problem with a different cause.
    if (!ensure_winsock_started()) {
        error = "the socket layer could not be started (WSAStartup failed)";
        return false;
    }

    {
        // A server listens once. Starting one that is already listening would leave the
        // first listener and its thread with nothing pointing at them, and starting one
        // that has been stopped cannot work either: `_stopping` is what every reader
        // decides by, and it is never cleared, so the new accept loop would refuse every
        // game that arrived and say nothing about why. Refusing outright is the honest
        // answer to both; restarting a run means a new Server, which is what both front
        // ends do.
        std::scoped_lock once(_stop_mutex);
        if (_stopped || _listener != kNoSocket) {
            error = "the server was already started, and a run is not restartable";
            return false;
        }
    }

    if (!_options.transcript.empty()) {
        _transcript = std::fopen(_options.transcript.c_str(), "ab");
        if (_transcript == nullptr) {
            error = "cannot open the transcript " + _options.transcript;
            return false;
        }
    }

    // Every way out from here that is not a listening server has to leave what it
    // opened behind it, because `stop()` is the only thing that closes these and a
    // failed start never reaches it.
    //
    // The store is not released here, and it is the one thing in this lambda that is
    // not about tidiness: the worlds above point into it, and this path leaves a server
    // that is about to be destroyed - a failed start is not restartable, and both front
    // ends drop it. The unique_ptr closes the file with the object, after them.
    const auto give_up = [this](std::string message) {
        if (_transcript != nullptr) {
            std::fclose(_transcript);
            _transcript = nullptr;
        }
        return message;
    };

    // The state file, when this run was given one. Opened here rather than lazily, so a
    // path that cannot be written is said at startup - the same as a transcript that
    // cannot be written - and not discovered a run later as state that quietly was not
    // being kept.
    if (!_options.state.empty()) {
        std::string why;
        _store = Store::open(_options.state, why);
        if (_store == nullptr) {
            error = give_up(std::move(why));
            return false;
        }
        // What the worlds answer from, before a single call can arrive. Both are told
        // where the state is and read everything the store has; from here on they write
        // through it as they go, so there is no moment at the end of a run when what was
        // kept and what was answered could disagree.
        _leaderboards.attach(_store.get());
        _inventory.attach(_store.get());
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
        error = give_up("cannot resolve " + _options.host);
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
        error = give_up("cannot bind " + _options.host + ":" + std::to_string(_options.port));
        return false;
    }
    if (::listen(listener, 16) != 0) {
        close_socket(listener);
        error = give_up("cannot listen on " + _options.host + ":" + std::to_string(_options.port));
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
    try {
        _accept_thread =
            std::thread([this, listener] { accept_loop(static_cast<std::uintptr_t>(listener)); });
    } catch (...) {
        // The handle is published a line above and the thread that owns it is not, which
        // is the one state stop() cannot clean up: it closes `_listener`, but `_listener`
        // is also what refuses a second start(), so the run would be unstartable as well
        // as unstopped. Both are undone, and the transcript with them - `give_up` is what
        // every other way out of this function uses.
        close_socket(listener);
        _listener = kNoSocket;
        error = give_up("cannot start the thread that accepts connections");
        return false;
    }

    log(LogLevel::info, "listening on " + _options.host + ":" + std::to_string(_port));
    return true;
}

void Server::stop() {
    // Held for the whole of it. `_stopping` is what tells the accept loop to finish,
    // and it cannot also be the "already stopped" test: it is set before any of this
    // work is done, so a second caller arriving mid-stop would find it set, skip the
    // early return and close the same sockets and join the same threads again. This
    // lock is nobody else's, so waiting here cannot be a wait for a worker that is
    // waiting on `_mutex`.
    std::scoped_lock once(_stop_mutex);
    if (_stopped) {
        return;  // already stopped
    }
    _stopped = true;

    // Published before anything is closed, because the accept loop and every
    // connection's reader decide what to do next by looking at it.
    _stopping.store(true, std::memory_order_release);

    // Only this thread can be here, and start() wrote the listener before any of this
    // existed, so the handle needs no lock of its own.
    if (_listener != kNoSocket) {
        // The accept() parked on this handle is woken by the close, not by the shutdown:
        // a listening socket has nothing for shutdown() to shut down, whatever it
        // returns. Both are here because shutting down and then closing is the sequence
        // that works on either platform.
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
        std::scoped_lock lock(_mutex);
        for (const std::uintptr_t client : _clients) {
            // The reader in that connection's thread is in a read slice and will look
            // at `_stopping` on its own; this tells the game's end the connection is
            // over, which is what makes it stop waiting for an answer.
            shutdown_socket(as_socket(client));
        }
    }
    for (std::thread& worker : _workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    _workers.clear();

    std::scoped_lock lock(_mutex);
    _clients.clear();
    // The workers were joined above, so nothing can still be writing to this.
    if (_transcript != nullptr) {
        std::fflush(_transcript);
        std::fclose(_transcript);
        _transcript = nullptr;
    }
}

std::uint16_t Server::port() const noexcept { return _port; }

std::string Server::summary() const {
    std::scoped_lock lock(_mutex);
    const std::size_t answered = _total_calls - _unanswered_calls;
    return std::to_string(_sessions.size()) + " game session(s), " + std::to_string(_total_calls) +
           " call(s), " + std::to_string(answered) + " answered, " +
           std::to_string(_unanswered_calls) + " left to the stub's defaults";
}

std::vector<SessionSnapshot> Server::sessions() const {
    std::scoped_lock lock(_mutex);
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
    std::scoped_lock lock(_mutex);
    return std::vector<CallRecord>(_records.begin(), _records.end());
}

std::size_t Server::record_count() const {
    std::scoped_lock lock(_mutex);
    // What the run made, not what is still held: the count is a position in the
    // history, and the history is allowed to forget its beginning.
    return _records_dropped + _records.size();
}

std::size_t Server::records_begin() const {
    std::scoped_lock lock(_mutex);
    return _records_dropped;
}

std::vector<CallRecord> Server::records_since(std::size_t index) const {
    std::scoped_lock lock(_mutex);
    std::vector<CallRecord> tail;
    // `index` is an absolute position in the run's history, the same one record_count()
    // and this cursor are in - so an index that has fallen off the front of the window
    // reads as "from the beginning of what is here", not as nothing at all. A reader that
    // wants to know it has fallen behind asks records_begin() and moves its cursor up.
    const std::size_t first = index > _records_dropped ? index - _records_dropped : 0u;
    if (first >= _records.size()) {
        return tail;
    }
    tail.reserve(_records.size() - first);
    for (std::size_t position = first; position < _records.size(); ++position) {
        tail.push_back(_records[position]);
    }
    return tail;
}

std::size_t Server::call_count() const {
    std::scoped_lock lock(_mutex);
    return _total_calls;
}

std::size_t Server::unanswered_count() const {
    std::scoped_lock lock(_mutex);
    return _unanswered_calls;
}

void Server::accept_loop(std::uintptr_t listener) {
    const socket_t socket = as_socket(listener);
    int reported_error = 0;  // the last accept failure this loop said out loud
    for (;;) {
        // One connection's worth of work is not worth the process, and this runs on a
        // thread: anything that escapes here would end the run for every game attached.
        // An accept that failed says why and carries on, which is what a listener that
        // is still open deserves.
        try {
            const socket_t client = ::accept(socket, nullptr, nullptr);
            if (client == kInvalidSocket) {
                // Read before anything else can touch it: this code is the only thing that
                // says why the accept failed, and taking the lock can set the thread's own
                // last error on the way in.
                const int error = WSAGetLastError();
                if (_stopping.load(std::memory_order_acquire)) {
                    return;  // the listener was closed: we are stopping
                }
                // An accept that failed is not a listener that is finished. Returning here
                // used to end this loop for the rest of the run on a single aborted
                // connection, and with it every game that had not connected yet: the
                // listener is still open, so the next accept is the same call made again.
                if (error != reported_error) {
                    // Once per kind of failure rather than twenty times a second: a
                    // condition that persists is one line, not a log nobody can read.
                    reported_error = error;
                    log(LogLevel::warn,
                        "accept failed (" + std::to_string(error) + ") - still listening");
                }
                Sleep(50);
                continue;
            }
            reported_error = 0;

            std::scoped_lock lock(_mutex);
            if (_stopping.load(std::memory_order_acquire)) {
                close_socket(client);
                return;
            }
            _clients.push_back(static_cast<std::uintptr_t>(client));
            // The worker closes its own socket and forgets its own entry, so what is left
            // here is the set of connections that are still attached - which is what stop()
            // shuts down and what a snapshot is asked about.
            _workers.emplace_back([this, client] { serve(static_cast<std::uintptr_t>(client)); });
        } catch (const std::exception& error) {
            log(LogLevel::error, "the accept loop threw: " + std::string(error.what()));
            Sleep(50);
        } catch (...) {
            log(LogLevel::error, "the accept loop threw");
            Sleep(50);
        }
    }
}

void Server::serve(std::uintptr_t client) {
    const socket_t socket = as_socket(client);
    try {
        serve_connection(client);
    } catch (const std::exception& error) {
        log(LogLevel::error, "a connection threw: " + std::string(error.what()));
    } catch (...) {
        log(LogLevel::error, "a connection threw");
    }
    {
        // Forgotten *before* the handle is closed: the number a closed handle had is one
        // the next socket this process opens can be given, and an entry erased after that
        // would be the new connection's - which is how a live game goes missing from the
        // set stop() shuts down.
        std::scoped_lock lock(_mutex);
        for (auto entry = _clients.begin(); entry != _clients.end(); ++entry) {
            if (*entry == client) {
                _clients.erase(entry);
                break;
            }
        }
    }
    // Closed exactly once per connection, on every way out including the exceptional
    // one, and only after nothing else can still be using it.
    close_socket(socket);
}

void Server::serve_connection(std::uintptr_t client) {
    const socket_t socket = as_socket(client);
    const std::string peer = peer_name(socket);
    const auto connected_at = std::chrono::steady_clock::now();

    Session* session = nullptr;
    std::string session_id;

    std::string payload;
    if (!recv_frame(socket, payload, _stopping)) {
        log(LogLevel::warn, "a connection from " + peer + " closed before it said hello");
        return;
    }

    Json hello;
    if (!parse(payload, hello) || !hello.is_object()) {
        log(LogLevel::warn, "the first frame from " + peer + " was not a JSON object");
        return;
    }
    const std::string kind = as_string_member(hello, "type");
    if (kind != "hello") {
        log(LogLevel::warn, "first message from " + peer + " was '" + kind + "', not a hello");
        return;
    }

    {
        std::scoped_lock lock(_mutex);
        session_id = make_session_id();
        // The profile is resolved before the session exists, because a name the scenario
        // does not have is refused rather than answered with the default one - whether the
        // game asked for it by name or a match rule named it. Silently serving the default
        // meant two clients could run as the same player, and from outside that looks like
        // a lobby with a member missing.
        std::string refused;
        const std::optional<Profile> profile = _dispatcher.profile_for(hello, &refused);
        if (!profile) {
            log(LogLevel::error, "refusing a handshake: the scenario has no profile '" +
                                     (refused.empty() ? std::string("?") : refused) + "'");
            return;
        }
        // The scenario is what a profile starts as and the store is what it has become.
        // Seeding first is what makes the authored values the floor rather than the last
        // word: a key no game has ever written is refreshed from the scenario, and a key
        // one has is left as the game left it. Then the merge hands the session what the
        // store knows, which is also how two games matched to one profile see each
        // other's writes - see include/bridge/store.hpp for why that is deliberate.
        Profile resolved = *profile;
        if (_store != nullptr) {
            _store->seed(resolved);
            _store->merge_into(resolved);
        }
        auto created =
            std::make_unique<Session>(session_id, hello, std::move(resolved), _store.get());
        session = created.get();
        _sessions.push_back(std::move(created));
    }
    log(LogLevel::info, "game connected: " + session->describe());

    if (send_frame(socket, make_welcome(session_id, session->profile().name).dump())) {
        for (;;) {
            if (!recv_frame(socket, payload, _stopping)) {
                break;  // the normal way a game leaves, or the server is stopping
            }
            // One frame is not worth the process. This runs on a thread of its own,
            // so anything that escapes here - a parse that throws, an answer whose
            // own bookkeeping does - ends the whole harness and every other game
            // with it. The frame is dropped and the connection kept: the frame was
            // read whole, so the stream is still where it should be, and a
            // connection that really is wedged fails on the next read anyway.
            try {
                Json message;
                if (!parse(payload, message) || !message.is_object()) {
                    log(LogLevel::warn,
                        "protocol error from " + session_id + ": an unparsable frame");
                    break;
                }
                const std::string message_kind = as_string_member(message, "type");
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
            } catch (const std::exception& error) {
                log(LogLevel::error, "a frame from " + session_id + " threw: " + error.what());
                continue;
            } catch (...) {
                log(LogLevel::error, "a frame from " + session_id + " threw");
                continue;
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
        // "live" flag changes. What does not stay is what was queued for it: a game
        // that has gone can never make the call that would have carried those
        // payloads back, so leaving them here would be a leak that ends only when the
        // run does.
        std::scoped_lock lock(_mutex);
        session->set_connected(false);
        _inbox.erase(session_id);
    }
    log(LogLevel::info, "game disconnected: " + description + " (" +
                            std::to_string(static_cast<double>(elapsed) / 1000.0) + "s, " +
                            std::to_string(calls) + " calls)");
}

std::string Server::handle_call(Session& session, const Json& message) {
    const std::string name = as_string_member(message, "name");

    Json args = Json::object();
    if (const Json* value = json_member(message, "args"); value != nullptr && value->is_object()) {
        args = *value;
    } else if (value != nullptr && !value->is_null()) {
        log(LogLevel::warn, name + ": arguments were not an object; using none");
    }

    std::int64_t seq = 0;
    if (const Json* value = json_member(message, "seq"); value != nullptr && value->is_number()) {
        seq = as_int64(*value);
    }

    const auto started = std::chrono::steady_clock::now();

    // A scenario can say that answering this call takes a while, and this is where that is
    // waited for: before the state lock, because what the knob is for is a *round trip* that
    // takes longer - the thing a game's own calls queue behind (docs/architecture.md, finding
    // 16 of the review this tree went through) - and a wait taken while holding `_mutex` would
    // be a delay for every other game attached to the run instead. `started` is above it, so
    // the transcript's `ms` counts the wait, which is what a reader comparing two calls is
    // looking at.
    if (const std::int64_t delay_ms = _dispatcher.delay_for(session, name); delay_ms > 0) {
        log(LogLevel::debug, name + ": waiting " + std::to_string(delay_ms) +
                                 " ms before answering, as the scenario says");
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }

    Answer answer;
    CallRecord record;
    // Empty unless this is the call that finds the store has broken. Filled under the
    // lock and said after it, because the log sink is somebody else's code and the state
    // lock is not held across one.
    std::string store_failure;
    {
        // Resolving a call reads the scenario, may write this session's stats, and
        // may now touch the lobbies other games are in - so it happens under the
        // lock, and so does telling the games that were not asking. The socket work
        // around it does not.
        //
        // The call is *published* here too - the session's own count, the run's totals
        // and the history entry - rather than in a second critical section: a reader
        // that took a snapshot between the two used to see a call that one part of the
        // server knew about and the other did not.
        std::scoped_lock lock(_mutex);

        std::vector<std::pair<std::uint64_t, Json>> notifications;
        // The worlds answer in turn, each speaking only about what it has grounds for: the
        // rooms and the packets games made, the boards they posted scores to, and the items
        // they hold. A call none of them claims is the scenario's to answer, and then this
        // session's own state.
        if (!_world.answer(session, name, args, answer, notifications) &&
            !_leaderboards.answer(session, name, args, answer) &&
            !_inventory.answer(session, name, args, answer)) {
            answer = _dispatcher.answer(session, name, args);
        }

        // What the other members have to be told: theirs is not the call that did
        // this, so it waits for them and rides back on whatever reply they make
        // next - which is the only way a game ever learns anything.
        for (const auto& notification : notifications) {
            for (const auto& other : _sessions) {
                if (other->connected() && other->profile().steam_id == notification.first) {
                    _inbox[other->id()].push_back(notification.second);
                }
            }
        }

        // ...and whatever this game was told while it was busy, on this reply.
        if (const auto queued = _inbox.find(session.id()); queued != _inbox.end()) {
            if (!answer.events.is_array()) {
                answer.events = Json::array();
            }
            for (const Json& payload : queued->second) {
                answer.events.push_back(payload);
            }
            _inbox.erase(queued);
        }

        session.note_call();

        record.session = session.id();
        record.seq = seq;
        record.call = name;
        record.args = args;
        record.answered = answer.answered;
        record.via = answer.via;
        record.ms =
            std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - started)
                .count();
        record.ret = answer.ret;
        record.out = answer.out;
        record.at_unix_ms = unix_milliseconds_now();

        ++_total_calls;
        if (!answer.answered) {
            ++_unanswered_calls;
        }
        _records.push_back(record);
        // The oldest fall off the front once the window is full. `_records` used to grow
        // for the whole run - a game that polls an interface made 26,228 calls in
        // fourteen seconds in one recording here, so a long run was hundreds of megabytes
        // by the end - and every `records()` call copied all of it while holding the
        // state lock. The transcript is the record that keeps everything; this is a
        // window for a live view and a test, and `_records_dropped` is what keeps the
        // positions in it absolute, so a reader's cursor still means something.
        if (_records.size() > kMaxRecords) {
            _records.pop_front();
            ++_records_dropped;
        }

        // A store that has stopped accepting writes is the one failure about a state file
        // nobody could notice on their own: every call still answers, every transcript line
        // still lands, and only the keeping of state has quietly stopped happening. Said
        // once - on the first call to find it - and not on every call after that.
        if (_store != nullptr && !_store_failure_reported && !_store->error().empty()) {
            _store_failure_reported = true;
            store_failure = _store->error();
        }
    }
    // Outside the lock, because this is a blocking write to disk and the state it
    // was taken from is already recorded. A record carries its own sequence number,
    // which is what a reader orders by; the file's own order was never the promise.
    write_transcript(record);

    if (answer.answered) {
        std::string line = "-> " + name + " = " + answer.ret.dump();
        if (carries_out(answer.out)) {
            line += " out=" + answer.out.dump();
        }
        log(LogLevel::debug, line);
    } else {
        log(LogLevel::debug, "-- " + name + ": no opinion, the stub uses its default");
    }

    if (!store_failure.empty()) {
        log(LogLevel::error, "the state file has stopped accepting writes, so this run is no "
                             "longer keeping anything: " +
                                 store_failure);
    }

    // What the backend wants done to the game rides with the reply: the stub
    // queues it and hands it over on the game's own next RunCallbacks, which is
    // the only place a callback object may be called from.
    Json reply = make_reply(seq, answer.answered, answer.ret, answer.out);
    if (answer.events.is_array() && !answer.events.empty()) {
        reply["events"] = answer.events;
        log(LogLevel::debug, "   .. " + std::to_string(answer.events.size()) +
                                 " payload(s) for the game, on its next pump");
    }
    return reply.dump();
}

void Server::write_transcript(const CallRecord& record) {
    // Serialised on its own lock rather than on `_mutex`: two games' threads write this
    // file, and a line half of one call and half of another is not a transcript. The
    // lock is not `_mutex`, because the write is to disk and a game's next call should
    // not wait behind it. It also means the file's own order is the order the records
    // were finished in, which is not the order `_records` holds them in - a record
    // carries its sequence number and its arrival time, and that is what a reader
    // orders by.
    std::scoped_lock lock(_transcript_mutex);
    if (_transcript == nullptr) {
        return;  // the run has ended, or no file was ever asked for
    }
    if (_transcript_failed) {
        // A line was left unfinished below. Appending more would put the next record
        // straight after the part of this one that did make it, which is a truncated
        // line merged into the following one - the file would stop being JSON lines at
        // all, and every record after it unreadable.
        return;
    }
    const std::string line = record.to_json().dump() + "\n";
    // In a loop, because `fwrite` is allowed to write less than it was given, and the
    // count used to be compared and then dropped - a short write left the rest of the
    // line in the buffer and the next record was appended after the partial bytes, so
    // one full disk corrupted the framing of everything that followed it.
    const char* cursor = line.data();
    std::size_t remaining = line.size();
    while (remaining > 0u) {
        const std::size_t written = std::fwrite(cursor, 1, remaining, _transcript);
        if (written == 0u) {
            // Said once: a full disk is not twenty thousand lines' worth of news. The
            // file is left as it is, and no further record is appended to it.
            _transcript_failed = true;
            log(LogLevel::error,
                "the transcript is not being written in full: " + _options.transcript);
            return;
        }
        cursor += written;
        remaining -= written;
    }
    std::fflush(_transcript);
}

}  // namespace steammock
