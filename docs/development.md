# Development

How the source is laid out, what the tests cover, and what CI checks. For what the tool *does*, see
the [README](../README.md); for how the two halves talk, see [protocol.md](protocol.md) and
[architecture.md](architecture.md).

## Layout

| Path | What lives there |
| --- | --- |
| `gen/steam_api.idl.json` | The flat API surface the stub exports. **The one file to edit to add a call.** |
| `gen/steam_interfaces.json` | The interface layouts the stub hands out: names, version strings, slot order and argument kinds. Hand-maintained, and validated by the generator - see [Interface layouts](#interface-layouts). |
| `src/idl.cpp`, `src/codegen_main.cpp` | `steambridge_codegen` turns the IDL into the trampolines, the `.def` and the surface table. |
| `src/generated/` | Generated and committed - the build needs nothing to regenerate them. `--check` fails if stale. |
| `include/bridge/`, `src/` | Protocol, JSON, transport, client, the DLL entry point. |
| `src/session.cpp`, `src/scenario.cpp` | The backend's decisions: per-game state, and the scenario that overrides it. |
| `src/server.cpp`, `src/backend_main.cpp` | The loopback server, and the console front end for it. |
| `src/gui_main.cpp` | The live view. Optional, behind `STEAMBRIDGE_BUILD_GUI`. |
| `scenarios/` | Two scenarios: `example.json` for `fake_game` and the tests, `spacewar.json` for the walkthrough in the [README](../README.md). |
| `tests/` | C++ unit tests, `fake_game`, and the end-to-end test. |
| `docs/` | These notes. |

## Command lines and environment

The stub takes no arguments, which is the point of it, so everything a game or a backend is told comes
from where it runs and what is in its environment:

| Variable | What it does |
| --- | --- |
| `STEAMBRIDGE_HOST`, `STEAMBRIDGE_PORT` | Where the backend is. Default `127.0.0.1:50990`. |
| `STEAMBRIDGE_OFF` | Set to `1` to bypass the bridge: every call answers as if Steam is absent. |
| `STEAMBRIDGE_TIMEOUT_MS` | How long a call waits for the backend. Default `2000`. |
| `STEAMBRIDGE_LOG` | A file to append the stub's own log to. |
| `STEAMBRIDGE_LOG_LEVEL` | `error`, `warning`, `info` (default) or `debug`. |

If no backend is listening, the stub says so once and every call takes its Steam-absent value. A game
still boots, and a backend started later is picked up on the next call.

Both front ends take the same options - the console backend and the live view:

| Option | What it does |
| --- | --- |
| `--host`, `--port` | Where to listen. `--port 0` picks a free one; the default is the 50990 above. |
| `--scenario FILE` | What each game is told (default `scenarios/example.json`). |
| `--transcript FILE` | Append every call, as JSON lines, to this file. |
| `--log-level LEVEL` | `error`, `warning`, `info` or `debug`. |
| `--list-api` | Print the calls the stub exports, then exit. |
| `--show-profiles` | Print the scenario's games and match rules, then exit. |

The live view adds `--start`, which serves as soon as the window opens instead of waiting for a click,
and `-DSTEAMBRIDGE_STUB_NAME` decides what the built DLL is called - `steam_api` for a 32-bit game,
which is what the README's walkthrough builds.

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

`fake_game` is also a smoke test you can run by hand, without a game of your own - it loads the stub
the way a game's import table would and prints what it got:

```bat
set STEAMBRIDGE_STUB=build\Release\steam_api.dll
build\Release\fake_game.exe
```

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

## Interface layouts

A game built against a recent SDK does not import the per-interface accessors: they are inline in the
game, so it asks `SteamInternal_CreateInterface` for a version string and then calls the object it
gets back through its vtable. None of those calls reach a flat trampoline, which is why the stub also
hands out objects of its own, described by `gen/steam_interfaces.json`: one entry per interface with
its version string and its slots, in order.

That file is **hand-maintained data**. Adding a version string means adding its slots there and
running `steambridge_codegen`, which refuses to generate anything from a file whose kinds, names or
sizes do not add up - that validation is the only thing standing between a hand edit and a vtable a
game reads the wrong way. The entries were laid out by importing SDK headers once (five generations at
a time, oldest first, one version string per entry), and what that established is what a hand edit has
to keep to.

The SDKs are never vendored here, and are not needed to build. Unreal Engine bundles five of them,
under `Engine/Source/ThirdParty/Steamworks/Steamv1*`: headers for 1.46 through 1.57 and a 32-bit
`steam_api.dll` for 1.51, 1.53 and 1.57. The generation Spacewar was built against - `SteamUser019`,
`SteamFriends015`, `STEAMUGC_INTERFACE_VERSION010` and the rest - came from an SDK that is no longer on
this machine, so its entries are the file's own record of it and nothing can re-derive them.

Two things a header alone does not say:

* `STEAM_PRIVATE_API(...)` expands to `protected: ... public:` for a game, so those methods are
  protected virtuals that *own a slot* while having no flat entry point at all. Placing flat names by
  signature rather than by position is what keeps the slots after them in step.
* Methods under `#ifdef _PS3` do not exist on Windows and must not be given a slot.

**The class header is the authority on slot order. Valve's DLL is a witness, not a source.** The two
disagree, and what settles the direction is which of them a game follows: Valve's headers declare the
int32 overload of `ISteamUserStats::GetStat` first, every shipping `steam_api.dll` forwards its *flat*
wrapper for that name to the second slot, and a client compiled from the headers reaches the int32
overload exactly where the header declares it. Those wrappers describe the objects the DLL's own flat
accessors hand out - a layer the bridge never touches, because the flat accessors it exports are
answered by the backend rather than by an object of Valve's. A wrapper that disagrees with the header
was therefore reported rather than followed; for 1.51, 827 agreed, 24 forwarded elsewhere, 1 could not
be read, and 10 names were not in the DLL at all.

Two generations that declare the same version string were folded together, newest winning, and the
difference is written down here because a version string is meant to name one ABI:

* Where the loser had slots the winner does not, they were appended - a version string grows by having
  methods added at the end, so `STEAMAPPS_INTERFACE_VERSION008` is the 26 slots Spacewar's generation
  declared plus the four that came later. A vtable is an array; the longer one serves both.
* A genuine disagreement - the same slot, two signatures - was a choice, made in the winner's favour.
  `STEAMINVENTORY_INTERFACE_V003` is inverted in 1.46 and 1.47 relative to 1.51 and later, and
  `SteamUser021`'s third slot is `InitiateGameConnection` up to 1.51 and
  `InitiateGameConnection_DEPRECATED` after, under the same version string.
* A slot's *name* is the one the newest generation gave that method and signature, not its own SDK's:
  1.47 calls the int32 overload of `ISteamUserStats::GetStat` `SteamAPI_ISteamUserStats_GetStat` where
  every SDK from 1.51 on calls it `SteamAPI_ISteamUserStats_GetStatInt32`, and the IDL the backend
  answers from uses the newer name. One name per call means a scenario does not have to know which SDK
  built the game.

Three things worth knowing about the newer headers, if a version is ever added by hand rather than
taken from an import:

* From 1.51 on the flat header names the instance pointer `ISteamXxx *self` where it used to be
  `intptr_t instancePtr`, and it spells an overload's flat name with the argument's type
  (`GetStatInt32`) where it used to be a digit. Where even that is not enough to derive the name, the
  class header states it: `STEAM_FLAT_NAME( GetQueryFirstUGCKeyValueTag )`.
* It also lists methods that own no slot, because the class header implements them in terms of the
  virtuals and the binding generator still exports them: `ISteamNetworkingUtils::SetGlobalConfigValueInt32`
  is `SetConfigValue` with two arguments filled in. They are reported and left out.
* `isteamcontroller.h` is deprecated from 1.51 on and keeps using `EControllerSourceMode`, which the
  SDK no longer declares anywhere - it is `EInputSourceMode` in `isteaminput.h`. Since every enum in
  these headers is an int, an undeclared `E...` member is sized as one, and said so.

### What the generator writes from it

`steambridge_codegen` reads `gen/steam_interfaces.json` as well as the IDL, and writes
`src/generated/api_interfaces.cpp` (checked for staleness like the rest):

* one class per version string, whose virtuals mirror the interface's slots in order, so the vtable
  is the compiler's - and with it the calling convention, which is the part that has to be exactly
  right;
* for each slot, one line: the signature from the kinds in the file, the index of its descriptor, and
  its argument names. The names go over the wire as data, and the marshalling is in
  `include/bridge/synth.hpp` - a `Kind<>` trait per wire kind plus one variadic body - so adding a
  version adds lines to a table rather than a body to write;
* the value classes and structures the declarations name, with `static_assert`s against the sizes the
  file carries. A size that is wrong in the file is then a build error, not a call the game reads the
  wrong way;
* a version-string lookup, which is what `SteamInternal_CreateInterface` falls back to when nobody
  answered it. That call carries `"fallback": "interface"` in the IDL - "ask the backend, and if it
  has nothing to say, hand out the object of ours for the version string" - and so does
  `SteamInternal_ContextInit` with `"fallback": "context"`, which is the SDK's lazy accessor: the stub
  runs the game's own initialiser out of the blob it was passed, and asks the backend only the first
  time. Those two fields are the only places a generated trampoline does anything but forward.
* the same fallback inside a generated slot, for the `ISteamClient::GetISteamUser( hUser, hPipe,
  pchVersion )` family: a slot that returns an interface pointer *and* takes a version string gets the
  object of ours for it when the backend declines.

Adding a version is therefore an edit to that file and one command. The slot's `call` is the name the
newest SDK gave that method, so the backend answers a call the same way whichever route it arrived by.

### What the file does not cover

* `ISteamNetworkingFakeUDPPort` has no version string at all; it arrives as the return value of
  `ISteamNetworkingSockets::CreateFakeUDPPort`, so nothing can ask for it by name and it is left out.
  A slot that returns one of those hands the game a token the backend can see and echo, not an object
  of ours.
* A game asking for a version string no imported generation declares gets null, exactly as it would
  from a stub that did not answer at all. That is the boundary the data draws: it is one command to
  move it (see above), not a limitation of the mechanism.

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
