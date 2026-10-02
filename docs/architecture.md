# Architecture

## The pieces

| Piece | Where | Responsibility |
| --- | --- | --- |
| Trampolines | `src/generated/api_stub.cpp` | One exported function per IDL entry. Marshals arguments, sends the call, reads the reply, falls back to a default. |
| Interfaces | `src/generated/api_interfaces.cpp`, `include/bridge/synth.hpp` | One object per interface version, handed out for a version string. Its slots forward into the same protocol as the trampolines, so a generated slot body is one line. |
| Client | `src/client.cpp` | One connection per process, the handshake, request and reply sequencing, offline behaviour, and the promise that nothing throws into the game. |
| Transport | `src/transport_tcp.cpp` | Framed loopback TCP, behind `bridge/transport.hpp` so another implementation can replace it. See [what replacing it would take](#the-socket-layer-and-what-replacing-it-would-take). |
| Protocol | `include/bridge/frame.hpp`, `include/bridge/protocol.hpp` | The 4-byte length prefix, the version stamp, and the shape of a reply. Both halves link this one definition. |
| Server | `src/server.cpp` | Accepts sessions, resolves each call, writes a transcript, and hands out snapshots of what it has seen. |
| Session | `src/session.cpp` | The per-game state machine: identity, language, app id, stats, achievements. |
| Scenario | `src/scenario.cpp` | Which profile a connecting game gets, and which calls a scenario overrides. |
| The world | `src/lobby.cpp`, `src/leaderboard.cpp`, `src/inventory.cpp` | The state no single session can answer: the lobbies and their members, the leaderboards the run's players have posted scores to, and the item catalogue beside what each player holds. Each outlives the sessions that made it. |
| The state file | `include/bridge/store.hpp`, `src/store.cpp` | The one thing here that outlives the *process*: what games wrote, kept in a SQLite database a run was pointed at with `--state`. Absent unless someone asks for it, in which case it is what a profile is merged from when a game connects and what a write goes to as it is made. |
| Generator | `src/idl.cpp`, `src/codegen_main.cpp` | `steammock_codegen` turns `gen/steam_api_surface.json` into the trampolines, the `.def` and the table of exported calls, and holds the mock's own decisions, by call name. |
| Interface layouts | `src/interfaces.cpp`, `tools/steamworks_sdk_import.py` | Turns an SDK's headers into `gen/steam_interfaces.json` and then into `src/generated/api_interfaces.cpp`: the payloads a callback delivers, and the calls a game can make through an interface object. This is also where the file decides that a call's `void *` is one structure. See [structures and buffers](#structures-and-buffers-the-game-owns). |

The two halves share one CMake project and one toolchain. They differ in their entry point: the stub
is a DLL a game loads, the backend is a program a person starts.

## One call, end to end

1. The game calls an exported name. On Windows x64 the import table resolves it to the stub, which
   is why the DLL has to be named `steam_api64.dll` and sit next to the executable.
2. The trampoline builds a JSON object of its arguments. Pointers travel as opaque integers, so the
   backend can echo them but not follow them.
3. `Client::call` takes the round-trip lock, dials the backend if it is not connected, sends
   `{"type":"call","seq":N,...}`, and blocks for the matching reply. Loopback latency is
   microseconds, and the timeout is a ceiling rather than a target. It is `STEAMMOCK_TIMEOUT_MS`,
   2000 ms by default.
4. `Server` hands the call to `Dispatcher::answer`. That tries the scenario first, then the session,
   and reports which of the two spoke. It answers with `answer: "handled"` plus `ret` and `out`, or
   with `answer: "default"`.
5. On `handled`, the trampoline converts the return value to its declared type and writes the
   out-parameters back through the pointers the game passed in. On anything else, whether that is
   declined, unreachable, timed out or unparsable, it returns the default for its own return type.

A call through an interface object starts at step 2 and follows the same path. The trampoline for
`SteamInternal_CreateInterface` handed the game a singleton whose vtable is ours, so the slot the game
calls is one of our functions. That function names the call, sends what the game passed, and reads the
reply the way a trampoline does. One scenario therefore answers both routes, and the transcript keeps
one vocabulary for one API.

## Threading

Calls arrive on whatever thread the game uses, so the client holds one mutex across a round trip. That
serialises Steam calls from different threads. For a tool that watches a game this is a feature:
request and reply pairing cannot get confused, and the transcript stays in the order the game made its
calls. [The socket layer note](#the-socket-layer-and-what-replacing-it-would-take) works out what that
costs a game whose backend has stopped answering.

The server works the other way round. It accepts on one thread and serves each connection on a thread
of its own, because one game per connection is the model and there are only ever a few. Everything
mutable, meaning the sessions, the records, the counters and the transcript, sits behind one mutex,
and the code holds no lock while it reads or writes a frame. A front end that wants to draw the
current state calls `sessions()` or `records()` and gets a copy, so it never looks at state that is
changing underneath it. The history it gets is a window of the last 20,000 calls,
`Server::kMaxRecords`, because a game that polls can make thousands a second. The transcript keeps
every call, and a reader's cursor is an absolute position in the run rather than an offset into the
window, which is what `records_begin()` and `records_since()` mean.

The stub has no background thread, and that is deliberate. No unsolicited message has to be handled,
and the DLL does nothing surprising at load time. Callbacks arrive inside the game's own
`RunCallbacks`, because the backend sends them on the reply to a call the game already made. A thread
that could reach a game which has stopped calling is the same thread whose cost the socket layer note
works out. `DllMain` calls `DisableThreadLibraryCalls` and nothing else: the socket, the log file and
the environment are all read lazily, on the first API call, because doing real work under the loader
lock can deadlock a game at startup.

## The socket layer, and what replacing it would take

Four things speak sockets, and only one of them sits behind an interface:

* `bridge/transport.hpp` is what the client uses: `connect`, `close`, `is_connected`, `exchange` and
  `set_timeout_ms`. It is deliberately narrow, so a different implementation is a different object to
  construct rather than a change to the call path.
* `src/transport_tcp.cpp` is the one implementation. It uses one blocking socket, waits for a
  non-blocking connect against a `select()` deadline, and hangs the socket up on any framing or send
  failure. The next call then dials again rather than exchanging on a stream that is out of step.
* `src/socket_io.hpp` holds the winsock calls both ends share: the process-wide startup, "a handle
  lives in an integer", send-everything-or-nothing, and the half-close that wakes a thread parked in
  `recv`.
* `src/server.cpp` holds the accept loop and each connection's reader. Neither sits behind the
  interface. Both use synchronous winsock, and the reader waits in `select()` slices so it can look at
  `_stopping` between them.

`Client::call` holds one lock across the whole round trip, so a game with a stalled backend serialises
every other call it makes. The timeout bounds the wait, not the lock. The connect uses the timeout the
client asked for rather than the TCP stack's own SYN timer, which measured 21,038 ms here against a
host that answers nothing. Any failure hangs the socket up, so a caller queued behind a dead backend
waits one timeout, 2 s by default, instead of twenty-one seconds. Releasing the lock while waiting is
not a small change. There is one socket and one reply per request, in order, so two threads waiting at
once would race for the same frame. Whichever read first would take the other's reply, see a `seq` that
is not its own, and leave both calls unanswered. Releasing the lock means a reader thread and a map of
pending replies, which is a concurrency model inside a DLL a game can unload.

That is why replacing the layer is a change of its own rather than a fix. A replacement has to keep
five things, all of them pinned by `tests/test_server.cpp` and `tests/test_end_to_end.cpp` today:

* `exchange()` blocks and has an upper bound, and a failure leaves the connection **closed**, so the
  next call dials again.
* the connect is bounded by the timeout `set_timeout_ms` was given.
* `close()` is safe from another thread and from whichever thread sits inside `exchange()`, and it
  waits for the round trip in flight rather than pulling the socket out from under it.
* nothing throws across the exported boundary into the game, ever.
* a stop never hangs. It wakes a reader parked in `recv`, joins the workers, and flushes the
  transcript after them.

An async library brings traps of its own. The first is that an `io_context` and its threads would live
inside a module a game can unload, so `FreeLibrary` could run while one of them is inside the stub.
The second is that Asio throws, and only its `error_code` overloads keep the promise above. The third
is that the server's reader changes shape, because cancellation and a completion handler replace the
`select()` slice and the `_stopping` flag, and `stop()` has to keep the contract above. Standalone Asio
would also be one more dependency, after the submodules nlohmann/json, GLFW and Dear ImGui and the
system `ws2_32`.

Two measurements in `tests/test_server.cpp` are what the decision can be made from, both against a
real server over a real socket.

* **A stalled backend and three threads.** A scripted answer can take a stated time, with
  `"delay_ms": 3000`. The server waits before answering, outside its state lock, and the transcript's
  `ms` counts the wait. With a delay longer than the client's timeout, three threads calling through
  one client cost **three timeouts one after another**, 1,844 ms measured for 3 x 600. Three clients
  with a connection of their own cost **one timeout**, 600 ms. The second arm says the server is not
  the serialiser, since it answers each connection on its own thread, so the difference between the
  arms is the lock. A slow backend is the server's own queueing either way, and the lock costs nothing
  there. A stalled one is where the price lands, and every queued caller waits out its own timeout in
  turn.
* **A megabyte each way.** 1 MB of arguments answered with 1 MB of out-parameters, byte for byte, and
  the same megabyte whole in one line of the transcript. The sender refuses a frame over
  `kMaxFrameBytes` without costing the connection, and the receiver hangs up rather than read a length
  it cannot honour. No rig run has ever exceeded a kilobyte, the largest frame in a whole set being a
  1 KB packet carried as hex, while a screenshot buffer, the biggest thing this API hands over as a
  value, is about 1.2 MB.

## Defaults and failure

Three states, and the difference matters:

* **answered**: the backend had something to say, and the game gets exactly that.
* **declined**, meaning `answer: "default"`: the backend knows the call and has no opinion, so the stub
  behaves as if Steam were absent.
* **unreachable**: no backend, or it went away, or it timed out. The stub logs the failure once,
  returns the same defaults, and retries on the next call, so a backend can start mid-run.

The defaults are the values a game sees without Steam: `false`, `0`, an empty string, a null interface
pointer. Nothing here invents success.

## The interfaces a recent SDK calls

A game built against a recent SDK does not import the per-interface accessors, because they are inline
in its own headers. It asks `SteamInternal_CreateInterface` for a version string and calls the object
it gets back through the vtable, so none of those calls reaches a trampoline. The stub answers them
with objects of its own.

`SteamInternal_CreateInterface` is still a generated trampoline like every other call. It asks the
backend first, so a scenario can answer it and the transcript records the request, and it hands out
one of ours only when nobody answered. That object is a singleton per version string, built from
`gen/steam_interfaces.json`. [development.md](development.md#interface-layouts) covers where those
layouts come from.

Three decisions keep that from being 1,320 hand-written functions, which is the number of slots the
layouts imported here declare:

* **The declarations speak the wire's types, not an SDK's.** An enum is an int, `CSteamID` is eight
  bytes, and a structure returned by value is as many bytes as the file says. Nothing from an SDK is
  vendored, so the marshalling for a slot is a property of the type a declaration used, and each kind
  appears once in `bridge/synth.hpp` rather than once per slot. The compiler still lays out the vtable
  and the calling convention.
* **What does not differ per slot is written once.** Building the request, sending it and reading the
  reply has nothing to do with any one signature, so it is one out-of-line function in
  `src/synth.cpp`. In the header it cost a copy in each of the 820 distinct signatures the file
  declares, and 700 KB of the DLL's code when that was measured. The calls are pooled too: the same
  call with the same arguments declared in six versions is one entry, and the linker folds the
  identical thunks that reach it.
* **One name per call, whichever route reached it.** A slot is named what the newest imported SDK
  names that method with that signature, not what the game's own SDK called it, because the SDKs
  renamed these along the way. Asking the same question through the flat import and through the vtable
  therefore arrives as one call, and a scenario that answers it answers both routes. The
  `STEAM_PRIVATE_API` slots have no flat entry point, so they keep the name the layouts gave them.

The objects answer nothing themselves. Every slot forwards to the backend, and what the backend
declines falls back to the default the game would see with Steam absent.

A game reaches these objects one of two ways, and the stub helps only with the fetch:

* A recent SDK's accessors are inline. They bounce through `SteamInternal_ContextInit` with a blob of
  `{ void (*pFn)(void*); uintptr_t counter; void* value }`, whose `pFn` is the game's own initialiser.
  The stub runs that initialiser and returns the address of `value`, and the initialiser asks us for
  the interface through `SteamInternal_CreateInterface` in turn. Only the first call reaches the
  backend. The rest are the game's accessor reading the SDK's cache, once per use of an interface,
  which for a game that polls is thousands a second: Spacewar's own loop asked 26,228 times in
  fourteen seconds, and reports one.
* An older SDK's accessors go through `ISteamClient::GetISteamUser( hUser, hPipe, pchVersion )` and
  its siblings. Those are slots like any other whose version-string argument names what they return,
  so a declined one is answered with our object for that string, the same fallback
  `SteamInternal_CreateInterface` has.

`SteamworksExample.exe` takes both routes: with `SteamAPI_Init` scripted true it collects its
interfaces, polls its controller through them, and exits on what the backend told it instead of
faulting on a null pointer.

## Structures and buffers the game owns

Both of these are a call that writes into memory the game allocated. An SDK's declaration cannot
describe either one, so `tools/steamworks_sdk_import.py` carries the answer in a table, keyed by
class, method and parameter name.

### A structure handed back through a pointer

`steam_api_flat.h` declares these `void *`: `GetDownloadedLeaderboardEntry` is
`void * pLeaderboardEntry` there and `LeaderboardEntry_t *` in the interface header. A pointer to a
structure is otherwise a list, whose count is the caller's business, and the wire has no shape for a
list. `SINGLE_STRUCTS` therefore has one line per pointer that is a single structure, and the row it
writes names that structure.

A structure has a `Kind<>` trait like any other type, so it could be reported and defaulted but not
set. A structure named by an out-parameter gains one method, `store`, and the generated file gains a
`store_<Structure>(<Structure>*, const Json&)` that writes the declared members field by field, with
the same expression a callback payload is filled by. A member the wire cannot carry, such as an array
or a `void *`, is left as the caller had it rather than zeroed, because that memory belongs to the
game.

### A buffer and its length

A buffer is two values on the wire, the bytes and how many of them, so the layouts name both the kind
and the parameter that carries the length. `BYTE_BUFFERS` chooses the buffers that travel as hex, and
`TEXT_BUFFERS` the ones that travel as text, which is what
`GetItemDefinitionProperty( def, "name", buf, &bufSize )` writes. A named type with a length is a list
of that many:

    ["pchValueBuffer", "out_text", "punValueBufferSizeOut"]
    ["pOutItemsArray", "SteamItemDetails_t", "out", "punOutItemsArraySize"]

The length parameter takes one of two shapes, and the SDK uses both. The first is a plain capacity, as
in `GetAppName( nAppID, pchName, cchNameMax )`. The second is the caller's own pointer, which is the
two-call shape a game sizes a buffer with: `GetResultItems( handle, NULL, &count )` asks how many there
are, allocates that many, and asks again. A length that is a pointer belongs to the buffer. It arrives
as the room the caller has, and the number the value needs travels back through it. The buffer is the
only writer, which is why the generated call wraps such a parameter in `BufferLength` rather than
passing it as an out-parameter. The declared type stays the SDK's own, whether that is `char *`,
`SteamItemDetails_t *` or `uint32 *`, because only the argument changed, not the ABI.

The wrappers live in `bridge/synth.hpp`: `BytesOut`, `TextOut` and `ArrayOut<T>`. Each is the store of
the kind it carries. `BytesOut` writes hex into the game's bytes, `TextOut` writes text and its
terminator, and `ArrayOut<T>` writes one `Kind<T>::store` per element, which is the store a single
structure through a pointer already had. All three report the same count: the number the value needs
with the terminator included. A caller with the room is told what it got, a caller without it is told
what to ask for, and a caller that passed a null buffer gets the count with no write at all. A buffer
with no room is written nowhere and reported, never overrun.

`ISteamInventory` is the first feature on this, and Spacewar's stats screen is the read-out. The screen
lists one line per item, by the name the catalogue gave it: a `GetItemDefinitionProperty` into a buffer
the game owns, straight after a `GetResultItems` into an array the game owns. See `src/inventory.cpp`
for the catalogue and for the payloads that end a result, `SteamInventoryFullUpdate_t` first and then
`SteamInventoryResultReady_t`.

That screen also found the one thing those two calls need before they are reached. Spacewar draws
nothing on it, not even the inventory, until a `UserStatsReceived_t` payload arrives, so answering
`RequestCurrentStats` with `true` and stopping there leaves the screen saying "Unable to retrieve data
from Steam". That payload carries the app id, the result and the player it is about, which is why the
state machine owns it rather than a scenario. See `h_request_current_stats` in `src/session.cpp`.

Neither mechanism covers the flat half of the same call, because the flat generator's kinds know
neither a structure nor a length. A game that reaches such a call through its flat spelling still gets
the opaque pointer it always did. Nor does either cover a structure passed or returned by value, which
no call in the layouts uses.

## The state file

A run keeps everything it knows in memory and nothing else, so the next run starts from the same place
this one did. That is what makes a run repeatable - and what makes an achievement unlocked on Tuesday
locked again on Wednesday. `--state` is the one thing here that outlives the process, and it is off
unless a person asks for it.

Three decisions shape it.

**What goes in is what a game wrote.** A scenario stays the file a person reads and hands to somebody
else; the store holds what games did to it. The welcome a game is given is a *merge* of the two, and
the direction is one way round: a value a game wrote stands, a value only a scenario has written is
refreshed from it, and nothing is deleted for being absent from either. `source` - one column, saying
`'scenario'` or `'game'` - is the whole of that rule, which is why seeding is the only place it is read.

That is also the one place a state file changes a *single* run's behaviour: two games matched to one
profile merge from the same record, so they can see each other's writes. Without a store they are handed
copies and cannot. Deliberate, and said out loud in `bridge/store.hpp`: one Steam id with two sets of
stats is not what keeping state is meant to mean.

**The store speaks this domain, not SQL.** `bridge/store.hpp` names no SQLite type and no query; its
methods are "a stat a game wrote", "the boards the store holds", "an inventory, by player". So
everything that knows what the file *is* sits in `src/store.cpp`, and the seven tables are a decision one
translation unit holds rather than one every caller reads past. Nothing there is app-scoped, and that is
a mirror rather than an oversight: a board is keyed by its name and an inventory by the player holding
it, exactly as the worlds key themselves in memory, so two apps with a board of the same name share one
row for the same reason they share one board within a run.

**The free end is that it is a database.** A person can read it, and add to it, with any SQLite client.
Nothing in this repository is needed for that, and it is not decoration: a friend added with a prompt is
read back like a friend a run learned about, because nothing here deletes a row it was not told about.

The one thing the store must not do is fail quietly, and it never throws. A file that will not open, or
one written with a different schema, stops the run at startup with the reason. A *write* that fails
later cannot stop the run - the game still has to be answered - so it is recorded and the backend says
it once, on the first call that finds it. A run that behaves normally while keeping nothing is the
failure nobody would notice on their own.

A write happens while the server's `_mutex` is held, because resolving a call is what does the writing.
One row in a transaction of its own would otherwise put a disk flush in front of every other game in
the run, and this is the one place where the cost of a state file is paid by games that are not using
it. The file is therefore kept in SQLite's write-ahead log, and
[development.md](development.md#the-state-file) has the measurement behind that.

## Where the next features attach

* **A live view** needs no restructuring. `Server` already exposes the two snapshots a panel draws:
  the connected and recently disconnected sessions with their profiles and stats, and the call history
  with what each call resolved to.
* **A reader thread in the stub** is what would reach a game that has stopped calling anything. No
  thread is needed while the game pumps, because the backend sends a payload on the reply to a call the
  game already made, and the stub hands it to the game's own `CCallbackBase` object inside the game's
  own `RunCallbacks`. The socket layer note works out the cost of that thread.
* **Record and replay** replaces the transport. A `PassthroughTransport` forwards to a real
  `steam_api64.dll` loaded under another name and records both directions to the same JSON the
  transcript uses, and a `ReplayTransport` serves that file back.
* **Two call shapes are left:** the flat spelling of the calls described above, and a structure passed
  or returned by value. Everything else that is not done is blocked by the socket layer rather than by
  a call shape.

## Known limitations

* A returned string is copied into a per-thread buffer that stays valid until the next call on that
  thread, because the reply it came from dies with the call. A game that stores the pointer would read
  the next call's text. Games normally copy immediately.
* Interface pointers are tokens, except where they are objects of ours. A scenario answers
  `SteamInternal_CreateInterface`, so the backend sees that same number back on every later call and
  cannot dereference it. A version string the stub has layouts for is the other case: the game gets a
  real object whose vtable is ours, and its calls arrive as calls like any other.
* Scalars, C strings, pointers and out-parameters travel. A structure passed or returned by value does
  not: the stub reports the call, sends the structure's bytes as null, and gives the caller a zeroed
  one back rather than inventing a value. A structure the layouts name as an out-parameter works, and
  so does a buffer with a length. One behind a `void *` the layouts do not name, whether that is a
  length-prefixed buffer or a list nobody sized, is still the opaque pointer it was.
* The ABI those objects present was checked once, by compiling a client from Valve's own headers and
  watching where its calls landed. The compiler lays the calls out as that SDK says to, so the slots it
  reaches are the slots a shipped game reaches. That check settled the overload order above, and the
  layouts keep it. Nobody can repeat it for a game built against an SDK whose headers are gone. The
  generation Spacewar was built against is one of those, and its entries are the file's own record.
* A game can ask for an interface version the stub has no layout for, which is anything newer than the
  SDKs the file was imported from. It gets null, exactly as it would from a stub that did not answer at
  all, and the transcript says which string it asked for. Importing another generation is one command.
  See [development.md](development.md#interface-layouts).
* Floating point goes over the wire as JSON numbers. The writer is exact and a reader gets the same
  double back. The bridge's own hand-rolled parser is the loose end: it keeps integers exact and
  accumulates fractions digit by digit, so a `float` parameter is rounded through a `double`.
* One connection per process, serialised calls, and no multiplexing of several games onto one socket.
  A game per connection is simpler and matches how they run.
* A game that disconnects keeps its session, so the run summary and the transcript stay about the whole
  run. `SessionSnapshot::connected` is what tells a view which rows are still live.
