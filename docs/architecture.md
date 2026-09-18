# Architecture

## The pieces

| Piece | Where | Responsibility |
| --- | --- | --- |
| Trampolines | `src/generated/api_stub.cpp` | One exported function per IDL entry. Marshals arguments, sends the call, reads the reply, falls back to a default. |
| Client | `src/client.cpp` | One connection per process, the handshake, request/reply sequencing, offline behaviour, and the promise that nothing throws into the game. |
| Transport | `src/transport_tcp.cpp` | Framed loopback TCP. Behind `bridge/transport.hpp` so named pipes or shared memory can replace it. |
| Protocol | `include/bridge/json.hpp`, `include/bridge/frame.hpp` | The JSON subset and the 4-byte length prefix. The Python mirror is `python/steambridge/protocol.py`. |
| Backend | `python/steambridge/` | Accepts sessions, resolves each call (scenario, hook, state), writes a transcript. |

## One call, end to end

1. The game calls an exported name. On Windows/x64 the import table resolves it to the stub, which
   is why the DLL has to be named `steam_api64.dll` and sit next to the executable.
2. The trampoline builds a JSON object of its arguments. Pointers go over the wire as opaque
   integers, so the backend can echo them but not follow them.
3. `Client::call` takes the round-trip lock, dials the backend if it is not connected, sends
   `{"type":"call","seq":N,...}`, and blocks for the matching reply. Loopback latency is
   microseconds; the timeout (`STEAMBRIDGE_TIMEOUT_MS`, default 2000) is a ceiling, not a target.
4. The backend resolves the call - scripted, hook, or session state - and answers with
   `answer: "handled"` plus `ret`/`out`, or with `answer: "default"`.
5. On `handled`, the trampoline converts the return value to its declared type and writes any
   out-parameters back through the pointers the game passed in. On anything else - declined,
   unreachable, timed out, unparsable - it returns the default for its return type.

## Threading

Calls arrive on whatever thread the game uses, so one round trip is held under a mutex. That
serialises Steam calls from different threads; for a debugging harness that is a feature rather than
a limitation, because request/reply pairing cannot get confused and the transcript stays in the order
a game made its calls.

The stub has no background thread yet. That is deliberate for now: it means no unsolicited messages
have to be handled, and it keeps the DLL's behaviour at load time free of surprises. Callback
injection is what will change it (see below).

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

## Where the next features attach

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
  plus generator support. The type table is the only place that has to grow.

## Known limitations

* Scalars, C strings and out-parameters only: no structs, no buffers, no `char*` with a length yet.
* A returned string is copied into a per-thread buffer that stays valid until the next call on that
  thread, because the reply it came from dies with the call. Games normally copy immediately; one
  that stores the pointer would read the next call's text.
* Interface pointers are tokens. `SteamAPI_ISteamUser()` returns whatever the scenario says, and the
  backend sees that same number back on every later call; it cannot dereference it.
* Floating point goes over the wire as JSON numbers, so a `float` parameter is rounded through a
  `double`.
* One connection per process, serialised calls, no multiplexing of several games onto one socket -
  a game per connection is simpler and matches how they run.
