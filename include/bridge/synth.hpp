#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <type_traits>
#include <variant>

#include "bridge/call.hpp"
#include "bridge/client.hpp"
#include "bridge/json_read.hpp"

namespace steammock {

// ---------------------------------------------------------------------------
//  The interfaces the stub hands out itself.
// ---------------------------------------------------------------------------
//  A game built against a recent SDK does not import the per-interface
//  accessors: it asks SteamInternal_CreateInterface for a version string and
//  then calls the object it gets back through its vtable. Those calls reach no
//  flat trampoline, so the stub answers them with objects of its own, built from
//  gen/steam_interfaces.json.
//
//  Three things make that cheaper than it looks:
//
//  * The generated declarations speak the wire's types rather than an SDK's - an
//    enum is an int, CSteamID is eight bytes, a structure returned by value is as
//    many bytes as the file says. The ABI is what matters and nothing from an SDK
//    is vendored, so the marshalling is a property of the *type* a declaration
//    used, and each kind appears once here rather than once per slot.
//  * Every slot does the same four things: name the call, send what the game
//    passed, take back what was answered, fall back to a default. The only part
//    that differs per slot is its name and its parameter names, and those are
//    data - so a generated slot body is one line, and the compiler still lays out
//    the vtable and the calling convention.
//  * A slot packs its arguments into the wire form below and hands them to one
//    out-of-line function, `run_slot`, which builds the request and reads the
//    reply. A signature still owns its packing, which is where the calling
//    convention lives - but the JSON machinery exists once in the DLL rather
//    than once per signature, which is what keeps a wide interface file from
//    writing a fresh copy of it into every shape of call it declares.
//
//  Nothing here answers anything: a slot forwards to the backend exactly like a
//  trampoline does, and what the backend declines falls back to the same default
//  a game sees when Steam is not running.

// One slot: the name the call travels under, and the names of its parameters.
// Both are baked in by the generator, so a call costs one string and nothing per
// kind. `parameters` is null when the slot takes none.
struct SlotInfo {
    const char* call;
    const char* const* parameters;
};

// How many user handles one version of an interface is handed out for. A process
// that hosts is a customer and a game server at once, Steam gives each of them a
// user handle of its own, and the object a call is made through is the only thing on
// this side that can say which of them made it - so each handle needs an object of
// its own. Two is that number; a handle that arrives once both are spoken for reuses
// the first object, which is where a process running two game servers would be
// wrong, and nothing else is.
constexpr std::size_t kInterfaceEndpoints = 2;

// One interface version the stub can answer, and the objects to hand back for it -
// one per user handle, each remembering the handle it was handed out for. That
// memory is what lets a call made through one of them say who made it, which is the
// only way a packet queue can tell a customer's read from a game server's.
struct InterfaceVersion {
    const char* version;
    void* object[kInterfaceEndpoints];
    std::int32_t* user[kInterfaceEndpoints];
};

// A game that skips its null check should read empty text rather than fault.
inline const char kEmptyText[] = "";

// ---------------------------------------------------------------------------
//  One argument, packed.
// ---------------------------------------------------------------------------
//  `bits` is the value the wire carries and `wire` says how to read it: an
//  integer, a boolean, the bits of a double, or the address of the text. There
//  is no kind for a pointer or for a value class, because the protocol carries
//  neither - an address is an integer and CSteamID is the one integer it is -
//  and what an argument *is* has already been decided by the type it was
//  declared with, which is the one place that knows.
//
//  A null pointer is not a wire kind: `Kind<T*>::arg` packs the pointed-to value
//  and a null there is `wire_null`, which is the same null the wire has for a
//  value nobody could send.

// One packed argument, as the request carries it. The type travels with the value
// rather than beside it: a `uint64` with its top bit set and the `int64` that has
// the same bit pattern are different numbers on the wire, and a kind in a second
// field cannot say which one a value is - `wire_int(-1)` and `wire_uint(2^64-1)`
// used to pack identically, so the marshaller had to cast and pick a sign for
// both. Every spelling below keeps the type it was given instead.
using Arg = std::variant<std::nullptr_t, bool, std::int64_t, std::uint64_t, double, const char*>;

inline Arg wire_null() noexcept { return nullptr; }
inline Arg wire_bool(bool value) noexcept { return Arg(value); }

inline Arg wire_int(std::int64_t value) noexcept { return Arg(value); }
inline Arg wire_uint(std::uint64_t value) noexcept { return Arg(value); }
inline Arg wire_real(double value) noexcept { return Arg(value); }

// An address travels as the integer it is, which is what the protocol has for a
// handle no side can dereference - and as the unsigned one, because an address has
// no sign.
inline Arg wire_pointer(const void* value) noexcept {
    return Arg(static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(value)));
}

