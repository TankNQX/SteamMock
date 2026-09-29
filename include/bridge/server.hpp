#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "bridge/defaults.hpp"
#include "bridge/json_read.hpp"
#include "bridge/lobby.hpp"
#include "bridge/log.hpp"
#include "bridge/scenario.hpp"
#include "bridge/session.hpp"

namespace steammock {

// ---------------------------------------------------------------------------
//  The loopback server a game's stub talks to.
// ---------------------------------------------------------------------------
//  One connection is one game process. The server answers calls, writes a
//  line-delimited transcript of everything it saw, and keeps a Session per game
//  so several games can be debugged side by side - each with its own profile,
//  its own stats and its own achievements.
//
//  Everything mutable lives behind one mutex, which is recursive: a call is
//  resolved while it is held, and resolving one runs the world's own answers -
//  which may ask the server about itself through `summary()` or `sessions()`
//  rather than reason about what they half-changed. The same thread asking again
//  is a reader, and what it sees is the state as that moment has it; a thread
//  that is not the one holding it still waits, as it should. The calls themselves
//  run on a thread per connection (a game per connection, and only a few of
//  them), while those two hand back copies for anyone watching - a console, a
//  test, or the live view a GUI draws. Taking a snapshot never blocks on a
//  socket, and no lock is held while a frame is read or written.

// One call, as the transcript records it and as a live view shows it.
struct CallRecord {
    std::string session;
    std::int64_t seq = 0;
    std::string call;
    Json args;
    bool answered = false;
    std::string via = "none";
    double ms = 0.0;
    Json ret;
    Json out;

    // Wall-clock arrival time, for a view that wants to say "just now". Not part
    // of the transcript: a transcript is a log, not a timeline.
    std::int64_t at_unix_ms = 0;

    Json to_json() const;
};

// What one connected game looks like from outside, without handing out a pointer
// into the server's own state.
struct SessionSnapshot {
    std::string id;
    std::string exe;
    std::string arch;
    std::int64_t pid = 0;
    std::string profile;
    std::vector<std::pair<std::string, std::int64_t>> stats;
    std::vector<Achievement> achievements;
    std::size_t call_count = 0;
    bool connected = false;
    std::int64_t connected_at_unix_ms = 0;
};

// Where the server says what it is doing. The console points this at stderr; a
// GUI can point it at a log panel instead.
using LogFn = std::function<void(LogLevel, const std::string&)>;

// The default sink: "HH:MM:SS level message" on stderr. Exposed so a front end
// can put its own lines in the same shape as the server's.
void stderr_log_sink(LogLevel level, const std::string& message);

struct ServerOptions {
    std::string host = kDefaultHost;
    std::uint16_t port = kDefaultPort;  // 0 lets the operating system pick one
    std::string transcript;             // empty keeps no transcript
    LogLevel log_level = LogLevel::info;
    LogFn log;  // empty uses timestamped lines on stderr
};

class Server {
public:
    explicit Server(Dispatcher dispatcher, ServerOptions options = ServerOptions{});
    ~Server();

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    // Binds and starts accepting. False means the socket could not be bound and
    // `error` says why; nothing is left half-started.
    bool start(std::string& error);

    // Stops accepting, drops every connection and closes the transcript. Safe to
    // call more than once, and from the destructor.
    void stop();

    std::uint16_t port() const noexcept;
    std::string summary() const;

    // Snapshots, for a console, a test or a live view.
    std::vector<SessionSnapshot> sessions() const;
    std::vector<CallRecord> records() const;

    // ...and the same history in pieces. A view that redraws has to ask sixty
    // times a second, and copying every record each time would get slower with
    // every call a game makes, so it keeps a cursor and takes only what is new.
    //
    // `index` is an absolute position in the run's history, not an offset into what is
    // still held: the history keeps a window (kMaxRecords) and forgets its beginning, so
    // a cursor that has fallen off the front reads from the start of the window, and
    // records_begin() is what tells a reader it has fallen behind.
    std::size_t record_count() const;
    std::size_t records_begin() const;
    std::vector<CallRecord> records_since(std::size_t index) const;

    // How much call history stays in memory. A game can make a few thousand calls a
    // second - 26,228 in fourteen seconds in one recording here - so this is a window
    // rather than a log: the transcript is the record that keeps everything. Public
    // because it is part of what the history means to a reader, and because a test that
    // pins the window has to know where its edge is.
    static constexpr std::size_t kMaxRecords = 20000;

    std::size_t call_count() const;
    std::size_t unanswered_count() const;

private:
    void log(LogLevel level, const std::string& message) const;
    void accept_loop(std::uintptr_t listener);
    // One connection, with everything it can throw caught and its socket closed exactly
    // once: a worker thread has nothing to return an exception to, and a handle closed
    // twice is a handle whose next owner loses.
    void serve(std::uintptr_t client);
    void serve_connection(std::uintptr_t client);

    // Answers one call and records it. Returns the frame to send back, or an
    // empty string when the reply could not be built.
    std::string handle_call(Session& session, const Json& message);

    void write_transcript(const CallRecord& record);

    Dispatcher _dispatcher;
    ServerOptions _options;

    // The rooms games made. The only state here that is not per session, because it
    // is what makes two instances agree about the same lobby instead of each being
    // handed its own convenient story.
    LobbyWorld _world;

    // What a game has been told but has not heard yet, by session. A game learns
    // things only when it asks, so anything said to it while it was busy waits here
    // for its next call and travels back on that reply.
    std::map<std::string, std::vector<Json>> _inbox;

    // Sockets are held as integers so this header stays free of winsock
    // includes; kNoSocket marks "not listening".
    static constexpr std::uintptr_t kNoSocket = static_cast<std::uintptr_t>(~0ull);

    std::uintptr_t _listener = kNoSocket;
    std::uint16_t _port = 0;
    std::thread _accept_thread;

    // Recursive: a call resolves under it, and resolving one runs the world's own
    // answers, which may ask this server about itself (see the header's comment).
    mutable std::recursive_mutex _mutex;
    // stop() is the one entry point a second thread may arrive at while the first is
    // still inside it - the workers it joins need `_mutex`, so the "already stopped"
    // test cannot live there. This is what makes it happen once.
    std::mutex _stop_mutex;
    bool _stopped = false;
    std::vector<std::thread> _workers;
    std::vector<std::uintptr_t> _clients;
    std::vector<std::unique_ptr<Session>> _sessions;

    // The call history, as a window rather than a log - kMaxRecords above is its size and
    // its meaning. A deque, because the oldest go one at a time as new ones arrive and
    // erasing the front of a vector would shift every remaining record on every call made
    // past the edge. `_records_dropped` is what keeps a reader's cursor an absolute
    // position in the run rather than in the window.
    std::deque<CallRecord> _records;
    std::size_t _records_dropped = 0;

    std::size_t _total_calls = 0;
    std::size_t _unanswered_calls = 0;
    // What everyone else looks at to find out the run is ending: the accept loop before
    // it accepts again, and a connection's own reader each time a read slice ends. Atomic
    // rather than merely guarded, because those readers have to see it without waiting
    // for a lock the thread that is stopping them may be holding.
    std::atomic<bool> _stopping{false};

    std::FILE* _transcript = nullptr;
    // The transcript has a lock of its own: it is written from every connection's
    // thread, after the state lock is released, so a game's next call does not wait on
    // another game's write to disk. `stop()` closes the file after joining the workers,
    // which is what makes that safe without holding this one.
    std::mutex _transcript_mutex;
    bool _transcript_failed = false;
};

}  // namespace steammock
