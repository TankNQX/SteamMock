# Architecture

## The pieces

| Piece | Where | Responsibility |
| --- | --- | --- |
| Trampolines | `src/generated/api_stub.cpp` | One exported function per IDL entry. Marshals arguments, sends the call, reads the reply, falls back to a default. |
| Interfaces | `src/generated/api_interfaces.cpp`, `include/bridge/synth.hpp` | One object per interface version, handed out for a version string, whose slots forward into the same protocol. The marshalling is a property of the declaration's types, so a generated slot body is one line. |
| Client | `src/client.cpp` | One connection per process, the handshake, request/reply sequencing, offline behaviour, and the promise that nothing throws into the game. |
| Transport | `src/transport_tcp.cpp` | Framed loopback TCP. Behind `bridge/transport.hpp` so named pipes or shared memory can replace it. |
| Protocol | `include/bridge/frame.hpp`, `include/bridge/protocol.hpp` | The 4-byte length prefix, the version stamp, and the shape of a reply. One definition, linked by both halves. |
| Server | `src/server.cpp` | Accepts sessions, resolves each call, writes a transcript, and hands out snapshots of what it has seen. |
| Session | `src/session.cpp` | The per-game state machine: identity, language, app id, stats, achievements. |
| Scenario | `src/scenario.cpp` | Which profile a connecting game gets, and which calls a scenario overrides. |
| Generator | `src/idl.cpp`, `src/codegen_main.cpp` | Turns `gen/steam_api.idl.json` into the trampolines, the `.def` and the surface table. |

Everything above is one CMake project and one toolchain. The only thing the two halves do not share
is their entry point: the stub is a DLL that a game loads, the backend is a program a person starts.

## One call, end to end

1. The game calls an exported name. On Windows/x64 the import table resolves it to the stub, which
   is why the DLL has to be named `steam_api64.dll` and sit next to the executable.
2. The trampoline builds a JSON object of its arguments. Pointers go over the wire as opaque
   integers, so the backend can echo them but not follow them.
3. `Client::call` takes the round-trip lock, dials the backend if it is not connected, sends
   `{"type":"call","seq":N,...}`, and blocks for the matching reply. Loopback latency is
   microseconds; the timeout (`STEAMBRIDGE_TIMEOUT_MS`, default 2000) is a ceiling, not a target.
4. `Server` hands the call to `Dispatcher::answer`, which tries the scenario, then the session, and
   reports which of them spoke. It answers with `answer: "handled"` plus `ret`/`out`, or with
   `answer: "default"`.
5. On `handled`, the trampoline converts the return value to its declared type and writes any
   out-parameters back through the pointers the game passed in. On anything else - declined,
   unreachable, timed out, unparsable - it returns the default for its return type.

A call through an interface object starts at step 2 and takes the same path from there. What the
`SteamInternal_CreateInterface` trampoline handed out is a singleton whose vtable is ours, so the slot
the game calls is one of our functions: it names the call, sends what the game passed, and reads the
reply exactly as a trampoline does - which is why one scenario answers both routes and the transcript
has one vocabulary for one API.

## Threading

Calls arrive on whatever thread the game uses, so one round trip is held under a mutex. That
serialises Steam calls from different threads; for a debugging harness that is a feature rather than
a limitation, because request/reply pairing cannot get confused and the transcript stays in the order
a game made its calls.

The server is the other way round: it accepts on one thread and serves each connection on its own,
because a game per connection is the model and there are only ever a few. Everything mutable - the
sessions, the records, the counters, the transcript - sits behind one mutex, and no lock is held
while a frame is read or written. A front end that wants to draw the current state calls
`sessions()` or `records()` and gets a copy, so it can never be looking at state that is being
changed underneath it.

The stub still has no background thread. That is deliberate for now: it means no unsolicited
messages have to be handled, and it keeps the DLL's behaviour at load time free of surprises.
Callback injection is what will change it (see below).

## Nothing in DllMain

`DllMain` only calls `DisableThreadLibraryCalls`. The socket, the log file and the environment are all
read lazily, on the first API call, because doing real work under the loader lock is a good way to
deadlock a game at startup.

## Defaults and failure