inline Arg wire_cstring(const char* value) noexcept { return Arg(value); }

// ---------------------------------------------------------------------------
//  The kind a C++ type travels as.
// ---------------------------------------------------------------------------
//  `in` is what the game passed, `from` is the return value the reply carries,
//  `store` writes an out-parameter back, `fallback` is what a caller gets when
//  nobody answered, and `out()` answers whether this argument type is one the
//  caller's pointer is written through - which only a pointer is, and never an
//  opaque one: a `void*` is a buffer the wire reports rather than carries.
//
//  `out()` is a function rather than a constant because of what clang does with
//  a generated constant nobody odr-uses: `if constexpr (Kind<T>::out)` reads it in
//  a constant expression, which is not a use, so every `static constexpr bool out`
//  in the generated file earned a -Wunused-const-variable. A predicate is read the
//  same way and warns about nothing.

template <class T, class = void> struct Kind {
    static_assert(std::is_arithmetic_v<T>,
                  "no wire kind for this type: the generated file has to specialize "
                  "steammock::Kind for it");

    static constexpr bool out() noexcept { return false; }

    static Arg arg(T value) noexcept {
        if constexpr (std::is_same_v<T, bool>) {
            return wire_bool(value);
        } else if constexpr (std::is_floating_point_v<T>) {
            return wire_real(static_cast<double>(value));
        } else if constexpr (std::is_signed_v<T>) {
            return wire_int(static_cast<std::int64_t>(value));
        } else {
            return wire_uint(static_cast<std::uint64_t>(value));
        }
    }

    static T from(const Json& reply) noexcept {
        if constexpr (std::is_same_v<T, bool>) {
            return reply_bool(reply);
        } else if constexpr (std::is_floating_point_v<T>) {
            return static_cast<T>(reply_real(reply));
        } else if constexpr (std::is_signed_v<T>) {
            return static_cast<T>(reply_int(reply));
        } else {
            return static_cast<T>(reply_uint(reply));
        }
    }

    static void store(T* target, const Json& value) noexcept {
        if (target == nullptr) {
            return;
        }
        if constexpr (std::is_same_v<T, bool>) {
            *target = as_bool(value);
        } else if constexpr (std::is_floating_point_v<T>) {
            *target = static_cast<T>(as_double(value));
        } else if constexpr (std::is_signed_v<T>) {
            *target = static_cast<T>(as_int64(value));
        } else {
            *target = static_cast<T>(as_uint64(value));
        }
    }

    static T fallback() noexcept { return T{}; }
};

// A call that returns nothing: there is no value to read and no default to pick.
template <> struct Kind<void> {
    static constexpr bool out() noexcept { return false; }
    static void from(const Json& reply) noexcept { (void)reply; }
    static void fallback() noexcept {}
};

// A string the game receives, copied out of the reply (see bridge/call.hpp).
// One a game sends is text rather than a buffer, which is why it is not opaque.
template <> struct Kind<const char*> {
    static constexpr bool out() noexcept { return false; }

    static Arg arg(const char* value) noexcept { return wire_cstring(value); }
    static const char* from(const Json& reply) { return reply_cstring(reply, kEmptyText); }
    static const char* fallback() noexcept { return kEmptyText; }
};

// An opaque pointer: an interface, a buffer, a handle the backend only echoes.
template <> struct Kind<void*> {
    static constexpr bool out() noexcept { return false; }

    static Arg arg(const void* value) noexcept { return wire_pointer(value); }
    static void* from(const Json& reply) noexcept { return reply_pointer(reply); }
    static void* fallback() noexcept { return nullptr; }
};

