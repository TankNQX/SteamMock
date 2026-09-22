# Development

How the source is laid out, what the tests cover, and what CI checks. For what the tool *does*, see
the [README](../README.md); for how the two halves talk, see [protocol.md](protocol.md) and
[architecture.md](architecture.md).

## Layout

| Path | What lives there |
| --- | --- |
| `gen/steam_api.idl.json` | The API surface the stub exports. **The one file to edit to add a call.** |
| `src/idl.cpp`, `src/codegen_main.cpp` | `steambridge_codegen` turns the IDL into the trampolines, the `.def` and the surface table. |
| `src/generated/` | Generated and committed - the build needs nothing to regenerate them. `--check` fails if stale. |
| `include/bridge/`, `src/` | Protocol, JSON, transport, client, the DLL entry point. |
| `src/session.cpp`, `src/scenario.cpp` | The backend's decisions: per-game state, and the scenario that overrides it. |
| `src/server.cpp`, `src/backend_main.cpp` | The loopback server, and the console front end for it. |
| `src/gui_main.cpp` | The live view. Optional, behind `STEAMBRIDGE_BUILD_GUI`. |
| `scenarios/` | Example scenario: two games, two profiles. |
| `tests/` | C++ unit tests, `fake_game`, and the end-to-end test. |
| `docs/` | These notes. |

## Tests

```sh
ctest --test-dir build -C Release --output-on-failure
```

| Test | Covers |
| --- | --- |
| `protocol` | The C++ JSON subset and the frame header: integer fidelity, escapes, strict rejection of malformed input. |
| `backend` | Replies, the session state machine, scenarios and match rules - and that every call the state machine answers is one the IDL actually exports. |
| `server` | The server in process: a real port, a real connection through the stub's own transport, and the snapshots and summary the live view draws. |
| `generated_files_are_current` | The generated files match `gen/steam_api.idl.json`. |
| `end_to_end` | The real thing: the backend started as a subprocess, a game loading the real DLL, both sides checked, and the command line itself. |

## Adding a call

```sh
# edit gen/steam_api.idl.json, then:
build/Release/steambridge_codegen
```

The generator validates the IDL and rewrites `src/generated/api_stub.cpp`,
`src/generated/steam_api_exports.def` and `src/generated/api_surface.cpp`. The
`generated_files_are_current` test fails the build if the IDL and the generated files have drifted
apart, so there is no way to forget this step. `steambridge --list-api` prints the surface as the
backend sees it.

Most of the seed surface's signatures are hand-written rather than lifted from a real header - the
GameServer and `SteamInternal_*` entries are the exception, taken from the SDK's own declarations -
so reconcile the rest against your own `steam_api_flat.h` (or the export table of a real
`steam_api64.dll`) before relying on them.

A game notices a missing export at load time, not at call time: Windows resolves the whole import
table first, so one name the surface does not cover stops the game before `DllMain`. Compare the
game's `steam_api.dll` imports against `steambridge --list-api` when a game will not start.

## What CI checks

Everything below runs on a push, and all of it must be green.

* **MSVC**, for x64 and Win32, in Debug and Release, plus two jobs with warnings as errors. This is
  the compiler the harness ships with.
* **clang-cl**, on the same ABI, also x64 and Win32. Clang is there for the warnings MSVC has no
  equivalent of - a constant nobody uses, a name that shadows a member - which is how two real bugs
  were caught rather than shipped.
* **clang-format** (`tools/check-format.ps1`), so the hand-written files stay formatted as
  `.clang-format` describes. The files under `src/generated/` are **out of scope on purpose**: a
  test byte-compares them against what `steambridge_codegen` writes, so an editor's format-on-save
  would break the build. Leave that setting off for those paths, or run the checker to find out.
* **The live view** (`gui / msvc / x64`), built with `-DSTEAMBRIDGE_BUILD_GUI=ON` and warnings as
  errors. It is the only job that checks out the submodules, so the rest stay fast and need no
  third-party code.

## Tools

| Path | What it does |
| --- | --- |
| `tools/check-format.ps1` | The clang-format gate CI runs. Also checks files that are not committed yet. |
| `.clang-format`, `.clang-tidy` | The style the transcript was written in, and the checks that are kept clean. |
