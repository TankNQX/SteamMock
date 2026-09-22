// ============================================================================
//  End-to-end test: the real backend, the real stub DLL, a real game process.
// ----------------------------------------------------------------------------
//    test_end_to_end --server build/steambridge.exe \
//                     --stub build/steam_api64.dll \
//                     --game build/fake_game.exe \
//                     --scenario scenarios/example.json
//
//  It starts the backend as a subprocess on a port of its own choosing, runs the
//  fake game with that port in its environment, then checks both sides of the
//  conversation: what the game was told, and what the backend recorded about it.
//
//  This is the only test that exercises the compiled stub against the running
//  server over a real socket - the framing, the handshake, out-parameters, the
//  transcript, and the command line itself. Windows only, because the stub is a
//  DLL that has to be loaded in a separate process, the way a game loads it.
//
//  Exits non-zero if a check fails.
// ============================================================================

#include <windows.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <thread>
#include <vector>

#include "bridge/json.hpp"
#include "child_process.hpp"

using steambridge_test::ChildProcess;
using steambridge_test::quote;

namespace {

int g_failures = 0;

void check(const char* what, bool ok, const std::string& detail = std::string()) {
    if (ok) {
        std::printf("  [ok  ] %s\n", what);
    } else {
        ++g_failures;
        std::printf("  [FAIL] %s%s\n", what, detail.empty() ? "" : (" - " + detail).c_str());
    }
}

bool file_exists(const std::string& path) {
    std::FILE* file = std::fopen(path.c_str(), "rb");
    if (file == nullptr) {
        return false;
    }
    std::fclose(file);
    return true;
}

bool read_file(const std::string& path, std::string& out) {
    std::FILE* file = std::fopen(path.c_str(), "rb");
    if (file == nullptr) {
        return false;
    }
    char buffer[4096];
    std::size_t got = 0;
    while ((got = std::fread(buffer, 1, sizeof(buffer), file)) > 0) {
        out.append(buffer, got);
    }
    std::fclose(file);
    return true;
}

std::string trimmed(std::string text) {
    while (!text.empty() && (text.front() == ' ' || text.front() == '\t' || text.front() == '\r')) {
        text.erase(text.begin());
    }
    while (!text.empty() && (text.back() == ' ' || text.back() == '\t' || text.back() == '\r')) {
        text.pop_back();
    }
    return text;
}

// One `key=value` per line, each key unique, which is what fake_game prints.
std::map<std::string, std::string> parse_key_values(const std::string& output) {
    std::map<std::string, std::string> values;
    std::size_t position = 0;
    while (position <= output.size()) {
        std::size_t end = output.find('\n', position);
        if (end == std::string::npos) {
            end = output.size();
        }
        const std::string line = output.substr(position, end - position);
        position = end + 1;
        const std::size_t equals = line.find('=');
        if (equals == std::string::npos) {
            continue;
        }
        values[trimmed(line.substr(0, equals))] = trimmed(line.substr(equals + 1));
    }
    return values;
}

std::string value_of(const std::map<std::string, std::string>& values, const char* key) {
    const auto found = values.find(key);
    return found == values.end() ? std::string() : found->second;
}

std::vector<steambridge::Json> read_transcript(const std::string& path, bool& ok) {
    std::vector<steambridge::Json> records;
    std::string text;
    ok = read_file(path, text);
    if (!ok) {
        return records;
    }
    std::size_t position = 0;
    while (position <= text.size()) {
        std::size_t end = text.find('\n', position);
        if (end == std::string::npos) {
            end = text.size();
        }
        const std::string line = text.substr(position, end - position);
        position = end + 1;
        if (trimmed(line).empty()) {
            continue;
        }
        steambridge::Json record;
        if (steambridge::Json::parse(line, record)) {
            records.push_back(std::move(record));
        }
    }
    return records;
}

std::string text_member(const steambridge::Json& record, const char* key) {
    const steambridge::Json* member = record.find(key);
    return member != nullptr && member->is_string() ? member->as_string() : std::string();
}

bool ends_with(const std::string& text, const std::string& tail) {
    return text.size() >= tail.size() &&
           text.compare(text.size() - tail.size(), tail.size(), tail) == 0;
}

// Pulls the bound address out of the backend's opening line. The line has to be
// complete before it is believed: reading a half-delivered line would hand the
// game a truncated port number and it would connect to the wrong place.
bool extract_address(const std::string& output, std::string& host, std::string& port) {
    const std::string marker = "listening on";
    const std::size_t at = output.find(marker);
    if (at == std::string::npos) {
        return false;
    }
    const std::size_t start = at + marker.size();
    const std::size_t end = output.find('\n', start);
    if (end == std::string::npos) {
        return false;
    }
    const std::string address = trimmed(output.substr(start, end - start));
    const std::size_t colon = address.rfind(':');
    if (colon == std::string::npos || colon + 1 >= address.size()) {
        return false;
    }
    host = address.substr(0, colon);
    port = address.substr(colon + 1);
    for (const char ch : port) {
        if (ch < '0' || ch > '9') {
            return false;
        }
    }
    return !host.empty();
}

}  // namespace