// A buffer the wire carries, as hex. A game's packets and a lobby chat line are bytes,
// and hex is the one shape the protocol already has for text - the marshalling asks for
// the buffer and its length together, and the hex lives until the end of the call that
// built it, which is as long as anything reads it.
inline std::string hex_of(const void* data, std::uint32_t size) {
    static constexpr char kDigits[] = "0123456789abcdef";
    std::string text;
    if (data == nullptr || size == 0) {
        return text;
    }
    const unsigned char* bytes = static_cast<const unsigned char*>(data);
    text.reserve(static_cast<std::size_t>(size) * 2u);
    for (std::uint32_t index = 0; index < size; ++index) {
        text.push_back(kDigits[bytes[index] >> 4u]);
        text.push_back(kDigits[bytes[index] & 0x0Fu]);
    }
    return text;
}

inline int hex_digit(char value) noexcept {
    if (value >= '0' && value <= '9') {
        return value - '0';
    }
    if (value >= 'a' && value <= 'f') {
        return value - 'a' + 10;
    }
    if (value >= 'A' && value <= 'F') {
        return value - 'A' + 10;
    }
    return -1;
}

// Writes as much of the hex as the game's buffer has room for, and answers how much that
// was: the game passed the capacity, and a stub that overran it would be writing into
// whatever is next to it in the game's own memory.
inline std::uint32_t bytes_into(void* data, std::uint32_t capacity, const std::string& hex) {
    unsigned char* target = static_cast<unsigned char*>(data);
    if (target == nullptr || capacity == 0) {
        return 0;
    }
    std::uint32_t written = 0;
    for (std::size_t index = 0; index + 1 < hex.size() && written < capacity; index += 2) {
        const int high = hex_digit(hex[index]);
        const int low = hex_digit(hex[index + 1]);
        if (high < 0 || low < 0) {
            break;
        }
        target[written++] = static_cast<unsigned char>((high << 4) | low);
    }
    return written;
}

// A buffer the wire carries, as hex. A game's packets and a lobby chat line are bytes,
// and hex is the one shape the protocol already has for text. The size is taken as a
// signed 64-bit value because the SDK declares these lengths both ways - a packet's is
// a uint32 and a chat line's an int32 - and braces will not narrow.
class Bytes {
public:
    Bytes(const void* data, std::int64_t size)
        : _hex(hex_of(data, size > 0 ? static_cast<std::uint32_t>(size) : 0u)) {}

    const std::string& text() const noexcept { return _hex; }

private:
    std::string _hex;
};

// A buffer the game wants filled in: the reply carries the hex and the stub writes it
// into the game's own memory, up to the capacity the game passed with the pointer.
struct BytesOut {
    void* data = nullptr;
    std::uint32_t size = 0;

    BytesOut() = default;
    BytesOut(void* target, std::int64_t capacity)
        : data(target), size(capacity > 0 ? static_cast<std::uint32_t>(capacity) : 0u) {}
};

template <> struct Kind<Bytes> {
    static constexpr bool out() noexcept { return false; }

    static Arg arg(const Bytes& value) noexcept { return wire_cstring(value.text().c_str()); }
    static Bytes from(const Json& reply) noexcept {
        (void)reply;
        return Bytes(nullptr, 0);
    }
    static void store(Bytes target, const Json& value) noexcept {
        (void)target;
        (void)value;
    }
    static Bytes fallback() noexcept { return Bytes(nullptr, 0); }
};

template <> struct Kind<BytesOut> {
    static constexpr bool out() noexcept { return true; }

    static Arg arg(const BytesOut& value) noexcept {
        (void)value;
        return wire_null();
    }
    static BytesOut from(const Json& reply) noexcept {
        (void)reply;
        return BytesOut{};
    }
    // Called with the parameter the caller passed, which for one of these is the whole
    // thing: the game's pointer and the room it has, which is why this takes it by value.
    // The value here is the hex from the reply's out-block, not the reply itself - reading
    // it with reply_cstring asked a string for a member named "ret" and got nothing back,
    // so every buffer read this way stayed exactly as the game left it: a game that read a
    // packet read its own uninitialised bytes and said so.
    static void store(BytesOut target, const Json& value) noexcept {
        bytes_into(target.data, target.size, as_string(value));
    }
    static BytesOut fallback() noexcept { return BytesOut{}; }
};

// A pointer to anything with a kind of its own is an out-parameter: what the
// caller passed is sent so the backend can see it, and what comes back is
// written through the pointer only if the backend sent it - which is what makes
// "answer only the calls you care about" work for out-parameters too.
//
// Sending it is the value it points at, widened by that type's own kind, so the
// pointer itself has a kind only when there is nothing to point at.
template <class T> struct Kind<T*, void> {
    static constexpr bool out() noexcept { return true; }

    static Arg arg(T* value) noexcept {
        return value != nullptr ? Kind<T>::arg(*value) : wire_null();
    }

    static void store(T* target, const Json& value) noexcept { Kind<T>::store(target, value); }
};

