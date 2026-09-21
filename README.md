# SteamApiBridge

A local debugging harness for the Steamworks flat API.

The stub DLL is shaped like `steam_api64.dll`: it exports the same flat API names, and instead of
talking to Valve it forwards every call it sees over loopback TCP to a backend that decides the
answer. That way a game's Steam integration can be developed and debugged **locally**, with
scripted and inspectable responses, and only run against real Steam once the logic is settled.

Both halves are C++, built by one CMake project, and share one implementation of the wire format.

```
   game.exe                        stub DLL (this repo)              backend (this repo)
   ────────                        ────────────────────              ───────────────────
   SteamAPI_Init()  ──import──▶   generated trampoline
   SteamAPI_ISteamUser_GetSteamID()      │
                                         │  4-byte length + JSON, 127.0.0.1:50990
                                         ▼
                                  BridgeClient ─────────────────▶  Server + Session
                                  (one connection per game)              │
                                         ▲                               │  scenario / state
                                         │        {"seq":7,"answer":"handled","ret":…,"out":{…}}
                                         └───────────────────────────────┘
```

The backend is where the interesting decisions live: a scenario file decides what a given game is
told, and the session keeps per-game state (identity, stats, achievements) so several games can run
side by side, each with its own profile.

## Quick start

```sh
# 1. Build everything (produces build/steam_api64.dll and build/steambridge)
cmake -S . -B build -A x64            # or: -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release

# 2. Start the backend. It prints the address it bound.
build/Release/steambridge --scenario scenarios/example.json --transcript run.jsonl

# 3. Run a game with the stub beside it (or point the harness's own test game at it)
build/Release/fake_game.exe          # with STEAMBRIDGE_STUB set to the built DLL
```

To use it with a real game: put `steam_api64.dll` (this build) next to the game's executable, where
it shadows the real one, and point the backend at a scenario for that game. `STEAMBRIDGE_PORT`,
`STEAMBRIDGE_HOST`, `STEAMBRIDGE_OFF`, `STEAMBRIDGE_TIMEOUT_MS`, `STEAMBRIDGE_LOG` and
`STEAMBRIDGE_LOG_LEVEL` are read from the environment by the stub, so the game needs no changes and
no arguments.

If no backend is listening, the stub logs it once and every call falls back to the value a game
would see with Steam not running - a game still boots, and a backend started later is picked up on
the next call.

The backend's command line is the same set of choices:

| Option | What it does |
| --- | --- |
| `--host`, `--port` | Where to listen. `--port 0` picks a free one. |
| `--scenario FILE` | What each game is told (default `scenarios/example.json`). |
| `--transcript FILE` | Append every call, as JSON lines, to this file. |
| `--log-level LEVEL` | `error`, `warning`, `info` or `debug`. |
| `--list-api` | Print the calls the stub exports, then exit. |
| `--show-profiles` | Print the scenario's games and match rules, then exit. |

## What the backend answers

Resolution order, first one that speaks wins (the transcript records which):

1. a `scripted` entry for that call in the game's profile - the policy calls, `SteamAPI_Init` and
   friends, where a scenario has to say what it wants;
2. the session state machine - identity, language, app id, stats, achievements;
3. nobody: the call is reported **unanswered**, and the stub uses its own default.

That last case is the point. An unanswered call behaves exactly as it would with Steam absent, so a
game cannot be handed a success nobody asked for, and the transcript always says what happened and
why.

There used to be a third rung between those two - an arbitrary Python callable passed to the
server - and it is the one thing that did not survive the move to C++. A scenario that says what it
means is easier to hand to someone else than a lambda buried in a script, and the same structures
are what a live view would edit while a game is running.

## Layout

| Path | What lives there |
| --- | --- |
| `gen/steam_api.idl.json` | The API surface the stub exports. **The one file to edit to add a call.** |
| `src/idl.cpp`, `src/codegen_main.cpp` | `steambridge_codegen` turns the IDL into the trampolines, the `.def` and the surface table. |
| `src/generated/` | Generated and committed - the build needs nothing to regenerate them. `--check` fails if stale. |
| `include/bridge/`, `src/` | Protocol, JSON, transport, client, the DLL entry point. |
| `src/session.cpp`, `src/scenario.cpp` | The backend's decisions: per-game state, and the scenario that overrides it. |
| `src/server.cpp`, `src/backend_main.cpp` | The loopback server, and the console front end for it. |
| `scenarios/` | Example scenario: two games, two profiles. |
| `tests/` | C++ unit tests, `fake_game`, and the end-to-end test. |
| `docs/` | Architecture and protocol notes. |

## Adding a call

```sh
# edit gen/steam_api.idl.json, then:
build/Release/steambridge_codegen
```

The generator validates the IDL and rewrites `src/generated/api_stub.cpp`,
`src/generated/steam_api_exports.def` and `src/generated/api_surface.cpp`. A test
(`generated_files_are_current`) fails the build if the IDL and the generated files have drifted
apart. `steambridge --list-api` prints the surface as the backend sees it.

## Tests

```sh
ctest --test-dir build -C Release --output-on-failure
```

| Test | Covers |
| --- | --- |
| `protocol` | The C++ JSON subset and the frame header: integer fidelity, escapes, strict rejection of malformed input. |
| `backend` | Replies, the session state machine, scenarios and match rules - and that every call the state machine answers is one the IDL actually exports. |
| `generated_files_are_current` | The generated files match `gen/steam_api.idl.json`. |
| `end_to_end` | The real thing: the backend started as a subprocess, a game loading the real DLL, both sides checked, and the command line itself. |

The end-to-end test is Windows only, because it loads the stub as a DLL in a separate process.

## Status

Working now: the loopback bridge, the server, the session state machine, scenarios with per-game
profiles, out-parameters, the transcript, offline fallback, and the generator.

Next, roughly in order of value:

1. **A live view.** `Server` in `include/bridge/server.hpp` is front-end free and hands out
   snapshots of its sessions and its call history, so a window on top of it can show what a game is
   asking, edit a profile's stats and achievements while it runs, and script a call on the spot -
   the "arbitrary logic" the Python hook used to provide, without a rebuild.
2. **Callback injection.** Games expect `RunCallbacks` to deliver `UserStatsReceived`, and worse.
   The stub records registrations today; the backend should be able to push an event, which needs
   the SDK's callback payload structs. This also means a reader thread in the stub.
3. **The full export surface**, generated from your `steam_api_flat.h` - the seed here is 28 calls
   with hand-written signatures, and every one of them should be reconciled against the real header
   before being relied on.
4. **Record / replay**: a passthrough mode where the stub forwards to a real `steam_api64.dll`,
   records both directions, and the backend replays that recording later. This is the feature that
   makes "debug locally, then test against Steam" pay for itself, and the `Transport` interface is
   already the seam for it.
5. **Struct and buffer parameters.** The type table covers scalars, strings and out-parameters
   today; fixed-size structs and `char*` buffers need dedicated kinds.

## What this is not

* **Not a runtime for games you do not own.** It forwards the API surface and reports what a
  scenario tells it to report. Nothing here answers an ownership, entitlement or licensing question,
  and the defaults are the "Steam is not running" values.
* **Not shippable.** A substitute `steam_api64.dll` is a development tool; the Steamworks agreement
  does not allow redistributing one with a game. Keep it in your dev and test runs.
* **Not a Steam emulator.** It has no idea what a Steam ID means, it does not talk to Valve, and it
  stops at the API boundary: anything that needs the real service - matchmaking, friends, cloud -
  has to be scripted per call.

## License

MIT. See `LICENSE`.
