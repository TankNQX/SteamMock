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
#include "bridge/inventory.hpp"
#include "bridge/json_read.hpp"
#include "bridge/leaderboard.hpp"
#include "bridge/lobby.hpp"
#include "bridge/log.hpp"
#include "bridge/scenario.hpp"
#include "bridge/session.hpp"
#include "bridge/store.hpp"

namespace steammock
{

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
struct CallRecord
{
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
struct SessionSnapshot
{
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

struct ServerOptions
{
    std::string host = kDefaultHost;
    std::uint16_t port = kDefaultPort; // 0 lets the operating system pick one
    std::string transcript;            // empty keeps no transcript
    // Where the state a game writes is kept between runs. Empty - which is what a run
    // with no `--state` passes, and the default - keeps nothing, and is the harness
    // exactly as it was before there was a store.
    std::string state;
    LogLevel log_level = LogLevel::info;
    LogFn log; // empty uses timestamped lines on stderr
};

class Server
{
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

    // The live view's overrides: the answer a call gets from now on, whatever the
    // scenario, the worlds and this session's own state would have said. This is the
    // resolution order's first rung (see bridge/scenario.hpp), which a scenario can also
    // reach from below with its own `overrides` block - the two differ in who wrote them,
    // and the live one wins, so an override that works at the keyboard can be written down
    // into the file afterwards.
    //
    // An override is also how a failure path is reached: a call that has to fail to be
    // tested does not need a scenario written for it, and does not need the process
    // restarted either.
    //
    // `entry` is written in a `scripted` entry's own words, because it is answered by the
    // same code: `ret`, `out`, and `then` mean here what they mean in a file. `delay_ms`
    // is the one word it does not honour - a call's delay is known before it is resolved,
    // and reading it from an override would put a second lock on the path every call in
    // the run takes.
    //
    // An override applies to every game in the run, since the call is what is being
    // tested rather than the game; and it is what the transcript reports as `via: live`,
    // so a reader can always tell an override from the scenario.
    // An override answers for every game in the run when `profile` is empty, which is the call
    // being tested rather than the game; naming a profile is how one game alone is made to fail,
    // which a run of several clients otherwise cannot be told apart by. A game's own entry beats
    // the run-wide one. Either way it is what the transcript reports as `via: live`, so a reader
    // can always tell an override from the scenario.
    void set_override(const std::string& profile, const std::string& call, Json entry);
    void clear_override(const std::string& profile, const std::string& call);
    // The same two without a scope: the entry that answers every game in the run, which is what
    // an override was before one could name a game.
    void set_override(const std::string& call, Json entry);
    void clear_override(const std::string& call);
    // Every override in force, as (profile, call, entry): what a window draws, and what a person
    // copies into a scenario's `overrides` block when an override turns out to be worth keeping.
    std::vector<std::tuple<std::string, std::string, Json>> overrides() const;
    std::size_t override_count() const;
    // The identities the scenario declares, which is what the live view's scope chooser offers
    // beside the games currently connected: an override for a game that has not joined yet is
    // worth being able to set while the run is going.
    std::vector<std::string> profile_names() const;

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

    // The state this run keeps between runs, or null when it keeps none. Declared before
    // everything that points at it - the worlds and the sessions - because members are
    // destroyed in reverse, and a world reaching a store that has already closed would be
    // reading a file handle SQLite has finished with.
    std::unique_ptr<Store> _store;

    // Whether the store's first failure has been reported. A store that has stopped
    // accepting writes is the one failure here nobody would notice on their own: the run
    // behaves normally and simply stops keeping anything. It is said once, on the first
    // call that finds it, and not once per call after that.
    bool _store_failure_reported = false;

    // The rooms games made. The only state here that is not per session, because it
    // is what makes two instances agree about the same lobby instead of each being
    // handed its own convenient story.
    LobbyWorld _world;

    // The leaderboards the run's players have posted scores to. Here for the same reason
    // as the rooms and beside them: a board is the one thing a single game cannot make,
    // because a ranking is only worth having if somebody else is on it. It is also where
    // every call's player is learned by name, which is what makes a roster row for a
    // player who has already left answerable.
    LeaderboardWorld _leaderboards;

    // The catalogue of items, and what each player holds. Beside the other two because it is
    // the same kind of thing: an app's item definitions are not any one session's, and an
    // inventory outlives the screen that asked for it - a game that leaves and comes back is
    // handed the items it was granted the first time.
    InventoryWorld _inventory;

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
    // The live view's overrides, by the identity each entry answers for. Guarded by `_mutex` rather
    // than by a lock of its own, because every call already resolves under that one, and a second
    // lock on the path every call in the run takes would be paid for by the calls no override
    // touches.
    Overrides _overrides;
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

} // namespace steammock