// ---------------------------------------------------------------------------
//  One slot, run.
// ---------------------------------------------------------------------------

// A null parameter-name array is only reachable if the generator emitted a slot
// with parameters and no names for them, which a test would catch; reading one
// safely is cheaper than trusting it here.
inline const char* name_at(const SlotInfo& info, std::size_t index) noexcept {
    return info.parameters != nullptr ? info.parameters[index] : "";
}

// One call, marshalled: the request carries the argument names the descriptor
// holds and the values as packed, and the reply is left for the caller to read.
// False means nobody answered and the caller falls back to a default.
//
// It is defined in synth.cpp rather than here, once, because the work it does -
// building a request, sending it, parsing what came back - has nothing to do
// with any one signature, and leaving it in the header would write a copy of it
// into every shape of call the generated file declares. hSteamUser is the user
// handle the object the game called through was handed out for, or 0 when the call
// did not come through one: only the calls that cannot be told apart without it put
// it on the wire, so most calls carry it nowhere.
bool run_slot(std::int32_t hSteamUser, const SlotInfo& info, const Arg* args, std::size_t count,
              Json& reply) noexcept;
template <class Parameter>
void store_out(const SlotInfo& info, std::size_t index, const Json& reply,
               Parameter parameter) noexcept {
    if constexpr (Kind<Parameter>::out()) {
        const Json* value = reply_out(reply, name_at(info, index));
        if (value != nullptr && !value->is_null()) {
            Kind<Parameter>::store(parameter, *value);
        }
    } else {
        (void)info;
        (void)index;
        (void)reply;
        (void)parameter;
    }
}

// The one marshalling path, so that a call made through an object of ours and one
// made without one cannot drift apart. hSteamUser is the handle the object was handed
// out for, or 0 when the call came through no object at all.
template <class Return, class... Parameters>
Return call_slot(std::int32_t hSteamUser, const SlotInfo& info, Parameters... parameters) noexcept {
    try {
        const Arg packed[sizeof...(Parameters) + 1] = {Kind<Parameters>::arg(parameters)...};
        Json reply;

        if (!run_slot(hSteamUser, info, packed, sizeof...(Parameters), reply)) {
            return Kind<Return>::fallback();
        }

        std::size_t index = 0;
        (store_out(info, index++, reply, parameters), ...);
        return Kind<Return>::from(reply);
    } catch (...) {
        // Never let an exception cross into the game.
        return Kind<Return>::fallback();
    }
}

template <class Return, class... Parameters>
Return slot(const SlotInfo& info, Parameters... parameters) noexcept {
    return call_slot<Return>(0, info, parameters...);
}

// The same call made through an interface object of ours, which knows the user handle
// it was handed out for. Two overloads rather than a second name is what keeps a
// generated body readable either way: the handle is one more thing in front of the
// call name, and a version of an interface asked for under two handles is two
// objects, which is how a customer's call is told from a game server's.
template <class Return, class... Parameters>
Return slot(std::int32_t hSteamUser, const SlotInfo& info, Parameters... parameters) noexcept {
    return call_slot<Return>(hSteamUser, info, parameters...);
}

// ---------------------------------------------------------------------------
//  What the generated file provides.
// ---------------------------------------------------------------------------

// The object to hand out for a version string, or null when the stub has no object
// for it - which is a game that asked for something newer than any SDK this was
// imported from. hSteamUser is the handle the game asked under, or 0 when it did not
// say which: the object already holding that handle comes back, otherwise one nobody
// has claimed yet, and the object remembers the handle from then on.
void* interface_object(const char* version, std::int32_t hSteamUser) noexcept;

// Every version string the stub can answer, for a harness that wants to say so.
const InterfaceVersion* interface_versions(std::size_t& count) noexcept;

// ---------------------------------------------------------------------------
//  The payloads a call can be completed with.
// ---------------------------------------------------------------------------
//  The backend answers a call, and may say what should happen to the game next:
//  a registered callback or call result wants one of these, and the layouts file
//  declares each one's fields, the size the SDK's callback pack gives it, and the
//  id a game registers it under. Nothing here reads a payload back - the bytes go
//  into the game's own object.