int main(int argc, char** argv) {
    std::string server_path;
    std::string stub_path;
    std::string game_path;
    std::string scenario_path;
    double timeout = 30.0;

    for (int index = 1; index < argc; ++index) {
        const std::string argument = argv[index];
        const auto take = [&](std::string& target) {
            if (index + 1 >= argc) {
                return false;
            }
            target = argv[++index];
            return true;
        };
        if (argument == "--server") {
            take(server_path);
        } else if (argument == "--stub") {
            take(stub_path);
        } else if (argument == "--game") {
            take(game_path);
        } else if (argument == "--scenario") {
            take(scenario_path);
        } else if (argument == "--timeout") {
            std::string text;
            if (take(text)) {
                timeout = std::atof(text.c_str());
            }
        }
    }

    const std::string* const required[] = {&server_path, &stub_path, &game_path, &scenario_path};
    for (const std::string* path : required) {
        if (path->empty() || !file_exists(*path)) {
            std::printf("missing input: %s\n", path->c_str());
            return 2;
        }
    }

    std::printf("[+] SteamApiBridge end-to-end test\n");

    char temp_directory[MAX_PATH] = {};
    GetTempPathA(MAX_PATH, temp_directory);
    const std::string transcript =
        std::string(temp_directory) + "steambridge-e2e-" +
        std::to_string(static_cast<unsigned long>(GetCurrentProcessId())) + ".jsonl";
    DeleteFileA(transcript.c_str());

    // --- the backend, started the way a person starts it -------------------
    std::string error;
    ChildProcess backend;
    if (!backend.start(quote(server_path) + " --port 0 --scenario " + quote(scenario_path) +
                           " --transcript " + quote(transcript) + " --log-level debug",
                       error)) {
        std::printf("cannot start the backend: %s\n", error.c_str());
        return 2;
    }
    if (!backend.wait_for("listening on", timeout)) {
        std::printf("the backend never started:\n%s\n", backend.output().c_str());
        return 2;
    }

    std::string host;
    std::string port;
    {
        const auto deadline = std::chrono::steady_clock::now() +
                              std::chrono::milliseconds(static_cast<long long>(timeout * 1000.0));
        while (!extract_address(backend.output(), host, port)) {
            if (std::chrono::steady_clock::now() >= deadline) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }
    if (host.empty() || port.empty()) {
        std::printf("the backend never said where it was listening:\n%s\n",
                    backend.output().c_str());
        return 2;
    }
    std::printf("    backend %s:%s\n", host.c_str(), port.c_str());

    // --- the game, with the backend's address in its environment ------------
    _putenv_s("STEAMBRIDGE_STUB", stub_path.c_str());
    _putenv_s("STEAMBRIDGE_HOST", host.c_str());
    _putenv_s("STEAMBRIDGE_PORT", port.c_str());
    _putenv_s("STEAMBRIDGE_LOG_LEVEL", "debug");

    ChildProcess game;
    if (!game.start(quote(game_path), error)) {
        std::printf("cannot start the fake game: %s\n", error.c_str());
        return 2;
    }
    const int game_exit = game.wait_for_exit(timeout);
    if (game_exit != 0) {
        std::printf("%s\n", game.output().c_str());
    }
    const std::map<std::string, std::string> values = parse_key_values(game.output());

    // --- what the game was told -------------------------------------------
    std::printf("\n[:] answers the game received\n");
    check("the stub loaded and every export resolved", game_exit == 0,
          "exit " + std::to_string(game_exit));
    check("SteamAPI_Init was answered true (scripted)", value_of(values, "init") == "true",
          value_of(values, "init"));
    check("the game sees Steam as running", value_of(values, "is_running") == "true");
    check("the session handshake completed", value_of(values, "session_set") == "true");
    check("the install path came back as text", value_of(values, "install_path_set") == "true");
    check("the interface getters returned tokens", value_of(values, "interfaces") == "true");
    check("the steam id came from the profile", value_of(values, "steam_id") == "76561198000000001",
          value_of(values, "steam_id"));
    check("the persona name came from the profile", value_of(values, "persona") == "DebugPlayer",
          value_of(values, "persona"));
    check("the app id came from the profile", value_of(values, "app_id") == "480",
          value_of(values, "app_id"));
    check("the language came from the profile", value_of(values, "language") == "english",
          value_of(values, "language"));

    std::printf("\n[:] out parameters\n");
    check("GetStatInt32 wrote through the pointer", value_of(values, "stat.Deaths.value") == "0",
          value_of(values, "stat.Deaths.value"));
    check("a known stat reports success", value_of(values, "stat.Deaths.found") == "true");
    check("SetStatInt32 reported success", value_of(values, "stat.Deaths.write_ok") == "true");
    check("reading back after the write sees the new value",
          value_of(values, "stat.Deaths.after_write.value") == "4",
          value_of(values, "stat.Deaths.after_write.value"));
    check("the read-back also reports success",
          value_of(values, "stat.Deaths.after_write.found") == "true");
    check("an unknown stat reports failure", value_of(values, "stat.NoSuchStat.found") == "false");
    check("an unknown stat leaves the caller's variable alone",
          value_of(values, "stat.NoSuchStat.value_untouched") == "true");
    check("an achievement can be read",
          value_of(values, "achievement.ACH_BOOTED.value") == "false");
    check("unlocking an achievement reported success",
          value_of(values, "achievement.ACH_BOOTED.unlocked") == "true");
    check("it reads back as unlocked", value_of(values, "achievement.ACH_BOOTED.after") == "true");
    check("an unknown achievement reports failure",
          value_of(values, "achievement.ACH_MISSING.found") == "false");
    check("an unknown achievement leaves the caller's variable alone",
          value_of(values, "achievement.ACH_MISSING.value_untouched") == "true");
    check("the achievement list is indexed", value_of(values, "achievement.0") == "ACH_BOOTED",
          value_of(values, "achievement.0"));
    check("StoreStats reported success", value_of(values, "store_stats") == "true");

    // --- the same API as a recent SDK asks for it --------------------------
    // A game built against a recent SDK has no flat imports to hook: it asks for
    // its interfaces by version string and calls them through the vtable. These
    // came back through objects the stub built itself, so what is checked here is
    // that the vtable the game was handed has the slots where that SDK's headers
    // put them - a call landing on the wrong slot is a call answered with the
    // wrong thing, and the flat checks above cannot see it.
    std::printf("\n[:] the vtable a recent SDK would use\n");
    check("the stub hands out an object for a version string it knows",
          value_of(values, "vtable.user") == "true");
    check("and another for a second interface", value_of(values, "vtable.utils") == "true");
    check("a version string it does not know gets null, not a wrong object",
          value_of(values, "vtable.unknown") == "true");
    check("a call through the vtable is answered from the profile",
          value_of(values, "vtable.steam_id") == "76561198000000001",
          value_of(values, "vtable.steam_id"));
    check("the app id comes back through a vtable slot ten entries in",
          value_of(values, "vtable.app_id") == "480", value_of(values, "vtable.app_id"));
    check("a call nobody answers still gives the game its own default",
          value_of(values, "vtable.h_user") == "0" &&
              value_of(values, "vtable.logged_on") == "false",
          value_of(values, "vtable.h_user") + " " + value_of(values, "vtable.logged_on"));
    check("an out-parameter of a call nobody answers is left alone",
          value_of(values, "vtable.image_size_untouched") == "true" &&
              value_of(values, "vtable.image_size_answered") == "false");

    // The other direction: a call the backend does answer, asked through the
    // vtable, has to write its out-parameter back through the pointer the game
    // passed - which is the half of the marshalling the checks above do not
    // reach.
    check("the stub hands out a third interface, by its SDK's version string",
          value_of(values, "vtable.stats") == "true");
    check("an answered call writes its out-parameter back through the vtable",
          value_of(values, "vtable.achievement.found") == "true" &&
              value_of(values, "vtable.achievement.written") == "false",
          value_of(values, "vtable.achievement.found") + " " +
              value_of(values, "vtable.achievement.written"));
    check("a vtable call sees what a flat call did to the same session",
          value_of(values, "vtable.achievement.unlocked") == "true");
    check("a call nobody answers leaves a vtable call's out-parameter alone",
          value_of(values, "vtable.user_achievement.answered") == "false" &&
              value_of(values, "vtable.user_achievement.untouched") == "true");

    // --- what the backend recorded ----------------------------------------
    std::printf("\n[:] what the backend recorded\n");
    const int forwarded = std::atoi(value_of(values, "forwarded").c_str());
    std::vector<steambridge::Json> records;
    for (int attempt = 0; attempt < 100; ++attempt) {
        bool ok = false;
        records = read_transcript(transcript, ok);
        // Every call the game made has to be there by the time it exits.
        if (ok && static_cast<int>(records.size()) >= forwarded) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    // The transcript can hold more than the count the game printed: the game's
    // last calls happen after it asked for the count.
    check("the backend saw at least every call the stub had counted",
          static_cast<int>(records.size()) >= forwarded,
          std::to_string(records.size()) + " recorded, " + std::to_string(forwarded) +
              " forwarded");
    // The stub counts what it forwarded and nobody answered. It is not a fixed
    // number - it depends on how many calls the game makes that the backend has
    // no opinion about - so this is a floor and a bound, not an equality.
    const int unhandled = std::atoi(value_of(values, "unhandled").c_str());
    check("the stub counted the calls nobody answered", unhandled >= 2 && unhandled <= forwarded,
          value_of(values, "unhandled") + " of " + std::to_string(forwarded));
    check("the last thing the game did was shut down",
          !records.empty() && text_member(records.back(), "call") == "SteamAPI_Shutdown",
          records.empty() ? "no records" : text_member(records.back(), "call"));

    std::map<std::string, std::string> sources;
    for (const steambridge::Json& record : records) {
        sources[text_member(record, "call")] = text_member(record, "via");
    }
    check("identity was answered from the session state",
          value_of(sources, "SteamAPI_ISteamUser_GetSteamID") == "state");
    check("a policy call was answered from the scenario",
          value_of(sources, "SteamAPI_Init") == "scripted");
    check("RunCallbacks was forwarded but left to the stub",
          value_of(sources, "SteamAPI_RunCallbacks") == "none");

    // The same question asked twice, by the two routes a game can take: once as
    // the flat import an older SDK gives it, once through the object a recent one
    // calls. Both arrive under one name, because the backend answers calls rather
    // than callers.
    int steam_id_records = 0;
    for (const steambridge::Json& record : records) {
        if (text_member(record, "call") == "SteamAPI_ISteamUser_GetSteamID") {
            ++steam_id_records;
        }
    }
    check("a vtable call and a flat call reach the backend as the same call", steam_id_records == 2,
          std::to_string(steam_id_records) + " recorded");

    // A value class as an argument: CSteamID is eight bytes and the wire carries
    // it as the integer it is, so the steam id the game passed by value through
    // the vtable has to be readable in the record of the call.
    bool value_argument_recorded = false;
    for (const steambridge::Json& record : records) {
        if (text_member(record, "call") != "SteamAPI_ISteamUserStats_GetUserAchievement") {
            continue;
        }
        const steambridge::Json* args = record.find("args");
        if (args != nullptr && args->find("steamIDUser") != nullptr &&
            args->find("steamIDUser")->as_uint64() == 76561198000000001ull) {
            value_argument_recorded = true;
        }
    }
    check("a value class passed by value reaches the backend", value_argument_recorded);

    bool out_parameter_recorded = false;
    bool stats_write_recorded = false;
    bool every_record_names_its_session = true;
    for (const steambridge::Json& record : records) {
        const std::string call = text_member(record, "call");
        const steambridge::Json* out = record.find("out");
        if (ends_with(call, "GetStatInt32") && out != nullptr && out->find("pData") != nullptr &&
            out->find("pData")->as_int64() == 0) {
            out_parameter_recorded = true;
        }
        if (ends_with(call, "SetStatInt32")) {
            const steambridge::Json* args = record.find("args");
            if (args != nullptr && args->find("nData") != nullptr &&
                args->find("nData")->as_int64() == 4) {
                stats_write_recorded = true;
            }
        }
        if (text_member(record, "session").empty()) {
            every_record_names_its_session = false;
        }
    }
    check("out parameters appear in the transcript", out_parameter_recorded);
    check("the stats write is in the transcript", stats_write_recorded);
    check("every record names its session", every_record_names_its_session);

    // --- the command line itself ------------------------------------------
    // The backend was already started for real above; these are the two things a
    // person asks it before starting a game at all.
    std::printf("\n[:] the command line\n");
    ChildProcess list_api;
    if (list_api.start(quote(server_path) + " --list-api", error)) {
        const int exit = list_api.wait_for_exit(timeout);
        const std::string text = list_api.output();
        check("--list-api exits cleanly", exit == 0, "exit " + std::to_string(exit));
        check("--list-api names the surface", text.find("surface 'seed'") != std::string::npos);
        check("--list-api prints a call", text.find("SteamAPI_Init()") != std::string::npos);
        check("--list-api marks an out parameter", text.find("int32* pData") != std::string::npos);
    } else {
        check("--list-api can be started", false, error);
    }

    ChildProcess show_profiles;
    if (show_profiles.start(
            quote(server_path) + " --show-profiles --scenario " + quote(scenario_path), error)) {
        const int exit = show_profiles.wait_for_exit(timeout);
        const std::string text = show_profiles.output();
        check("--show-profiles exits cleanly", exit == 0, "exit " + std::to_string(exit));
        check("--show-profiles names the games",
              text.find("profile 'second_game'") != std::string::npos);
        check("--show-profiles prints a match rule",
              text.find("exe_contains") != std::string::npos);
    } else {
        check("--show-profiles can be started", false, error);
    }

    check("the backend logged the connected game",
          backend.output().find("game connected") != std::string::npos);
    check("the backend logged the game leaving",
          backend.output().find("game disconnected") != std::string::npos);

    backend.terminate();
    DeleteFileA(transcript.c_str());

    std::printf("\n");
    if (g_failures == 0) {
        std::printf("[+] all checks passed\n");
    } else {
        std::printf("[-] %d check(s) FAILED\n", g_failures);
    }
    return g_failures == 0 ? 0 : 1;
}
