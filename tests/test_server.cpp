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

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <string>
#include <thread>

#include "bridge/json.hpp"
#include "bridge/log.hpp"
#include "bridge/protocol.hpp"
#include "bridge/scenario.hpp"
#include "bridge/server.hpp"
#include "bridge/transport.hpp"

namespace {

using steambridge::Json;

int g_failures = 0;

void check(const char* what, bool ok) {
    std::printf("  [%s] %s\n", ok ? "ok  " : "FAIL", what);
    if (!ok) {
        ++g_failures;
    }
}

// The server serves each connection on its own thread, so what it has been told
// arrives a moment after the socket says it did.
template <typename Predicate>
bool wait_until(Predicate ready, double seconds) {
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
    message.set("type", Json::string("hello"));
    message.set("v", Json::integer(steambridge::kProtocolVersion));
    message.set("exe", Json::string("game.exe"));
    message.set("arch", Json::string("x64"));
    message.set("pid", Json::integer(1234));
    return message;
}

Json call_message(const char* name, std::int64_t seq) {
    Json message = Json::object();
    message.set("type", Json::string("call"));
    message.set("v", Json::integer(steambridge::kProtocolVersion));
    message.set("seq", Json::integer(seq));
    message.set("name", Json::string(name));
    message.set("args", Json::object());
    return message;
}

std::string answer_of(const Json& reply) {
    const Json* answer = reply.find("answer");
    return answer != nullptr && answer->is_string() ? answer->as_string() : std::string();
}

// One framed message out, one framed answer back: exactly what a stub does.
bool exchange(steambridge::TcpTransport& client, const Json& message, Json& reply) {
    std::string text;
    if (!client.exchange(message.dump(), text)) {
        return false;
    }
    return Json::parse(text, reply);
}

// One scripted call, one answered from state, one nobody has an opinion about,
// so every "via" the transcript records shows up at least once.
const char* kScenario =
    "{\"profiles\":{\"default\":{\"app_id\":480,\"stats\":{\"Deaths\":3},"
    "\"scripted\":{\"SteamAPI_Init\":{\"ret\":true}}}}}";

void test_what_the_server_saw() {
    std::printf("[:] a real connection, and what the server says about it\n");

    Json scenario;
    if (!Json::parse(kScenario, scenario)) {
        check("the test scenario parses", false);
        return;
    }

    steambridge::ServerOptions options;
    options.port = 0;   // let the OS pick, so the test can run beside anything else
    options.log_level = steambridge::LogLevel::error;   // keep the test output clean
    steambridge::Server server(steambridge::Dispatcher(scenario), options);

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

    steambridge::TcpTransport client;
    check("a client connects through the transport a stub uses",
          client.connect("127.0.0.1", server.port()));

    Json reply;
    check("the handshake is answered", exchange(client, hello_message(), reply));
    const Json* session = reply.find("session");
    const std::string session_id =
        session != nullptr && session->is_string() ? session->as_string() : std::string();
    check("the welcome names the session", !session_id.empty());
    check("the welcome names the profile",
          reply.find("profile") != nullptr && reply.find("profile")->as_string() == "default");

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
        const std::vector<steambridge::SessionSnapshot> sessions = server.sessions();
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

    const std::vector<steambridge::CallRecord> records = server.records();
    check("every call is in the history", records.size() == 3u);
    if (records.size() == 3u) {
        check("the history keeps the order the game called in",
              records[0].call == "SteamAPI_Init" &&
                  records[1].call == "SteamAPI_GetHSteamUser" &&
                  records[2].call == "SteamAPI_Shutdown");
        check("a record names its session", records[0].session == session_id);
        check("a record echoes the sequence it answered", records[1].seq == 2);
        check("a record says the scenario answered it", records[0].via == "scripted");
        check("a record says the state machine answered it", records[1].via == "state");
        check("a record says nobody answered it", records[2].via == "none" && !records[2].answered);
        check("a record is timed", records[0].ms >= 0.0);
        check("a record is stamped for a live view", records[0].at_unix_ms > 0);
    }

    {
        const std::vector<steambridge::SessionSnapshot> sessions = server.sessions();
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
    check("stopping twice is harmless", (server.stop(), true));
}

}  // namespace

int main() {
    std::printf("[+] SteamApiBridge server tests\n\n");
    test_what_the_server_saw();

    if (g_failures == 0) {
        std::printf("\n[+] all checks passed\n");
    } else {
        std::printf("\n[-] %d check(s) FAILED\n", g_failures);
    }
    return g_failures == 0 ? 0 : 1;
}
