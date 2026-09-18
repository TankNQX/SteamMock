# SteamApiBridge

A local debugging harness for the Steamworks flat API.

The stub DLL is shaped like `steam_api64.dll`: it exports the same flat API names, and instead of
talking to Valve it forwards every call it sees over loopback TCP to a Python backend that decides
the answer. That way a game's Steam integration can be developed and debugged **locally**, with
scripted and inspectable responses, and only run against real Steam once the logic is settled.

```
   game.exe                        stub DLL (this repo)              python backend (this repo)
   ────────                        ────────────────────              ─────────────────────────
   SteamAPI_Init()  ──import──▶   generated trampoline
   SteamAPI_ISteamUser_GetSteamID()      │
                                         │  4-byte length + JSON, 127.0.0.1:50990
                                         ▼
                                  BridgeClient ─────────────────▶  Session + Dispatcher
                                  (one connection per game)              │
                                         ▲                               │  scenario / state / hook
                                         │        {"seq":7,"answer":"handled","ret":…,"out":{…}}
                                         └───────────────────────────────┘
```

The backend is where the interesting decisions live: a scenario file decides what a given game is
told, and the session keeps per-game state (identity, stats, achievements) so several games can run
side by side, each with its own profile.

## Quick start

```sh
# 1. Build the stub (produces build/Release/steam_api64.dll)
cmake -S . -B build -A x64            # or: -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release

# 2. Start the backend. It prints the port it bound.
python -m steambridge --scenario python/steambridge/scenarios/example.json --transcript run.jsonl

# 3. Run a game with the stub beside it (or point SteamApiBridge's own test game at it)
python python/tests/test_e2e.py --stub build/Release/steam_api64.dll \
                                --game build/Release/fake_game.exe \
                                --scenario python/steambridge/scenarios/example.json
```

To use it with a real game: put `steam_api64.dll` (this build) next to the game's executable, where
it shadows the real one, and point the backend at a scenario for that game. `STEAMBRIDGE_PORT`,
`STEAMBRIDGE_HOST`, `STEAMBRIDGE_OFF`, `STEAMBRIDGE_TIMEOUT_MS`, `STEAMBRIDGE_LOG` and
`STEAMBRIDGE_LOG_LEVEL` are read from the environment by the stub, so the game needs no changes and
no arguments.

If no backend is listening, the stub logs it once and every call falls back to the value a game
would see with Steam not running - a game still boots, and a backend started later is picked up on
the next call.

## What the backend answers

Resolution order, first one that speaks wins (the transcript records which):

1. a `scripted` entry for that call in the game's profile - the policy calls, `SteamAPI_Init` and
   friends, where a scenario has to say what it wants;
2. a hook, if you passed `on_call=...` to the server - arbitrary Python;
3. the session state machine - identity, language, app id, stats, achievements;
4. nobody: the call is reported **unanswered**, and the stub uses its own default.

That last case is the point. An unanswered call behaves exactly as it would with Steam absent, so a
game cannot be handed a success nobody asked for, and the transcript always says what happened and
why.

## Layout

| Path | What lives there |
| --- | --- |
| `gen/steam_api.idl.json` | The API surface the stub exports. **The one file to edit to add a call.** |
| `gen/generate.py` | Turns the IDL into the trampolines, the `.def` and the Python surface. |
| `src/generated/` | Generated and committed - the build needs no Python. `--check` fails if stale. |
| `include/bridge/`, `src/` | Protocol, JSON, transport, client, DLL entry point. |
| `python/steambridge/` | The backend: protocol, sessions, scenarios, server, CLI. |
| `python/steambridge/scenarios/` | Example scenario: two games, two profiles. |
| `tests/` | C++ unit tests and `fake_game`, a stand-in game that loads the stub. |
| `docs/` | Architecture and protocol notes. |

## Adding a call

```sh
# edit gen/steam_api.idl.json, then:
python gen/generate.py
```

The generator validates the IDL, writes `src/generated/api_stub.cpp` and
`src/generated/steam_api_exports.def`, and refreshes `python/steambridge/surface.json`. A test case
(`generated_files_are_current`) fails the build if the IDL and the generated files have drifted
apart. `python -m steambridge --list-api` prints the surface as the backend sees it.

## Tests

```sh
ctest --test-dir build -C Release --output-on-failure
```

| Test | Covers |
| --- | --- |
| `protocol` | The C++ JSON subset and the frame header: integer fidelity, escapes, strict rejection of malformed input. |
| `python_protocol` | The Python mirror: framing, replies, the session state machine, scenarios, match rules. |
| `generated_files_are_current` | The generated files match `gen/steam_api.idl.json`. |
| `python_end_to_end` | The real thing: a game loads the real DLL, the real backend answers, both sides are checked. |

The end-to-end test is Windows only, because it loads the stub as a DLL.

## Status

Working now: the loopback bridge, the session state machine, scenarios with per-game profiles,
out-parameters, the transcript, offline fallback, and the generator.

Next, roughly in order of value:

1. **Callback injection.** Games expect `RunCallbacks` to deliver `UserStatsReceived`, and worse.
   The stub records registrations today; the backend should be able to push an event, which needs
   the SDK's callback payload structs. This also means a reader thread in the stub.
2. **The full export surface**, generated from your `steam_api_flat.h` - the seed here is 28 calls
   with hand-written signatures, and every one of them should be reconciled against the real header
   before being relied on.
3. **Record / replay**: a passthrough mode where the stub forwards to a real `steam_api64.dll`,
   records both directions, and the backend replays that recording later. This is the feature that
   makes "debug locally, then test against Steam" pay for itself, and the `Transport` interface is
   already the seam for it.
4. **Struct and buffer parameters.** The type table covers scalars, strings and out-parameters
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
