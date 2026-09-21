// ---------------------------------------------------------------------------
//  steambridge - the backend a game's stub talks to.
// ---------------------------------------------------------------------------
//  Bind a loopback port, answer whatever the stub sends, and keep a transcript.
//  Every option has an environment-free equivalent here, because this is the
//  process a person starts while debugging a game:
//
//    steambridge --scenario scenarios/example.json --transcript run.jsonl
//    steambridge --list-api
//    steambridge --show-profiles
//
//  The whole decision layer lives in bridge/server.hpp and is front-end free, so
//  this file is argument parsing, three printers and a sleep loop. A GUI is the
//  other front end for the same Server.

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "bridge/log.hpp"
#include "bridge/protocol.hpp"
#include "bridge/scenario.hpp"
#include "bridge/server.hpp"
#include "bridge/surface.hpp"

// A Ctrl+C only sets a flag; the main thread notices and stops the server
// cleanly, so the transcript is flushed and the summary is printed. The handler
// is at file scope with C linkage because that is what signal() expects - it
// cannot live in the unnamed namespace below.
namespace {
std::atomic<bool> g_interrupted{false};
}

extern "C" void steambridge_on_interrupt(int) {
    g_interrupted.store(true);
}

namespace {

using steambridge::LogLevel;

constexpr const char* kDefaultHost = "127.0.0.1";
constexpr std::uint16_t kDefaultPort = 50990;
constexpr const char* kDefaultScenario = "scenarios/example.json";
// Kept in step with DllMain's SteamBridge_Version by CI's end-to-end test, which
// runs this build against that DLL.
constexpr const char* kVersion = "0.1.0";
constexpr const char* kProgram = "steambridge";

void print_usage(std::FILE* out) {
    std::fprintf(out,
                 "%s - answer Steam API calls from a local game, so you can debug the\n"
                 "integration without running Steam.\n\n"
                 "usage: %s [options]\n\n"
                 "  --host HOST          interface to listen on (default %s)\n"
                 "  --port PORT          port to listen on; 0 picks a free one (default %u)\n"
                 "  --scenario FILE      what each game is told (default %s)\n"
                 "  --transcript FILE    append every call, in JSON lines, to this file\n"
                 "  --log-level LEVEL    error, warning, info or debug (default info)\n"
                 "  --list-api           print the calls the stub exports, then exit\n"
                 "  --show-profiles      print the scenario's games and match rules, then exit\n"
                 "  --version            print the version, then exit\n"
                 "  -h, --help           print this, then exit\n",
                 kProgram, kProgram, kDefaultHost, static_cast<unsigned>(kDefaultPort), kDefaultScenario);
}

enum class ParseResult { run, exit_ok, exit_error };

struct Options {
    std::string host = kDefaultHost;
    std::uint16_t port = kDefaultPort;
    std::string scenario = kDefaultScenario;
    std::string transcript;
    LogLevel level = LogLevel::info;
    bool list_api = false;
    bool show_profiles = false;
    std::string error;
};

bool parse_port(const std::string& text, std::uint16_t& out) {
    if (text.empty()) {
        return false;
    }
    unsigned value = 0;
    for (const char ch : text) {
        if (ch < '0' || ch > '9') {
            return false;
        }
        value = value * 10u + static_cast<unsigned>(ch - '0');
        if (value > 65535u) {
            return false;
        }
    }
    out = static_cast<std::uint16_t>(value);
    return true;
}

bool parse_level(const std::string& text, LogLevel& out) {
    if (text == "error") {
        out = LogLevel::error;
    } else if (text == "warning" || text == "warn") {
        out = LogLevel::warn;
    } else if (text == "info") {
        out = LogLevel::info;
    } else if (text == "debug") {
        out = LogLevel::debug;
    } else {
        return false;
    }
    return true;
}

ParseResult parse_args(int argc, char** argv, Options& options) {
    for (int index = 1; index < argc; ++index) {
        std::string argument = argv[index];
        std::string value;
        bool has_value = false;
        if (const std::size_t equals = argument.find('='); equals != std::string::npos) {
            value = argument.substr(equals + 1u);
            argument = argument.substr(0, equals);
            has_value = true;
        }

        const auto take_value = [&](std::string& target) {
            if (!has_value) {
                if (index + 1 >= argc) {
                    return false;
                }
                target = argv[++index];
                return true;
            }
            target = value;
            return true;
        };

        if (argument == "-h" || argument == "--help") {
            print_usage(stdout);
            return ParseResult::exit_ok;
        }
        if (argument == "--version") {
            std::printf("%s %s\n", kProgram, kVersion);
            return ParseResult::exit_ok;
        }
        if (argument == "--list-api") {
            options.list_api = true;
            continue;
        }
        if (argument == "--show-profiles") {
            options.show_profiles = true;
            continue;
        }
        if (argument == "--host") {
            if (!take_value(options.host)) {
                options.error = "--host needs a value";
                return ParseResult::exit_error;
            }
            continue;
        }
        if (argument == "--port") {
            std::string text;
            if (!take_value(text) || !parse_port(text, options.port)) {
                options.error = "--port needs a number between 0 and 65535";
                return ParseResult::exit_error;
            }
            continue;
        }
        if (argument == "--scenario") {
            if (!take_value(options.scenario)) {
                options.error = "--scenario needs a value";
                return ParseResult::exit_error;
            }
            continue;
        }
        if (argument == "--transcript") {
            if (!take_value(options.transcript)) {
                options.error = "--transcript needs a value";
                return ParseResult::exit_error;
            }
            continue;
        }
        if (argument == "--log-level") {
            std::string text;
            if (!take_value(text) || !parse_level(text, options.level)) {
                options.error = "--log-level takes error, warning, info or debug";
                return ParseResult::exit_error;
            }
            continue;
        }

        options.error = "unknown option '" + argument + "'";
        return ParseResult::exit_error;
    }
    return ParseResult::run;
}

// ---------------------------------------------------------------------------
//  The two questions a person asks a stub that has no documentation
// ---------------------------------------------------------------------------

int print_api() {
    std::size_t count = 0;
    const steambridge::SurfaceCall* calls = steambridge::api_surface_calls(count);
    std::printf("# surface '%s' revision %d, %zu calls\n", steambridge::api_surface_name(),
                steambridge::api_surface_revision(), count);
    for (std::size_t index = 0; index < count; ++index) {
        const steambridge::SurfaceCall& call = calls[index];
        std::string params;
        for (std::size_t index_param = 0; index_param < call.param_count; ++index_param) {
            if (index_param != 0u) {
                params += ", ";
            }
            params += call.params[index_param].type;
            if (call.params[index_param].out) {
                params += "*";
            }
            params += " ";
            params += call.params[index_param].name;
        }
        std::printf("  %10s %s(%s)\n", call.returns, call.name, params.c_str());
    }
    return 0;
}

int show_profiles(const std::string& path) {
    steambridge::Dispatcher dispatcher;
    std::string error;
    if (!steambridge::Dispatcher::load_file(path, dispatcher, error)) {
        std::fprintf(stderr, "%s\n", error.c_str());
        return 2;
    }
    std::printf("scenario %s\n", path.c_str());
    for (const std::string& name : dispatcher.profile_names()) {
        std::printf("  profile '%s'\n", name.c_str());
    }
    std::printf("match rules, first one wins:\n");
    for (const steambridge::MatchRule& rule : dispatcher.match_rules()) {
        std::printf("  %s\n", rule.describe().c_str());
    }
    return 0;
}

// ---------------------------------------------------------------------------
//  Serving
// ---------------------------------------------------------------------------

int serve(const Options& options) {
    steambridge::Dispatcher dispatcher;
    std::string error;
    if (!steambridge::Dispatcher::load_file(options.scenario, dispatcher, error)) {
        std::fprintf(stderr, "%s (try --scenario %s)\n", error.c_str(), kDefaultScenario);
        return 2;
    }
    const std::vector<std::string> profiles = dispatcher.profile_names();

    steambridge::ServerOptions server_options;
    server_options.host = options.host;
    server_options.port = options.port;
    server_options.transcript = options.transcript;
    server_options.log_level = options.level;

    steambridge::Server server(std::move(dispatcher), std::move(server_options));
    if (!server.start(error)) {
        std::fprintf(stderr, "%s\n", error.c_str());
        return 2;
    }

    const auto say = [&options](LogLevel level, const std::string& message) {
        if (level <= options.level) {
            steambridge::stderr_log_sink(level, message);
        }
    };
    std::string joined;
    for (const std::string& name : profiles) {
        if (!joined.empty()) {
            joined += ", ";
        }
        joined += name;
    }
    say(LogLevel::info, "protocol v" + std::to_string(steambridge::kProtocolVersion) + ", scenario " +
                            options.scenario + ", profiles " + joined);
    if (!options.transcript.empty()) {
        say(LogLevel::info, "transcript: " + options.transcript);
    }

    // Printed in a fixed shape so a test (or a script) can find the port when 0
    // was requested.
    std::printf("listening on %s:%u\n", options.host.c_str(), static_cast<unsigned>(server.port()));
    std::fflush(stdout);

    std::signal(SIGINT, steambridge_on_interrupt);
#if defined(SIGTERM)
    std::signal(SIGTERM, steambridge_on_interrupt);
#endif
    while (!g_interrupted.load()) {
        // The accept loop runs on its own thread; this one only has to notice a
        // Ctrl+C and stop the server cleanly, which flushes the transcript.
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    server.stop();
    say(LogLevel::info, "done: " + server.summary());
    return 0;
}

}  // namespace

int main(int argc, char** argv) {
    Options options;
    switch (parse_args(argc, argv, options)) {
        case ParseResult::exit_ok:
            return 0;
        case ParseResult::exit_error:
            std::fprintf(stderr, "%s: %s\n\n", kProgram, options.error.c_str());
            print_usage(stderr);
            return 2;
        case ParseResult::run:
            break;
    }

    if (options.list_api) {
        return print_api();
    }
    if (options.show_profiles) {
        return show_profiles(options.scenario);
    }
    return serve(options);
}
