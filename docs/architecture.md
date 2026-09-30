# Architecture

## The pieces

| Piece | Where | Responsibility |
| --- | --- | --- |
| Trampolines | `src/generated/api_stub.cpp` | One exported function per IDL entry. Marshals arguments, sends the call, reads the reply, falls back to a default. |
| Interfaces | `src/generated/api_interfaces.cpp`, `include/bridge/synth.hpp` | One object per interface version, handed out for a version string, whose slots forward into the same protocol. The marshalling is a property of the declaration's types, so a generated slot body is one line. |
| Client | `src/client.cpp` | One connection per process, the handshake, request/reply sequencing, offline behaviour, and the promise that nothing throws into the game. |
| Transport | `src/transport_tcp.cpp` | Framed loopback TCP. It sits behind `bridge/transport.hpp` so named pipes or shared memory can replace it. See [what such a replacement has to keep](#the-socket-layer-and-what-replacing-it-would-take). |
| Protocol | `include/bridge/frame.hpp`, `include/bridge/protocol.hpp` | The 4-byte length prefix, the version stamp, and the shape of a reply. One definition, linked by both halves. |
| Server | `src/server.cpp` | Accepts sessions, resolves each call, writes a transcript, and hands out snapshots of what it has seen. |
| Session | `src/session.cpp` | The per-game state machine: identity, language, app id, stats, achievements. |
| Scenario | `src/scenario.cpp` | Which profile a connecting game gets, and which calls a scenario overrides. |
| The world | `src/lobby.cpp`, `src/leaderboard.cpp`, `src/inventory.cpp` | The state no single session can answer: the lobbies and their members, the leaderboards the run's players have posted scores to, and the item catalogue beside what each player holds. All of it outlives the sessions that made it, because "find the one somebody else made" is the whole point of each. |
| Generator | `src/idl.cpp`, `src/codegen_main.cpp` | Turns `gen/steam_api_surface.json` into the trampolines, the `.def` and the table of exported calls, and holds the mock's own decisions, by call name. |
| Interface layouts | `src/interfaces.cpp`, `tools/steamworks_sdk_import.py` | Turns an SDK's headers into `gen/steam_interfaces.json` and then into `src/generated/api_interfaces.cpp`: the payloads a callback delivers, and the calls that can be made through an interface object. This is also where the file decides that a call's `void *` is really one structure. See [the note below](#a-structure-the-game-gets-back). |

Everything above is one CMake project and one toolchain. The only thing the two halves do not share
is their entry point: the stub is a DLL that a game loads, the backend is a program a person starts.

## One call, end to end

1. The game calls an exported name. On Windows/x64 the import table resolves it to the stub, which
   is why the DLL has to be named `steam_api64.dll` and sit next to the executable.
2. The trampoline builds a JSON object of its arguments. Pointers go over the wire as opaque
   integers, so the backend can echo them but not follow them.
3. `Client::call` takes the round-trip lock, dials the backend if it is not connected, sends
   `{"type":"call","seq":N,...}`, and blocks for the matching reply. Loopback latency is
   microseconds; the timeout, which is `STEAMMOCK_TIMEOUT_MS` and 2000 ms by default, is a ceiling,
   not a target.
4. `Server` hands the call to `Dispatcher::answer`, which tries the scenario, then the session, and
   reports which of them spoke. It answers with `answer: "handled"` plus `ret` and `out`, or with
   `answer: "default"`.
5. On `handled`, the trampoline converts the return value to its declared type and writes any
   out-parameters back through the pointers the game passed in. On anything else, whether that is
   declined, unreachable, timed out or unparsable, it returns the default for its return type.

A call through an interface object starts at step 2 and takes the same path from there. What the
`SteamInternal_CreateInterface` trampoline handed out is a singleton whose vtable is ours, so the slot
the game calls is one of our functions. It names the call, sends what the game passed, and reads the
reply exactly as a trampoline does. That is why one scenario answers both routes and the transcript
has one vocabulary for one API.

## Threading

Calls arrive on whatever thread the game uses, so one round trip is held under a mutex. That
serialises Steam calls from different threads. For a tool that watches a game this is a feature
rather than a limitation, because request/reply pairing cannot get confused and the transcript stays
in the order a game made its calls. [The next section](#the-socket-layer-and-what-replacing-it-would-take)
covers what that costs a game whose backend has stopped answering, and what replacing the layer
underneath it would take.

The server is the other way round: it accepts on one thread and serves each connection on its own,
because a game per connection is the model and there are only ever a few. Everything mutable, meaning
the sessions, the records, the counters and the transcript, sits behind one mutex, and the code holds
no lock while it reads or writes a frame. A front end that wants to draw the current state calls
`sessions()` or `records()` and gets a copy, so it can never be looking at state that is being
changed underneath it. What it gets for the history is a window of the last 20,000 calls,
`Server::kMaxRecords`, because a game that polls can make thousands a second and this is what a view
draws. The transcript is the record that keeps every call, and a reader's cursor is an absolute
position in the run rather than an offset into the window, which is what `records_begin()` and
`records_since()` mean.

The stub still has no background thread, and that is deliberate. It means no unsolicited message has
to be handled, and it keeps the DLL's behaviour at load time free of surprises. Callbacks arrive
inside the game's own `RunCallbacks`, because the backend sends them with a reply to a call the game
already made, as described below, and a thread that could reach a game which has stopped calling is
the same thread whose cost the socket layer note works out.

## The socket layer, and what replacing it would take

Four things speak sockets, and only one of them is behind an interface:

* **`bridge/transport.hpp` is what the client uses.** It has `connect`, `close`, `is_connected`,
  `exchange` and `set_timeout_ms`, which is deliberately narrow, and it is written so that a
  different implementation is a different object to construct rather than a change to the call path.
* **`src/transport_tcp.cpp` is the one implementation.** One socket, blocking, with a non-blocking
  connect waited for against a `select()` deadline. Any framing or send failure hangs the socket up,
  so a later call dials again rather than exchanging on a stream that is out of step.
* **`src/socket_io.hpp` holds the winsock calls both ends share:** the process-wide startup, "a
  handle lives in an integer", send-everything-or-nothing, and the half-close that wakes a thread
  parked in `recv`.
* **`src/server.cpp` holds the accept loop and each connection's reader, and neither is behind the
  interface.** It uses synchronous winsock, and the reader waits in `select()` slices so it can look
  at `_stopping` between them.

A review of this tree pushed on one point. `Client::call` holds one lock across the whole round trip,
so a game with a stalled backend serialises every other call it makes. The lock is not what bounds
that. The connect is bounded by the timeout the client asked for, where it used to be the TCP
stack's own SYN timer, 21,038 ms measured here against a host that answers nothing, whatever
`STEAMMOCK_TIMEOUT_MS` said. Any failure hangs the socket up, so a caller queued behind a dead
backend waits one timeout, 2 s by default, rather than twenty-one seconds each. Releasing the lock
while waiting is not a small change. There is one socket and one reply per request, in order, so two
threads waiting at once would race for the same frame. Whichever read first would take the other's
reply, see a `seq` that is not its own, and leave both calls unanswered. Doing it means a reader
thread and a map of pending replies, which is a concurrency model inside a DLL a game can unload.

That is the case for replacing the layer rather than bending it further, and it is a change of its
own. What a replacement has to keep, all of it pinned by `tests/test_server.cpp` and
`tests/test_end_to_end.cpp` today:

* `exchange()` blocks and has an upper bound; a failure leaves the connection **closed**, so the
  next call dials again.
* the connect is bounded by the timeout `set_timeout_ms` was given.
* `close()` is safe from another thread, and from whichever thread is inside `exchange()`. It waits
  for the round trip in flight rather than pulling the socket out from under it.
* nothing throws across the exported boundary into the game, ever.
* the server's stop contract: a stop never hangs, it wakes a reader parked in `recv`, joins the
  workers, and flushes the transcript after them.

The traps that come with an async library as the replacement, Asio being the direction of record:

* **The `io_context` and its threads would live inside a module a game can unload.** `FreeLibrary`
  while a thread is running inside the stub is the crash this project least wants, and it is the same
  class of problem as the thread-local destructor a per-thread string used to leave registered in the
  DLL. It needs a shutdown that is safe to reach from `DllMain`, or a reason to keep every thread the
  game's own, which is what today's design does.
* **Asio throws.** Its `error_code` overloads are the alternative, and they have to be used on every
  call. "Nothing throws into the game" is not negotiable here.
* **The server's reader changes shape.** Cancellation and a completion handler replace the `select()`
  slice and the `_stopping` flag, and `stop()` has to keep the contract above. That is where a
  migration is most likely to hang, and the four-threads-stop-at-once test is what would say so.
* **It is one more dependency.** The third-party code in this tree is the submodules nlohmann/json,
  GLFW and Dear ImGui, plus the system `ws2_32`. Standalone Asio is header-only and would be another
  one, for x64 and Win32 under both MSVC and clang-cl.

Two things to decide when it is picked up. Whether a transport that is async underneath keeps the
client's blocking `exchange()`, which means a reader thread and a promise per request and is the
concurrency model above, or whether the client becomes async too, which reaches into every generated
trampoline, since those are the calls that must not block a game. And whether the stub and the server
take the same implementation, given that only the stub has the unload problem. The callback bullet
and the record/replay bullet below want the same seam, so replacing this layer, giving the stub a
reader thread and recording a session through a transport are one decision about the interface:
several implementations, one contract.

What that decision can be made *from*, instead of argued about: two measurements, both in
`tests/test_server.cpp`, both against a real server over a real socket.

* **A stalled backend and three threads.** A scripted answer can say how long it takes, with
  `"delay_ms": 3000`. The server waits before answering, outside its state lock, and the transcript's
  `ms` counts the wait. With a delay longer than the client's timeout, three threads calling through
  one client cost **three timeouts one after another**, 1,844 ms measured for 3 x 600, while three
  clients with a connection of their own cost **one timeout**, 600 ms. The second arm is what says
  the server is not the serialiser, since it answers each connection on its own thread, so the
  difference between the arms is the lock. Note which condition that is. A *slow* backend is the
  server's own queueing either way, and the lock costs nothing there. A *stalled* one pays the price,
  where every queued caller waits out its own timeout in turn.
* **A megabyte each way.** 1 MB of arguments answered with 1 MB of out-parameters, byte for byte,
  and the same megabyte whole in one line of the transcript. A frame over `kMaxFrameBytes` is refused
  by the sender without costing the connection, and an answer over it is refused by the receiver,
  which hangs up rather than read a length it cannot honour. Nothing in a rig run has ever exceeded a
  kilobyte, the largest frame in a whole set being a 1 KB packet carried as hex, while a screenshot
  buffer, the biggest thing this API hands over as a value, is about 1.2 MB. That is the range a
  replacement has to keep working in, and it is now pinned rather than assumed.

## Nothing in DllMain

`DllMain` only calls `DisableThreadLibraryCalls`. The socket, the log file and the environment are all
read lazily, on the first API call, because doing real work under the loader lock is a good way to
deadlock a game at startup.

## Defaults and failure

Three states, and the difference matters:

* **answered**: the backend had something to say, and the game gets exactly that;
* **declined**, or `answer: "default"`: the backend knows the call and has no opinion, so the stub
  behaves as if Steam were absent;
* **unreachable**: no backend, or it went away, or it timed out. The defaults are the same, the
  failure is logged once, and the call is retried on the next one so a backend can be started mid-run.

The defaults are the same values a game sees without Steam: `false`, `0`, an empty string, a null
interface pointer. Nothing here invents success.

## The interfaces a recent SDK calls

A game built against a recent SDK does not import the per-interface accessors: they are inline in its
own headers. It asks `SteamInternal_CreateInterface` for a version string and then calls the object it
gets back through the vtable, so none of those calls reaches a trampoline. The stub answers them with
objects of its own.

`SteamInternal_CreateInterface` is still a generated trampoline like every other call. It asks the
backend first, so a scenario can answer it and the transcript records the request, and only if nobody
did does it hand out one of ours. That object is a singleton per version string, built from
`gen/steam_interfaces.json`, the layouts imported from SDK headers and checked against real
`steam_api.dll` images. See [development.md](development.md#interface-layouts).

Three decisions keep that from being 2,115 hand-written functions:

* **The declarations speak the wire's types, not an SDK's.** An enum is an int, `CSteamID` is eight
  bytes, a structure returned by value is as many bytes as the file says. Nothing from an SDK is
  vendored, and the ABI is a size and a shape, so the marshalling for a slot is a property of the
  *type* a declaration used, and each kind appears once in `bridge/synth.hpp` rather than once per
  slot. That is what lets a generated slot body be one line, with the compiler still laying out the
  vtable and the calling convention.
* **What does not differ per slot is written once.** Building the request, sending it and reading the
  reply has nothing to do with any one signature, so it is one out-of-line function in
  `src/synth.cpp` rather than a copy inlined into every shape of call. As a header it cost a copy in
  each of the 374 shapes the file declares, and 700 KB of the DLL's code. The calls themselves are
  pooled too: the same call with the same arguments declared in six versions is one entry in the
  generated file, and the linker folds the identical thunks that reach it.
* **One name per call, whichever route reached it.** A slot is named what the newest imported SDK names
  that method with that signature, rather than what the game's own SDK called it, since the SDKs
  renamed these along the way: 1.47 says `SteamAPI_ISteamUserStats_GetStat` for the int32 overload
  where 1.51 and later say `SteamAPI_ISteamUserStats_GetStat`. Asking the same question through the
  flat import and through the vtable therefore arrives as one call, a scenario that answers
  `SteamAPI_ISteamUserStats_GetStat` answers it either way, and the transcript does not have two
  vocabularies for one API. The slots with no flat entry point, the `STEAM_PRIVATE_API` ones, keep
  the name the layouts gave them.

The objects answer nothing themselves. Every slot forwards to the backend, and what the backend
declines falls back to the default the game would see with Steam absent. A structure passed by value
is the one thing the wire cannot carry, so those calls are reported and answered with a zeroed value.
`gen/steam_interfaces.json` is where a newer SDK's version strings are added, and `steammock_codegen`
turns them into slots.

Two ways in, both ending at the same objects, and both of them needing the stub's help only because
they are the *fetch*:

* A recent SDK's accessors are inline and bounce through `SteamInternal_ContextInit`, handing it a
  blob of `{ void (*pFn)(void*); uintptr_t counter; void* value }` whose `pFn` is the game's own
  initialiser. The stub runs it and returns the address of `value`, and the initialiser asks us for
  the interface through `SteamInternal_CreateInterface` in turn. Only the first call reaches the
  backend. The ones after it are the game's accessor checking the SDK's cache, once per use of an
  interface, which for a game that polls is thousands a second: Spacewar's own loop asked 26,228 times
  in fourteen seconds, and reports one.
* An older SDK's accessors go through `ISteamClient::GetISteamUser( hUser, hPipe, pchVersion )` and
  its siblings. Those are slots like any other whose version-string argument names what they return,
  so a declined one is answered with our object for that string, the same fallback
  `SteamInternal_CreateInterface` has.

Spacewar's own `SteamworksExample.exe` is the case that exercised both: with `SteamAPI_Init` scripted
true it now collects its interfaces, polls its controller through them, and exits on what the backend
told it instead of faulting on a null pointer.

## A structure the game gets back

A call that hands the game a structure it owns, meaning the caller's own memory that the stub writes
into, is the one parameter shape an SDK's declaration cannot describe. `steam_api_flat.h` declares
these `void *`: `GetDownloadedLeaderboardEntry` is `void * pLeaderboardEntry` there and
`LeaderboardEntry_t *` in the interface header, and a pointer to a structure is otherwise a *list*,
whose count is the caller's business and which the wire has no shape for. So the file carries the
answer, exactly as it carries the answer about which buffers travel as bytes:
`SINGLE_STRUCTS` in `tools/steamworks_sdk_import.py` has one line per pointer that is a single
structure, keyed by class, method and parameter name, and the row it writes names the structure
instead of the pointer.

Everything after that was already there. A structure has a `Kind<>` trait like any other type, so it
could be reported and defaulted but not *set*. A structure named by an out-parameter therefore gains
one method, `store`, and the generated file gains a `store_<Structure>(<Structure>*, const Json&)`
that writes the members the layouts declare, field by field, with the same expression a callback
payload is filled by. A member the wire cannot carry, such as an array or a `void *`, is left as the
caller had it rather than zeroed, because that memory belongs to the game.

What this does not cover is the flat half of the same call, where the flat generator's kinds have no
notion of a structure, so a game that reaches the call through the flat spelling still gets an opaque
pointer. Nor does it cover a structure passed or returned *by value*, which no SDK call in the
layouts uses. A buffer with a length is the same problem one step further out, since the bytes are
the game's and so is the room for them, and it is the next section.

## A buffer the game owns, and its length

A byte buffer is two values on the wire, the bytes and how many of them, so the layouts name both:
the kind, and the parameter that carries the length. `BYTE_BUFFERS` in
`tools/steamworks_sdk_import.py` is where a buffer that travels as hex is chosen, `TEXT_BUFFERS`
the same for one that travels as text, which is what `char *` and a length really is:
`GetItemDefinitionProperty( def, "name", buf, &bufSize )` is a string written into the game's own
memory, and a *list* the game owns is one more string on the row, because a named type with a
length is that many of them:

    ["pchValueBuffer", "out_text", "punValueBufferSizeOut"]
    ["pOutItemsArray", "SteamItemDetails_t", "out", "punOutItemsArraySize"]

What the length parameter *is* then decides the rest, and the SDK uses both shapes. One is a plain
capacity, as in `GetAppName( nAppID, pchName, cchNameMax )`. The other is the caller's own pointer,
which is the two-call shape a game sizes a buffer with: `GetResultItems( handle, NULL, &count )` asks
how many there are, allocates that many, and asks again. A length that is a pointer belongs to the
buffer. It arrives as the room the caller has, and the number the value needs travels back through
it. The buffer is the only writer, which is why the generated call wraps such a parameter in
`BufferLength` instead of passing it as an out-parameter, since two writers of one number is one too
many, and why the declared type stays the SDK's own, whether that is `char *`, `SteamItemDetails_t *`
or `uint32 *`. What changed is the argument, not the ABI.

The wrappers are in `bridge/synth.hpp`: `BytesOut`, `TextOut` and `ArrayOut<T>`. Each is the store of
the kind it carries, writing hex into the game's bytes, text into its chars with the terminator, and
one `Kind<T>::store` per element for a list, which is the store a single structure written through a
pointer already had. The count that comes back means the same thing in all three, the number the
value *needs* with the terminator included. A caller that had the room is told what it got, a caller
that did not is told what to ask for, and a caller that passed a null buffer gets the count with no
write at all, which is the probe half of the two-call shape. A buffer with no room is written nowhere
and reported, never overrun, because the bytes past it belong to the game.

`ISteamInventory` is what this was built for and the first feature on it: the catalogue of item
definitions, which belongs to the app and therefore to no single session, beside what each player
holds. Items arrive the way they do in a real inventory, by the game asking for the promotions it is
entitled to and this world granting them once. See `src/inventory.cpp`, which is also where the two
payloads that end a result are queued, `SteamInventoryFullUpdate_t` first and then
`SteamInventoryResultReady_t`. Spacewar's own stats screen is the read-out: it lists one line per
item, by the name the catalogue gave it, which is a `GetItemDefinitionProperty` into a buffer the
game owns straight after a `GetResultItems` into an array the game owns.

That screen also turned up what those two calls need before they are ever reached. Spacewar draws
*nothing* on it, not even the inventory, until a `UserStatsReceived_t` payload has been handed to it,
and answering `RequestCurrentStats` with `true` and stopping there leaves it saying "Unable to
retrieve data from Steam". That payload belongs to a session, carrying the app id, the result and the
player it is about, which is why the state machine owns it rather than a scenario. See
`h_request_current_stats` in src/session.cpp.

What is not covered is the flat half of any of these: the flat generator's kinds have no notion of
a buffer with a length, so a game that reaches such a call through its flat spelling still gets the
opaque pointer it always did. Spacewar reaches the inventory through the interface, which is why
the interface path is the one that had to work.

## Where the next features attach

* **A live view** is the reason `Server` is free of any front end. It already exposes the two
  snapshots a panel needs, which are the connected and recently disconnected sessions with their
  profiles and stats, and the call history with what each call resolved to, so a window is a way of
  drawing them rather than a restructuring. Editing a profile's stats or scripting a call from that
  view is the replacement for the Python hook the port removed.
* **Callback injection** is done the roundabout way, and a reader thread would replace its shape. A
  payload the backend wants a game to have travels back on the reply to a call the game already made,
  waits in the stub's queue, and is handed to the game's own `CCallbackBase` inside the game's own
  `RunCallbacks`. No thread is needed, and nothing is ever delivered while the game is not pumping.
  What that cannot do is reach a game that has stopped calling anything, which is what a reader
  thread in the stub would be for, and it is the same thread whose cost the socket layer note above
  works out.
* **Record / replay** replaces the transport: a `PassthroughTransport` forwards to a real
  `steam_api64.dll` loaded under a different name, records both directions to the same JSON the
  transcript uses, and a `ReplayTransport` serves that file back. The backend then never has to
  invent anything for the recorded session.
* **Struct and buffer parameters** are the mirror of [what a structure the game gets back](#a-structure-the-game-gets-back)
  and [what a buffer the game owns](#a-buffer-the-game-owns-and-its-length), both of which are done:
  the layouts name the structure or the buffer and its length, the generated trait has a store, and
  the value is written into the game's own memory. What is left there is the flat spelling of the
  same calls, where the flat generator's kinds know neither a structure nor a length, and a structure
  passed or returned *by value*, which nothing in the layouts does. The features still blocked by the
  socket layer rather than by a call shape are the ones whose cost the transport note works out.

## Known limitations

* A returned string is copied into a per-thread buffer that stays valid until the next call on that
  thread, because the reply it came from dies with the call. Games normally copy immediately; one
  that stores the pointer would read the next call's text.
* Interface pointers are tokens, except where they are objects of ours. `SteamInternal_CreateInterface`
  returns whatever the scenario says, and the backend sees that same number back on every later call;
  it cannot dereference it. A version string the stub has layouts for is the other case: the game
  gets a real object whose vtable is ours, and its calls arrive as calls like any other.
* Scalars, C strings, pointers and out-parameters travel; a structure passed or returned *by value*
  does not. The call is reported, with the structure's bytes going over as null, and the caller gets
  a zeroed one back, the same answer a game gets with Steam absent, rather than an invented value.
  A structure the *layouts name as an out-parameter* is the exception and now works: the generated
  trait writes its declared members into the game's own memory, which is how a leaderboard row
  arrives. See [a structure the game gets back](#a-structure-the-game-gets-back). A structure behind
  a `void *` the layouts do not name, whether that is a length-prefixed buffer or a list nobody
  sized, is still the opaque pointer it was, and so is a structure returned by value.
* The ABI those objects present was checked once, by compiling a client from Valve's own headers and
  watching where its calls landed: the compiler lays the calls out as that SDK says to, so the slots
  it reaches are the slots a shipped game reaches. That is what settled the overload order above, and
  the layouts keep it. What nobody can check any more is a game built against an SDK whose headers are
  gone. The generation Spacewar was built against is one of those, and its entries are the file's own
  record.
* A game can ask for an interface version the stub has no layout for, which is anything newer than
  the SDKs the file was imported from. It gets null, exactly as it would from a stub that did not
  answer at all, and the transcript says which string it asked for. Importing another generation is
  one command. See [development.md](development.md#interface-layouts).
* Floating point goes over the wire as JSON numbers. The writer is exact, and a reader gets the same
  double back; the bridge's own hand-rolled parser is the loose end, keeping integers exact and
  accumulating fractions digit by digit, so a `float` parameter is rounded through a `double`.
* One connection per process, serialised calls, no multiplexing of several games onto one socket.
  A game per connection is simpler and matches how they run.
* A game that disconnects keeps its session, so the run summary and the transcript stay about the
  whole run. `SessionSnapshot::connected` is what tells a view which rows are still live.
* Flat entry points only, and that is a real boundary rather than a detail. A game built against a
  recent SDK gets its interfaces by asking `SteamInternal_CreateInterface` for a version string and
  then calling the object it hands back through its vtable. The per-interface accessors are inline in
  the game rather than imported, so none of those calls reach a trampoline. Spacewar's own
  `SteamworksExample.exe` is one of these: with `SteamAPI_Init` scripted true the transcript ends at
  `SteamInternal_ContextInit` declined, and the game's next call through the null pointer faults,
  with `SteamAPI_WriteMiniDump` reporting `0xC0000005` on the way out. Covering that case means
  synthesising the interfaces, an object per interface version whose vtable slots forward into this
  same IDL and protocol, which needs the slot order per version from a real SDK header or out of a
  real `steam_api.dll`.
