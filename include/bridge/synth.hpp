#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <type_traits>

#include "bridge/call.hpp"
#include "bridge/client.hpp"
#include "bridge/json.hpp"

namespace steambridge {

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

// One interface version the stub can answer, and the object to hand back for it.
struct InterfaceVersion {
    const char* version;
    void* object;
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

enum class Wire : std::uint8_t { null_value, boolean, integer, real, cstring };

struct Arg {
    std::uint64_t bits;
    Wire wire;
};

inline Arg wire_null() noexcept { return {0, Wire::null_value}; }
inline Arg wire_bool(bool value) noexcept { return {value ? 1u : 0u, Wire::boolean}; }

// The two integer spellings differ in the cast at the call site, not here: one
// sign-extends and one zero-extends, and widening a value the wrong way is a
// different number on the wire.
inline Arg wire_int(std::int64_t value) noexcept {
    return {static_cast<std::uint64_t>(value), Wire::integer};
}
inline Arg wire_uint(std::uint64_t value) noexcept { return {value, Wire::integer}; }

inline Arg wire_real(double value) noexcept {
    Arg arg{0, Wire::real};
    std::memcpy(&arg.bits, &value, sizeof(value));
    return arg;
}

// An address travels as the integer it is, which is what the protocol has for a
// handle no side can dereference.
inline Arg wire_pointer(const void* value) noexcept {
    return {reinterpret_cast<std::uint64_t>(value), Wire::integer};
}

inline Arg wire_cstring(const char* value) noexcept {
    return {reinterpret_cast<std::uint64_t>(value), Wire::cstring};
}

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
                  "steambridge::Kind for it");

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
            *target = value.as_bool();
        } else if constexpr (std::is_floating_point_v<T>) {
            *target = static_cast<T>(value.as_double());
        } else if constexpr (std::is_signed_v<T>) {
            *target = static_cast<T>(value.as_int64());
        } else {
            *target = static_cast<T>(value.as_uint64());
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
// into every shape of call the generated file declares.
bool run_slot(const SlotInfo& info, const Arg* args, std::size_t count, Json& reply) noexcept;

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

template <class Return, class... Parameters>
Return slot(const SlotInfo& info, Parameters... parameters) noexcept {
    try {
        const Arg packed[sizeof...(Parameters) + 1] = {Kind<Parameters>::arg(parameters)...};
        Json reply;

        if (!run_slot(info, packed, sizeof...(Parameters), reply)) {
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

// ---------------------------------------------------------------------------
//  What the generated file provides.
// ---------------------------------------------------------------------------

// The object to hand out for a version string, or null when the stub has no
// object for it - which is a game that asked for something newer than any SDK
// this was imported from.
void* interface_object(const char* version) noexcept;

// Every version string the stub can answer, for a harness that wants to say so.
const InterfaceVersion* interface_versions(std::size_t& count) noexcept;

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
    args.set("pContextInitData", arg_pointer(p_context_init_data));
    (void)invoke(call, args, reply);

    context->counter = 1;
    context->initialize(&context->value);
    return &context->value;
}

}  // namespace steambridge