struct EventInfo {
    const char* name;
    std::size_t size;
    // The SDK's own id for this callback, which is the only thing that can tie a
    // payload nobody asked for to the object that wants it. Two callbacks can be
    // the same size, and one of them is not the one that was meant.
    int callback;
    void (*fill)(const Json& fields, void* buffer) noexcept;
};

// How much room a payload is delivered in: one buffer, on the stack of whichever
// thread the game registered its callback on, so it is bounded rather than sized
// from the file. The generated `api_interfaces.cpp` asserts every payload in the
// layouts against this, so a file that declares a bigger one is a build error and
// not a write past the end - which is what the size only in this number's comment
// used to be worth. `SteamRemotePlayTogetherGuestInvite_t` is the largest the 1.41
// to 1.57 layouts declare, at 1024 bytes.
inline constexpr std::size_t kMaxEventBytes = 4096;

// The payload of that name, or null when the layouts do not declare one - which
// is what a scenario naming an event the data does not have has to be told.
const EventInfo* find_event(const char* name) noexcept;

// ---------------------------------------------------------------------------
//  What the game registered, and what wants one of those payloads.
// ---------------------------------------------------------------------------
//  A game hands its callback object and an id to RegisterCallback, or its call
//  result and the handle it was given to RegisterCallResult, and the SDK is then
//  expected to call that object when the thing it is waiting for happens. That is
//  what this is: the pairs are remembered where they were registered, the reply to
//  a call says which payload it wants delivered, and the game's own RunCallbacks -
//  its pump, on its own thread - is where delivery happens. A game that never
//  pumps is never told anything, which is what the real SDK does too.

void callback_registered(void* object, std::int32_t id) noexcept;
void callback_unregistered(void* object) noexcept;
void call_result_registered(void* object, std::uint64_t call) noexcept;
void call_result_unregistered(void* object, std::uint64_t call) noexcept;

// Hands the game whatever the backend has sent since the last pump.
void deliver_events() noexcept;

// ---------------------------------------------------------------------------
//  The lazy accessor a recent SDK bounces through.
// ---------------------------------------------------------------------------
//  Every per-interface accessor in those headers is an inline function built on
//  SteamInternal_ContextInit:
//
//    inline void SteamInternal_Init_SteamUser( ISteamUser** p ) { *p = ...; }
//    inline ISteamUser* SteamUser() {
//        static void* counter_and_context[3] = { &SteamInternal_Init_SteamUser, 0, 0 };
//        return *(ISteamUser**)SteamInternal_ContextInit( counter_and_context );
//    }
//
//  So the blob is { void (*pFn)(void*); uintptr_t counter; void* value }, which
//  is what the header says and says not to change, and what the function returns
//  is the *address of the third slot* - the game reads its interface pointer out
//  of that. `pFn` is the game's own inline initialiser, so it runs in the
//  caller's code and asks *us* for the interface; that is what makes this
//  answerable here at all.
//
//  The real one re-runs the initialiser when its generation counter says the
//  interfaces were recreated, so a game that shuts Steam down and starts it
//  again gets fresh pointers. Ours are singletons for the life of the process,
//  so a cached pointer is always still valid and running it once is enough.
//
//  The backend is asked once, when the initialiser is about to run, so a
//  transcript still shows that the game asked for the interface and what a
//  scenario said about it. What the game ends up with comes from its own
//  initialiser below - which asks us in turn, through the accessor's expression -
//  so the answer here is for the transcript rather than for the caller. The calls
//  after the first are the game's accessor checking the SDK's cache, once per use
//  of an interface, and those are not reported: the cache lives inside the DLL,
//  and the real one does not treat them as calls either.
inline void* context_init(void* p_context_init_data, const char* call) noexcept {
    struct ContextInitData {
        void (*initialize)(void* value);
        std::uintptr_t counter;
        void* value;
    };

    auto* context = static_cast<ContextInitData*>(p_context_init_data);
    if (context == nullptr) {
        return nullptr;
    }
    if (context->counter != 0 || context->initialize == nullptr) {
        return &context->value;
    }

    Json args = Json::object();
    Json reply;
    args["pContextInitData"] = arg_pointer(p_context_init_data);
    (void)invoke(call, args, reply);

    context->counter = 1;
    context->initialize(&context->value);
    return &context->value;
}

}  // namespace steammock