Three states, and the difference matters:

* **answered** - the backend had something to say, and the game gets exactly that;
* **declined** (`answer: "default"`) - the backend knows the call and has no opinion, so the stub
  behaves as if Steam were absent;
* **unreachable** - no backend (or it went away, or it timed out): same defaults, logged once, and
  retried on the next call so a backend can be started mid-run.

The defaults are the same values a game sees without Steam: `false`, `0`, an empty string, a null
interface pointer. Nothing here invents success.

## The interfaces a recent SDK calls

A game built against a recent SDK does not import the per-interface accessors: they are inline in its
own headers. It asks `SteamInternal_CreateInterface` for a version string and then calls the object it
gets back through the vtable, so none of those calls reaches a trampoline. The stub answers them with
objects of its own.

`SteamInternal_CreateInterface` is still a generated trampoline like every other call: it asks the
backend first, so a scenario can answer it and the transcript records the request, and only if nobody
did does it hand out one of ours. That object is a singleton per version string, built from
`gen/steam_interfaces.json` - the layouts imported from SDK headers and checked against real
`steam_api.dll` images (see [development.md](development.md#interface-layouts)).

Three decisions keep that from being 2,115 hand-written functions:

* **The declarations speak the wire's types, not an SDK's.** An enum is an int, `CSteamID` is eight
  bytes, a structure returned by value is as many bytes as the file says. Nothing from an SDK is
  vendored, and the ABI is a size and a shape - so the marshalling for a slot is a property of the
  *type* a declaration used, and each kind appears once (`bridge/synth.hpp`) rather than once per
  slot. That is what lets a generated slot body be one line, with the compiler still laying out the
  vtable and the calling convention.
* **What does not differ per slot is written once.** Building the request, sending it and reading the
  reply has nothing to do with any one signature, so it is one out-of-line function
  (`src/synth.cpp`) rather than a copy inlined into every shape of call - which is what it cost as a
  header: a copy in each of the 374 shapes the file declares, and 700 KB of the DLL's code. The calls
  themselves are pooled too: the same call with the same arguments declared in six versions is one
  entry in the generated file, and the linker folds the identical thunks that reach it.
* **One name per call, whichever route reached it.** A slot is named what the newest imported SDK names
  that method with that signature - not what the game's own SDK called it, since the SDKs renamed these
  along the way: 1.47 says `SteamAPI_ISteamUserStats_GetStat` for the int32 overload where 1.51 and
  later say `SteamAPI_ISteamUserStats_GetStatInt32`. Asking the same question through the flat import
  and through the vtable therefore arrives as one call, a scenario that answers
  `SteamAPI_ISteamUserStats_GetStatInt32` answers it either way, and the transcript does not have two
  vocabularies for one API. The slots with no flat entry point - the `STEAM_PRIVATE_API` ones - keep
  the name the layouts gave them.

The objects answer nothing themselves: every slot forwards to the backend, and what the backend
declines falls back to the default the game would see with Steam absent. A structure passed by value
is the one thing the wire cannot carry, so those calls are reported and answered with a zeroed value;
`gen/steam_interfaces.json` is where a newer SDK's version strings are added, and
`steambridge_codegen` turns them into slots.

Two ways in, both ending at the same objects, and both of them needing the stub's help only because
they are the *fetch*:

* A recent SDK's accessors are inline and bounce through `SteamInternal_ContextInit`, handing it a
  blob of `{ void (*pFn)(void*); uintptr_t counter; void* value }` whose `pFn` is the game's own
  initialiser - so the stub runs it and returns the address of `value`, and the initialiser asks us
  for the interface through `SteamInternal_CreateInterface` in turn. Only the first call reaches the
  backend: the ones after it are the game's accessor checking the SDK's cache, once per use of an
  interface, which for a game that polls is thousands a second - Spacewar's own loop asked 26,228
  times in fourteen seconds, and reports one.
* An older SDK's accessors go through `ISteamClient::GetISteamUser( hUser, hPipe, pchVersion )` and
  its siblings. Those are slots like any other whose version-string argument names what they return,
  so a declined one is answered with our object for that string - the same fallback
  `SteamInternal_CreateInterface` has.

Spacewar's own `SteamworksExample.exe` is the case that exercised both: with `SteamAPI_Init` scripted
true it now collects its interfaces, polls its controller through them, and exits on what the backend
told it instead of faulting on a null pointer.

## Where the next features attach

* **A live view** is the reason `Server` is front-end free. It already exposes the two snapshots a
  panel needs - the connected (and recently disconnected) sessions with their profiles and stats,
  and the call history with what each call resolved to - so a window is a way of drawing them, not a
  restructuring. Editing a profile's stats or scripting a call from that view is the replacement for
  the Python hook the port removed.
* **Callback injection** needs two things the stub does not have yet: a reader thread, because a
  callback arrives outside any call the game made, and a registry that remembers every
  `SteamAPI_RegisterCallback(callback, id)` so the backend-pushed event can be dispatched through
  the game's own `CCallbackBase`. Both are additive; the protocol already carries a `type` field, so
  a new message type does not break the wire format.
* **Record / replay** replaces the transport: a `PassthroughTransport` forwards to a real
  `steam_api64.dll` loaded under a different name, records both directions to the same JSON the
  transcript uses, and a `ReplayTransport` serves that file back. The backend then never has to
  invent anything for the recorded session.
* **Struct and buffer parameters** are new IDL kinds (`kind: "buffer"`, with a length expression)
  plus generator support. The type table in `src/idl.cpp` is the only place that has to grow.

## Known limitations

* A returned string is copied into a per-thread buffer that stays valid until the next call on that
  thread, because the reply it came from dies with the call. Games normally copy immediately; one
  that stores the pointer would read the next call's text.
* Interface pointers are tokens, except where they are objects of ours. `SteamAPI_ISteamUser()`
  returns whatever the scenario says, and the backend sees that same number back on every later call;
  it cannot dereference it. A version string the stub has layouts for is the other case: the game
  gets a real object whose vtable is ours, and its calls arrive as calls like any other.
* Scalars, C strings, pointers and out-parameters travel; a structure passed or returned *by value*
  does not. The call is reported, with the structure's bytes going over as null, and the caller gets
  a zeroed one back - the same answer a game gets with Steam absent, rather than an invented value.
* The ABI those objects present was checked once, by compiling a client from Valve's own headers and
  watching where its calls landed: the compiler lays the calls out as that SDK says to, so the slots
  it reaches are the slots a shipped game reaches. That is what settled the overload order above, and
  the layouts keep it. What nobody can check any more is a game built against an SDK whose headers are
  gone - the generation Spacewar was built against is one of those, and its entries are the file's own
  record.
* A game can ask for an interface version the stub has no layout for - anything newer than the SDKs
  the file was imported from. It gets null, exactly as it would from a stub that did not answer at
  all, and the transcript says which string it asked for. Importing another generation is one command
  (see [development.md](development.md#interface-layouts)).
* Floating point goes over the wire as JSON numbers. The writer is exact, and a reader gets the same
  double back; the bridge's own hand-rolled parser is the loose end, keeping integers exact and
  accumulating fractions digit by digit, so a `float` parameter is rounded through a `double`.
* One connection per process, serialised calls, no multiplexing of several games onto one socket -
  a game per connection is simpler and matches how they run.
* A game that disconnects keeps its session, so the run summary and the transcript stay about the
  whole run. `SessionSnapshot::connected` is what tells a view which rows are still live.
* Flat entry points only, and that is a real boundary rather than a detail. A game built against a
  recent SDK gets its interfaces by asking `SteamInternal_CreateInterface` for a version string and
  then calling the object it hands back through its vtable - the per-interface accessors are inline
  in the game, not imported, so none of those calls reach a trampoline. Spacewar's own
  `SteamworksExample.exe` is one of these: with `SteamAPI_Init` scripted true the transcript ends at
  `SteamInternal_ContextInit` declined, and the game's next call through the null pointer faults
  (`SteamAPI_WriteMiniDump` reports `0xC0000005` on the way out). Covering that case means
  synthesizing the interfaces - an object per interface version whose vtable slots forward into this
  same IDL and protocol - which needs the slot order per version, from a real SDK header or out of a
  real `steam_api.dll`.
