// ============================================================================
//  C++ unit tests: the server, in process.
// ----------------------------------------------------------------------------
//  test_backend checks the backend's decisions without opening a socket. This
//  one binds a real port, drives it through the same Transport a game's stub
//  uses, and then asks the server what it saw.
//
//  Those snapshots - the sessions it knows, the calls it recorded, the counters
//  and the summary - are what a live view draws, and until now nothing called
//  them at all. Exercising them here is what keeps them honest instead of
//  speculative.
//
//  Exits non-zero if a check fails.
// ============================================================================

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "bridge/client.hpp"
#include "bridge/json_read.hpp"
#include "bridge/log.hpp"
#include "bridge/protocol.hpp"
#include "bridge/scenario.hpp"
#include "bridge/server.hpp"
#include "bridge/transport.hpp"

namespace {

using steammock::Json;

int g_failures = 0;

void check(const char* what, bool ok) {
    std::printf("  [%s] %s\n", ok ? "ok  " : "FAIL", what);
    if (!ok) {
        ++g_failures;
    }
}

// The server serves each connection on its own thread, so what it has been told
// arrives a moment after the socket says it did.
template <typename Predicate> bool wait_until(Predicate ready, double seconds) {
    const auto deadline = std::chrono::steady_clock::now() +
                          std::chrono::milliseconds(static_cast<long long>(seconds * 1000.0));
    for (;;) {
        if (ready()) {
            return true;
        }
        if (std::chrono::steady_clock::now() >= deadline) {
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

Json hello_message() {
    Json message = Json::object();
    message["type"] = Json("hello");
    message["v"] = Json(steammock::kProtocolVersion);
    message["exe"] = Json("game.exe");
    message["arch"] = Json("x64");
    message["pid"] = Json(1234);
    return message;
}

Json call_message(const char* name, std::int64_t seq) {
    Json message = Json::object();
    message["type"] = Json("call");
    message["v"] = Json(steammock::kProtocolVersion);
    message["seq"] = Json(seq);
    message["name"] = Json(name);
    message["args"] = Json::object();
    return message;
}

std::string answer_of(const Json& reply) {
    const Json* answer = steammock::json_member(reply, "answer");
    return answer != nullptr && answer->is_string() ? steammock::as_string(*answer) : std::string();
}

// One framed message out, one framed answer back: exactly what a stub does.
bool exchange(steammock::TcpTransport& client, const Json& message, Json& reply) {
    std::string text;
    if (!client.exchange(message.dump(), text)) {
        return false;
    }
    return steammock::parse(text, reply);
}

// One scripted call, one answered from state, one nobody has an opinion about,
// so every "via" the transcript records shows up at least once.
const char* kScenario = "{\"profiles\":{\"default\":{\"app_id\":480,\"stats\":{\"Deaths\":3},"
                        "\"scripted\":{\"SteamAPI_Init\":{\"ret\":true}}}}}";

void test_what_the_server_saw() {
    std::printf("[:] a real connection, and what the server says about it\n");

    Json scenario;
    if (!steammock::parse(kScenario, scenario)) {
        check("the test scenario parses", false);
        return;
    }

    steammock::ServerOptions options;
    options.port = 0;  // let the OS pick, so the test can run beside anything else
    options.log_level = steammock::LogLevel::error;  // keep the test output clean
    steammock::Server server(steammock::Dispatcher(scenario), options);

    std::string error;
    check("the server binds a free port", server.start(error));
    if (!error.empty()) {
        std::printf("        %s\n", error.c_str());
        return;
    }
    check("it reports the port it actually bound", server.port() != 0);
    check("a fresh server has no sessions", server.sessions().empty());
    check("a fresh server has no calls", server.call_count() == 0u);
    check("a fresh server has nothing unanswered", server.unanswered_count() == 0u);

    steammock::TcpTransport client;
    check("a client connects through the transport a stub uses",
          client.connect("127.0.0.1", server.port()));

    Json reply;
    check("the handshake is answered", exchange(client, hello_message(), reply));
    const Json* session = steammock::json_member(reply, "session");
    const std::string session_id =
        session != nullptr && session->is_string() ? steammock::as_string(*session) : std::string();
    check("the welcome names the session", !session_id.empty());
    check("the welcome names the profile",
          steammock::json_member(reply, "profile") != nullptr &&
              steammock::as_string(*steammock::json_member(reply, "profile")) == "default");

    check("a scripted call is answered", exchange(client, call_message("SteamAPI_Init", 1), reply));
    check("a call the state machine knows is answered",
          exchange(client, call_message("SteamAPI_GetHSteamUser", 2), reply));
    check("a call nobody answers is declined",
          exchange(client, call_message("SteamAPI_Shutdown", 3), reply) &&
              answer_of(reply) == "default");

    // Still connected at this point, so the snapshot should say so.
    check("the live session is listed",
          wait_until([&server] { return server.sessions().size() == 1u; }, 5.0));
    {
        const std::vector<steammock::SessionSnapshot> sessions = server.sessions();
        if (sessions.size() == 1u) {
            check("the snapshot names the game", sessions[0].exe == "game.exe");
            check("the snapshot carries the process id", sessions[0].pid == 1234);
            check("the snapshot carries the architecture", sessions[0].arch == "x64");
            check("the snapshot carries the profile name", sessions[0].profile == "default");
            check("the snapshot carries the profile's stats",
                  sessions[0].stats.size() == 1u && sessions[0].stats[0].first == "Deaths" &&
                      sessions[0].stats[0].second == 3);
            check("the snapshot says the game is still here", sessions[0].connected);
            check("the snapshot is stamped when the game arrived",
                  sessions[0].connected_at_unix_ms > 0);
        }
    }

    client.close();

    check("the server catches up with the game",
          wait_until([&server] { return server.call_count() == 3u; }, 5.0));
    check("the server notices the game leaving", wait_until(
                                                     [&server] {
                                                         const auto sessions = server.sessions();
                                                         return sessions.size() == 1u &&
                                                                !sessions[0].connected;
                                                     },
                                                     5.0));

    check("every call was counted", server.call_count() == 3u);
    check("the declined one is counted as unanswered", server.unanswered_count() == 1u);

    const std::vector<steammock::CallRecord> records = server.records();
    check("every call is in the history", records.size() == 3u);
    if (records.size() == 3u) {
        check("the history keeps the order the game called in",
              records[0].call == "SteamAPI_Init" && records[1].call == "SteamAPI_GetHSteamUser" &&
                  records[2].call == "SteamAPI_Shutdown");
        check("a record names its session", records[0].session == session_id);
        check("a record echoes the sequence it answered", records[1].seq == 2);
        check("a record says the scenario answered it", records[0].via == "scripted");
        check("a record says the state machine answered it", records[1].via == "state");
        check("a record says nobody answered it", records[2].via == "none" && !records[2].answered);
        // A duration cannot be negative, so "it is not negative" is a check that
        // cannot fail. What can fail is a duration that was never taken: a call
        // answered in this process takes milliseconds, and anything else is a
        // stamp that is not this call's.
        check("a record is timed, and not by a placeholder", records[0].ms < 10000.0);
        check("a record is stamped for a live view", records[0].at_unix_ms > 0);
    }

    {
        const std::vector<steammock::SessionSnapshot> sessions = server.sessions();
        check("the snapshot keeps the session id",
              sessions.size() == 1u && sessions[0].id == session_id);
        check("the snapshot counts that session's calls",
              sessions.size() == 1u && sessions[0].call_count == 3u);
    }

    const std::string summary = server.summary();
    check("the summary counts the run",
          summary.find("1 game session(s), 3 call(s)") != std::string::npos);
    check("the summary separates answered from declined",
          summary.find("2 answered") != std::string::npos &&
              summary.find("1 left to the stub's defaults") != std::string::npos);

    server.stop();
    // The destructor stops it again, so stopping a stopped server has to be
    // harmless - and the run summary has to survive it.
    server.stop();
    check("the run summary survives being stopped",
          server.summary().find("3 call(s)") != std::string::npos);
}

// Four threads stopping at once, which is what stop() used to be unable to survive:
// `_stopping` is set before the work begins, so a second caller used to arrive at a
// guard that was already true, skip it, and close the same sockets and join the same
// std::thread a second time. A handle closed twice and a thread joined twice are both
// faults that say nothing when they happen, so what this test asks for is no crash.
void test_four_threads_stop_at_once() {
    std::printf("[:] four threads calling stop() at once\n");

    for (int attempt = 0; attempt < 5; ++attempt) {
        Json scenario;
        if (!steammock::parse(kScenario, scenario)) {
            check("the test scenario parses", false);
            return;
        }

        steammock::ServerOptions options;
        options.port = 0;
        options.log_level = steammock::LogLevel::error;
        steammock::Server server(steammock::Dispatcher(scenario), options);

        std::string error;
        if (!server.start(error)) {
            check("the server binds a free port", false);
            std::printf("        %s\n", error.c_str());
            return;
        }

        // A game is attached first, so there is a listening socket, a worker thread and a
        // session for the four to share rather than an empty shell.
        steammock::TcpTransport client;
        Json reply;
        const bool attached = client.connect("127.0.0.1", server.port()) &&
                              exchange(client, hello_message(), reply) &&
                              wait_until([&server] { return server.sessions().size() == 1u; }, 5.0);
        if (!attached) {
            check("a game is attached before anything stops", false);
            return;
        }

        // Released together, so all four are inside stop() rather than queued behind
        // whichever thread happened to get there first.
        std::atomic<bool> go{false};
        std::vector<std::thread> stoppers;
        stoppers.reserve(4);
        for (int index = 0; index < 4; ++index) {
            stoppers.emplace_back([&server, &go] {
                while (!go.load(std::memory_order_acquire)) {
                    std::this_thread::yield();
                }
                server.stop();
            });
        }
        go.store(true, std::memory_order_release);
        for (std::thread& stopper : stoppers) {
            stopper.join();
        }

        // The run's record is what a stop must not take with it, and the session it
        // names has to be marked as gone - a game whose socket is shut down is the one
        // thing every stop here has in common.
        check("four stops at once leave the run's record readable",
              server.summary().find("1 game session(s)") != std::string::npos);
        const std::vector<steammock::SessionSnapshot> sessions = server.sessions();
        check("and the session it names is marked as gone",
              sessions.size() == 1u && !sessions[0].connected);
        client.close();
        server.stop();  // and a fifth, afterwards, is still harmless
    }
}

// A server listens once. `_stopping` is what every reader decides by and nothing
// clears it, so a second start() on a stopped server would bind a listener whose accept
// loop refuses every game that arrived - a port that looks bound and serves nobody,
// which is worse than a refusal. Starting one twice is the same leak on the other side:
// the first listener and its thread would have nothing pointing at them.
void test_a_run_is_not_restartable() {
    std::printf("[:] a server starts once, and says so when asked twice\n");

    Json scenario;
    if (!steammock::parse(kScenario, scenario)) {
        check("the test scenario parses", false);
        return;
    }

    steammock::ServerOptions options;
    options.port = 0;
    options.log_level = steammock::LogLevel::error;
    steammock::Server server(steammock::Dispatcher(scenario), options);

    std::string error;
    check("the first start binds a port", server.start(error));
    const std::uint16_t port = server.port();

    error.clear();
    check("a second start is refused", !server.start(error));
    check("and it says why", error.find("already started") != std::string::npos);
    check("the running server is left alone", server.port() == port);

    server.stop();
    error.clear();
    check("a start after a stop is refused too", !server.start(error));
    check("and says the same thing", error.find("already started") != std::string::npos);
    check("with nothing left listening", server.call_count() == 0u);
}

// A connect has to be bounded by the timeout the caller asked for. It is not the
// socket's own receive/send timeouts that do that - they are read and write timeouts,
// and a `::connect` is neither, so on Windows a blocking one waits for the TCP stack
// to give up on its SYN retransmissions (~21 seconds) whatever those say. The
// transport makes the connect on a non-blocking socket and waits for the deadline
// itself, and this is the check that it does: a game pointed at an address nothing
// answers on is a game that gets its defaults back in the time it was promised, not
// twenty seconds later.
//
// 192.0.2.0/24 is TEST-NET-1 (RFC 5737): reserved for documentation, routed nowhere,
// so the SYN goes into a hole rather than being refused. A host with no route at all
// fails this connect immediately instead, which is still a pass - the check is that
// nothing here takes longer than the timeout says.
void test_a_connect_is_bounded_by_its_timeout() {
    std::printf("[:] a connect gives up when the timeout says, not when the stack does\n");

    steammock::TcpTransport client;
    client.set_timeout_ms(400);

    const auto started = std::chrono::steady_clock::now();
    const bool connected = client.connect("192.0.2.1", 50990);
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                             std::chrono::steady_clock::now() - started)
                             .count();

    check("nothing answers, so the connect fails", !connected);
    check("it is not left connected", !client.is_connected());
    // Generous against the 400 ms asked for and still well under the stack's own
    // timer, so this fails on the old blocking connect and cannot fail on a slow
    // machine with the deadline in place.
    check("and it gave up on the transport's deadline, not the stack's", elapsed < 5000);
    std::printf("        gave up after %lld ms\n", static_cast<long long>(elapsed));
}

// What a stalled backend costs a game's own threads: finding 16 of the review this tree went
// through. `Client::call` holds one lock across a whole round trip, so a call that is out waiting
// for an answer is what every other thread's call waits behind - and the price of that is not one
// shared wait, it is *each* caller's own timeout, one after another.
//
// Two arms, against the same server in the same run, because one arm cannot tell a client's lock
// from the backend's own queueing:
//
//   1. one client - the process-wide one a game gets - with three threads calling in it: the
//      three timeouts come one after another;
//   2. three clients of their own, a TcpTransport each, so three connections: the same three
//      timeouts, but at the same time, because the server answers each connection on its own
//      thread and a call it never answers in time costs each of those one timeout in parallel.
//
// The second arm is what says the server is not the serializer, so the difference between the
// arms is the lock. The assertions are on the two figures rather than on the lock, on purpose:
// the day the transport stops holding the whole round trip, the first figure comes down to the
// second's and this is the test that will say so - see docs/architecture.md, "The socket layer".
void test_a_stalled_backend_costs_each_queued_caller() {
    std::printf("[:] a backend that never answers in time, and the calls queued behind it\n");

    constexpr int kCallers = 3;
    constexpr long long kTimeoutMs = 600;
    // Longer than the timeout, so no call in either arm is ever answered: the delay is a stalled
    // backend, not a slow one, and that is what finding 16 is about.
    constexpr long long kDelayMs = 3000;

    Json scenario;
    const std::string text =
        "{\"profiles\":{\"default\":{\"app_id\":480,\"scripted\":{\"SteamAPI_Init\":"
        "{\"ret\":true,\"delay_ms\":" +
        std::to_string(kDelayMs) + "}}}}}";
    if (!steammock::parse(text, scenario)) {
        check("the stalled scenario parses", false);
        return;
    }

    steammock::ServerOptions options;
    options.port = 0;
    options.log_level = steammock::LogLevel::error;
    steammock::Server server(steammock::Dispatcher(scenario), options);

    std::string error;
    if (!server.start(error)) {
        check("the stalled server binds a free port", false);
        return;
    }

    // The client is one process-wide object that reads its environment once, on its first call -
    // which is the first line of the first arm below, so this is where the port and the timeout
    // can still be chosen. The timeout is the whole of what the arms measure against.
    const std::string port = std::to_string(server.port());
    _putenv_s("STEAMMOCK_HOST", "127.0.0.1");
    _putenv_s("STEAMMOCK_PORT", port.c_str());
    _putenv_s("STEAMMOCK_TIMEOUT_MS", std::to_string(kTimeoutMs).c_str());
    _putenv_s("STEAMMOCK_LOG_LEVEL", "error");

    // --- arm 1: a game's own threads, behind the one lock ---------------------------------
    const auto queued_started = std::chrono::steady_clock::now();
    const int answered = [&] {
        int count = 0;
        std::vector<std::thread> callers;
        callers.reserve(kCallers);
        for (int index = 0; index < kCallers; ++index) {
            callers.emplace_back([&count] {
                Json reply;
                if (steammock::invoke("SteamAPI_Init", Json::object(), reply)) {
                    ++count;
                }
            });
        }
        for (std::thread& caller : callers) {
            caller.join();
        }
        return count;
    }();
    const auto queued = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::steady_clock::now() - queued_started)
                            .count();

    // --- arm 2: three clients of their own, one connection each ---------------------------
    // Attached and through the handshake *before* the clock starts, so what is measured is a
    // call that times out and not three connections being made - the connect has its own
    // deadline and its own test above.
    std::array<steammock::TcpTransport, kCallers> transports;
    bool attached = true;
    Json welcome;
    for (steammock::TcpTransport& transport : transports) {
        transport.set_timeout_ms(static_cast<unsigned>(kTimeoutMs));
        attached = attached && transport.connect("127.0.0.1", server.port()) &&
                   exchange(transport, hello_message(), welcome);
    }
    check("three clients of their own attached", attached);

    const auto own_started = std::chrono::steady_clock::now();
    const int own_answered = [&] {
        int count = 0;
        std::vector<std::thread> callers;
        callers.reserve(kCallers);
        for (int index = 0; index < kCallers; ++index) {
            callers.emplace_back([&transports, &count, index] {
                Json answer;
                if (exchange(transports[static_cast<std::size_t>(index)],
                             call_message("SteamAPI_Init", 2), answer)) {
                    ++count;
                }
            });
        }
        for (std::thread& caller : callers) {
            caller.join();
        }
        return count;
    }();
    const auto own = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::steady_clock::now() - own_started)
                         .count();

    check("nobody was answered: the backend is stalled, not slow",
          answered == 0 && own_answered == 0);
    // More than two timeouts for three threaded callers, and less than two for three that each
    // have a connection. Both are about the shape rather than the exact figure, so a loaded
    // machine cannot move either one across.
    check("a game's three threads spent one timeout each, one after the other",
          queued >= 2 * kTimeoutMs);
    check("three clients of their own spent their timeouts at the same time",
          own <= 2 * kTimeoutMs);
    std::printf("        %d thread(s) behind one client: %lld ms | %d client(s) of their own:"
                " %lld ms | %lld ms of timeout each\n",
                kCallers, static_cast<long long>(queued), kCallers, static_cast<long long>(own),
                kTimeoutMs);

    server.stop();
}

// A megabyte through the wire, and the two caps that refuse more. Nothing in this harness has
// ever sent one: the largest frame in a whole rig run is a 1 KB P2P packet carried as hex, and
// the cap is 4 MB, so the middle of the range is untested in both directions - while a screenshot
// buffer, the biggest thing this API hands over as a value, is about 1.2 MB. docs/architecture.md,
// "The socket layer", is where this feeds in: whatever replaces the transport has to keep both
// halves of it, so both are pinned here rather than assumed.
void test_a_megabyte_goes_either_way() {
    std::printf("[:] a megabyte through the wire, and the caps that refuse more\n");

    // Carried as a size from the start: `1024 * 1024` in an int is a product that widens, which is
    // the sort of arithmetic this check exists to keep out of a buffer length.
    constexpr std::size_t kMegabyte = std::size_t{1024} * 1024;
    const std::string big(kMegabyte, 'a');
    const std::string over(5 * kMegabyte, 'b');  // more than kMaxFrameBytes

    // One call answered with as much `out` as it was sent, so a single exchange covers both
    // directions; and one answered with more than the wire allows, to see the cap refuse it.
    Json answer;
    answer["ret"] = Json(true);
    answer["out"] = Json::object();
    answer["out"]["pvData"] = Json(big);
    Json too_big;
    too_big["ret"] = Json(true);
    too_big["out"] = Json::object();
    too_big["out"]["pvData"] = Json(over);
    Json scripted = Json::object();
    scripted["SteamAPI_Test_A_Megabyte"] = answer;
    scripted["SteamAPI_Test_Too_Big"] = too_big;
    Json profile = Json::object();
    profile["app_id"] = Json(480);
    profile["scripted"] = scripted;
    Json profiles = Json::object();
    profiles["default"] = profile;
    Json scenario = Json::object();
    scenario["profiles"] = profiles;

    const std::filesystem::path transcript =
        std::filesystem::temp_directory_path() / "steammock-big-frame.jsonl";
    std::error_code ignored;
    std::filesystem::remove(transcript, ignored);

    steammock::ServerOptions options;
    options.port = 0;
    options.log_level = steammock::LogLevel::error;
    options.transcript = transcript.string();
    steammock::Server server(steammock::Dispatcher(scenario), options);

    std::string error;
    if (!server.start(error)) {
        check("the big-frame server binds a free port", false);
        return;
    }

    steammock::TcpTransport client;
    client.set_timeout_ms(5000);  // a megabyte is not a stall, but it is not a ping either
    Json reply;
    const bool attached =
        client.connect("127.0.0.1", server.port()) && exchange(client, hello_message(), reply);
    check("a client attached", attached);

    // Both directions at a megabyte: the arguments carry one, the answer carries one back, and the
    // server wrote one to the transcript on the way past.
    Json big_call = call_message("SteamAPI_Test_A_Megabyte", 1);
    big_call["args"]["pvData"] = Json(big);
    Json big_reply;
    check("a megabyte of arguments was answered", exchange(client, big_call, big_reply));
    const Json* echoed = steammock::json_member(big_reply, "out");
    check("and a megabyte of out-parameters came back intact",
          echoed != nullptr && steammock::as_string_member(*echoed, "pvData").size() == big.size());
    if (echoed != nullptr) {
        check("  byte for byte", steammock::as_string_member(*echoed, "pvData") == big);
    }

    // ...and the transcript has the same megabyte in it, which is the other path a big frame
    // travels on: one record for the one call so far, written whole by the same code a small one
    // goes through.
    const auto read_transcript = [&transcript] {
        if (!std::filesystem::exists(transcript)) {
            return std::string();
        }
        std::ifstream file(transcript, std::ios::binary);
        return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    };
    const std::string recorded = read_transcript();
    check("the transcript holds the megabyte too", recorded.find(big) != std::string::npos);
    check("one call, one line of it", std::count(recorded.begin(), recorded.end(), '\n') == 1);

    // Over the cap, outbound: refused, and the connection is left usable rather than truncated -
    // nothing was sent, so there is nothing to be out of step about.
    Json over_call = call_message("SteamAPI_Test_A_Megabyte", 2);
    over_call["args"]["pvData"] = Json(over);
    Json over_reply;
    check("a frame larger than the cap is refused rather than sent",
          !exchange(client, over_call, over_reply));
    check("and the connection is still good afterwards",
          exchange(client, call_message("SteamAPI_Test_A_Megabyte", 3), reply));

    // Over the cap, inbound: the answer is what is too big, and the client hangs up rather than
    // reading a length it cannot honour - so the next call has to be a new connection.
    steammock::TcpTransport other;
    other.set_timeout_ms(5000);
    Json other_reply;
    const bool other_attached =
        other.connect("127.0.0.1", server.port()) && exchange(other, hello_message(), other_reply);
    check("a second client attached", other_attached);
    check("an answer larger than the cap is refused",
          !exchange(other, call_message("SteamAPI_Test_Too_Big", 4), other_reply));
    check("and the client that refused it is not connected any more", !other.is_connected());

    // Three calls the server answered (the refused request never left), three lines - and the
    // megabyte one is still whole, with the records on either side of it intact.
    const std::string all = read_transcript();
    check("the records the server answered are all there",
          std::count(all.begin(), all.end(), '\n') == 3);
    check("and the megabyte in the middle of them is whole",
          all.find(big) != std::string::npos && std::count(all.begin(), all.end(), '\n') == 3);

    other.close();
    client.close();
    server.stop();
    std::filesystem::remove(transcript, ignored);
}

// The call history in memory is a window, and a reader's cursor is an absolute position
// in the run rather than an offset into that window. It used to be an unbounded vector
// that every snapshot copied whole while holding the state lock, and a game can make
// thousands of calls a second: 26,228 in fourteen seconds in one recording here. The
// edge is what this checks - the oldest are the ones dropped, the positions stay
// absolute, and a cursor that has fallen off the front reads from the start of the
// window instead of being told there is nothing left, which would silently stop a live
// view drawing.
//
// It drives the real constant rather than a test-only window size, which is why it makes
// 20,001 calls through a socket and takes a few seconds: a check on the edge of the actual
// history is worth more than a fast one on an edge no run has.
void test_the_history_is_a_window() {
    std::printf("[:] the history in memory has an edge, and the positions stay absolute\n");

    Json scenario;
    if (!steammock::parse(kScenario, scenario)) {
        check("the test scenario parses", false);
        return;
    }

    steammock::ServerOptions options;
    options.port = 0;
    options.log_level = steammock::LogLevel::error;
    steammock::Server server(steammock::Dispatcher(scenario), options);

    std::string error;
    if (!server.start(error)) {
        check("the server binds a free port", false);
        std::printf("        %s\n", error.c_str());
        return;
    }

    steammock::TcpTransport client;
    Json reply;
    if (!client.connect("127.0.0.1", server.port()) || !exchange(client, hello_message(), reply)) {
        check("a client is attached", false);
        server.stop();
        return;
    }

    // One more call than the window holds, so exactly one record has to have fallen off
    // the front. The sequence number is the position, which is what lets the check below
    // say *which* records are here rather than only how many.
    const std::size_t calls = steammock::Server::kMaxRecords + 1u;
    bool answered = true;
    for (std::size_t index = 0; index < calls; ++index) {
        if (!exchange(client,
                      call_message("SteamAPI_Shutdown", static_cast<std::int64_t>(index + 1u)),
                      reply)) {
            answered = false;
            break;
        }
    }
    check("every call is answered", answered);

    check("every call was counted", server.call_count() == calls);
    check("the history says how many the run made, not how many it holds",
          server.record_count() == calls);

    const std::vector<steammock::CallRecord> held = server.records();
    check("and it holds exactly the window's worth", held.size() == steammock::Server::kMaxRecords);
    check("the oldest recorded call is the first one the window kept",
          server.records_begin() == 1u);
    if (held.size() == steammock::Server::kMaxRecords) {
        check("the records it kept are the newest ones",
              held.front().seq == 2 && held.back().seq == static_cast<std::int64_t>(calls));
    }

    // A cursor that has fallen off the front, and one that is exactly at the edge: both
    // read the retained window, and neither is told the history is empty.
    check("a cursor off the front reads the whole window",
          server.records_since(0u).size() == steammock::Server::kMaxRecords);
    check("a cursor at the edge reads the whole window too",
          server.records_since(server.records_begin()).size() == steammock::Server::kMaxRecords);
    check("a cursor at the end reads nothing", server.records_since(server.record_count()).empty());

    // ...and the incremental read a live view actually makes still lines up with what the
    // window holds, so a poll that is one record behind takes exactly one record.
    const std::vector<steammock::CallRecord> tail = server.records_since(calls - 1u);
    check("one behind the end is one record",
          tail.size() == 1u && tail[0].seq == static_cast<std::int64_t>(calls));

    client.close();
    server.stop();
}

}  // namespace

int run() {
    std::printf("[+] SteamMock server tests\n\n");
    test_what_the_server_saw();
    test_four_threads_stop_at_once();
    test_a_run_is_not_restartable();
    test_a_connect_is_bounded_by_its_timeout();
    test_a_stalled_backend_costs_each_queued_caller();
    test_a_megabyte_goes_either_way();
    test_the_history_is_a_window();

    if (g_failures == 0) {
        std::printf("\n[+] all checks passed\n");
    } else {
        std::printf("\n[-] %d check(s) FAILED\n", g_failures);
    }
    return g_failures == 0 ? 0 : 1;
}

// An exception escaping `main` terminates the process with no message at all, and the
// only realistic source in a test is a failed allocation. Report it the way a failing
// check is reported instead, so ctest's output says what happened.
int main() {
    try {
        return run();
    } catch (const std::exception& error) {
        std::printf("\n[-] the test itself threw: %s\n", error.what());
        return 1;
    } catch (...) {
        std::printf("\n[-] the test itself threw something that is not a std::exception\n");
        return 1;
    }
}
