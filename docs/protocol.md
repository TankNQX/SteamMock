# Protocol

Two frames per call: a request, then the reply to it. Everything is UTF-8 JSON inside a length
prefix, so a transcript is readable and both ends can be written in an afternoon.

## Framing

```
+--------+--------+--------+--------+-------------------------------+
| length (uint32, little endian)    | UTF-8 JSON payload …          |
+--------+--------+--------+--------+-------------------------------+
```

* The length counts the payload only, and never includes itself.
* `0` and anything above 4 MiB (4 194 304, `kMaxFrameBytes` / `MAX_FRAME_BYTES`) is a protocol
  error. Both sides refuse rather than allocate.
* One frame is one message. A reader that sees two frames in one `read` must handle both.
* Both ends are: `src/transport_tcp.cpp` and `python/steambridge/protocol.py`. They are checked
  against each other by `python_end_to_end`, which is the only test that speaks both.

## Messages

Every message has `type` and `v` (protocol version; currently `1`).

### `hello` - stub to backend, immediately after connecting

| Field | Meaning |
| --- | --- |
| `exe` | Executable file name, for match rules and for the transcript. |
| `arch` | `x64` or `x86`. |
| `pid` | Process id (Windows). |
| `module` | Fixed string identifying the stub build. |

### `welcome` - backend to stub

| Field | Meaning |
| --- | --- |
| `session` | Session id, a short hex string, unique per connection. |
| `profile` | Name of the profile the session was matched to. |

### `call` - stub to backend

| Field | Meaning |
| --- | --- |
| `seq` | Per-connection counter. The reply must carry the same number. |
| `name` | The exported function name, exactly as games import it. |
| `args` | Object of the call's arguments, keyed by the names in the IDL. |
| `session` | Session id from the welcome, echoed back for the transcript. |

### `reply` - backend to stub

| Field | Meaning |
| --- | --- |
| `seq` | Same as the request. A reply for another sequence is ignored. |
| `answer` | `"handled"`, or `"default"` for "no opinion, use your default". |
| `ret` | Only when handled: the return value. |
| `out` | Only when handled: object of out-parameter names to new values. |

`answer: "default"` is the important one: it is how the backend says "I know this call and I am not
going to answer it", which is not the same as failing. The stub then returns what it would return
with Steam not running, and records the call as unanswered.

### `bye` - either side

Sent when a side is closing cleanly. EOF means the same thing, and is the normal way a game leaves.

## Types

The IDL's type table, and how each one travels:

| IDL type | C++ | JSON | Default when unanswered |
| --- | --- | --- | --- |
| `void` | `void` | - | - |
| `bool` | `bool` | `true` / `false` (also accepts `1` / `0`) | `false` |
| `int32` | `std::int32_t` | number | `0` |
| `uint32` | `std::uint32_t` | number | `0` |
| `int64` | `std::int64_t` | number | `0` |
| `uint64` | `std::uint64_t` | number | `0` |
| `float` | `float` | number | `0.0f` |
| `double` | `double` | number | `0.0` |
| `cstring` | `const char*` | string, or `null` | `""` (never `nullptr`) |
| `opaque_ptr` | `void*` | number (the address) | `nullptr` |

Whole numbers stay whole: the C++ parser keeps integers away from `double`, so a 17-digit Steam id
arrives exactly, and the Python side uses `int`. Pointers travel as integers because the backend has
no business following them - it can correlate them, which is what a pass-through needs.

## Out-parameters

A parameter with `"dir": "out"` travels twice:

1. the stub sends the value the game's variable currently holds (or `null` if the game passed a null
   pointer, in which case the stub never dereferences it);
2. if the reply's `out` object has that name, the stub writes the new value back through the pointer.

Omitting the name leaves the caller's value untouched, which is how "the backend only answers what it
cares about" works for out-parameters too: real Steam leaves the caller's variable alone when a stat
name is unknown, and so does this.

## Versioning

`v` is checked by both sides. A mismatch is logged and the call unanswered rather than guessed at -
a harness that silently mis-parses is worse than one that says so. Adding a message type or a field
does not need a version bump; changing the meaning of an existing one does.
