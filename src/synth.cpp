// ---------------------------------------------------------------------------
//  synth.cpp - the one copy of the marshalling every generated slot shares.
// ---------------------------------------------------------------------------
//  The generated interface file declares one class per version string, whose
//  virtuals are the interface's slots in order - so the vtable is the compiler's
//  and with it the calling convention. What a slot does with its arguments is
//  the same for every slot, though, and only the *shape* of them differs, which
//  the compiler works out per signature. This is the part that does not differ:
//  turn packed arguments into a request, send it, hand back what came.
//
//  It lives here rather than in bridge/synth.hpp on purpose. A header would put
//  a copy of it into each of the signatures the generated file declares, which
//  is what made the synthesized objects larger than everything else in the DLL
//  put together - see docs/development.md, "Interface layouts".

#include "bridge/synth.hpp"

#include <map>
#include <mutex>

#include "bridge/log.hpp"

namespace steammock {
namespace {

// One packed argument as the request carries it. The names travel with the
// values rather than inside them, so a kind is only ever a question of how to
// spell the value a declaration already gave us.
Json packed_value(const Arg& argument) {
    switch (argument.wire) {
        case Wire::boolean: return Json::boolean(argument.bits != 0);
        case Wire::integer: return Json::integer(static_cast<std::int64_t>(argument.bits));
        case Wire::real: {
            double number = 0.0;
            std::memcpy(&number, &argument.bits, sizeof(number));
            return Json::real(number);
        }
        case Wire::cstring:
            return argument.bits == 0 ? Json::null()
                                      : Json::string(reinterpret_cast<const char*>(argument.bits));
        case Wire::null_value: break;
    }
    // A value class the wire cannot carry, or an out-parameter nobody passed.
    return Json::null();
}

}  // namespace

bool run_slot(const SlotInfo& info, const Arg* args, std::size_t count, Json& reply) noexcept {
    try {
        Json request = Json::object();
        for (std::size_t index = 0; index < count; ++index) {
            request.set(name_at(info, index), packed_value(args[index]));
        }
        return invoke(info.call, request, reply);
    } catch (...) {
        // Never let an exception cross into the game: a slot that cannot send
        // its call is a slot nobody answered, which is what false means here.
        return false;
    }
}

// ---------------------------------------------------------------------------
//  What the game registered, and what wants one of those payloads.
// ---------------------------------------------------------------------------
//  RegisterCallback hands over a callback object and an id, RegisterCallResult an
//  object and the call handle the game was given: both are remembered here, under
//  a lock, because a game may register from one thread and pump from another.
//  Delivery happens in RunCallbacks - the game's own thread, inside the game's own
//  call - so nothing here runs on a thread the game does not know about.

namespace {

constexpr std::size_t kMaxEventsPerPump = 64;

std::mutex& registry_mutex() noexcept {
    static std::mutex mutex;
    return mutex;
}

std::map<std::int32_t, void*>& callbacks_by_id() noexcept {
    static std::map<std::int32_t, void*> by_id;
    return by_id;
}

std::map<std::uint64_t, void*>& results_by_call() noexcept {
    static std::map<std::uint64_t, void*> by_call;
    return by_call;
}

// A vtable slot holds a member function, so on x86 it takes `this` in ECX and pops
// its own arguments - a plain function pointer would be the caller's convention
// and would corrupt the stack. On x64 there is one convention and nothing to say.
#if defined(_M_IX86)
#    define STEAMMOCK_MEMBER_CALL __thiscall
#else
#    define STEAMMOCK_MEMBER_CALL
#endif

// CCallbackBase is Run(pvParam) first and Run(pvParam, bIOFailure, hSteamAPICall)
// second: the SDK exports that order and other people link against it, so a call
// result is the second slot and a plain callback the first.
using RunFunction = void(STEAMMOCK_MEMBER_CALL*)(void* self, void* payload, bool io_failure,
                                                 std::uint64_t call);

void call_object(void* object, const EventInfo& event, const Json* fields,
                 std::uint64_t call) noexcept {
    // Zeroed, and larger than any payload this declares: a layout that is wrong
    // then reads zeros rather than past what the stub wrote.
    alignas(std::uint64_t) unsigned char payload[64] = {};
    if (fields != nullptr) {
        event.fill(*fields, payload);
    }

    void* const* const vtable = *reinterpret_cast<void* const* const*>(object);
    if (vtable == nullptr || vtable[1] == nullptr) {
        return;
    }
    const RunFunction run = reinterpret_cast<RunFunction>(vtable[1]);
    run(object, payload, false, call);
}

// One event, as the backend spells it: the payload's name, and either the call it
// completes or the callback id it belongs to.
void deliver_one(const Json& event) noexcept {
    const Json* name = event.find("event");
    if (name == nullptr || !name->is_string()) {
        return;
    }
    const EventInfo* info = find_event(name->as_string().c_str());
    if (info == nullptr) {
        log_write(LogLevel::warn, "an event the layouts do not declare: " + name->as_string());
        return;
    }

    const Json* fields = event.find("in");
    void* object = nullptr;
    std::uint64_t call = 0;
    {
        const std::lock_guard<std::mutex> lock(registry_mutex());
        const Json* handle = event.find("call");
        const Json* id = event.find("id");
        if (handle != nullptr && handle->is_number()) {
            call = handle->as_uint64();
            const auto found = results_by_call().find(call);
            if (found != results_by_call().end()) {
                object = found->second;
            }
        } else if (id != nullptr && id->is_number()) {
            const auto found = callbacks_by_id().find(static_cast<std::int32_t>(id->as_int64()));
            if (found != callbacks_by_id().end()) {
                object = found->second;
            }
        }
    }

    if (object == nullptr) {
        // Nobody is waiting: a result the game never registered, or one it has
        // already unregistered. The real SDK drops those too.
        return;
    }
    call_object(object, *info, fields, call);
}

}  // namespace

void callback_registered(void* object, std::int32_t id) noexcept {
    try {
        const std::lock_guard<std::mutex> lock(registry_mutex());
        callbacks_by_id()[id] = object;
    } catch (...) {
        // A registry that cannot grow is a game that gets no callbacks, which is
        // what it gets with no backend at all.
    }
}

void callback_unregistered(void* object) noexcept {
    try {
        const std::lock_guard<std::mutex> lock(registry_mutex());
        // By object, because that is all UnregisterCallback is given: the id lives
        // in the object the game handed us, and it is not ours to read.
        for (auto entry = callbacks_by_id().begin(); entry != callbacks_by_id().end();) {
            if (entry->second == object) {
                entry = callbacks_by_id().erase(entry);
            } else {
                ++entry;
            }
        }
    } catch (...) {
    }
}

void call_result_registered(void* object, std::uint64_t call) noexcept {
    try {
        const std::lock_guard<std::mutex> lock(registry_mutex());
        results_by_call()[call] = object;
    } catch (...) {
    }
}

void call_result_unregistered(void* object, std::uint64_t call) noexcept {
    try {
        const std::lock_guard<std::mutex> lock(registry_mutex());
        const auto found = results_by_call().find(call);
        if (found != results_by_call().end() && found->second == object) {
            results_by_call().erase(found);
        }
    } catch (...) {
    }
}

void deliver_events() noexcept {
    try {
        // Bounded, so a scenario that queued a burst drains in one pump and a game
        // that is not pumping cannot be walked over a queue that keeps growing.
        Json event;
        for (std::size_t delivered = 0; delivered < kMaxEventsPerPump; ++delivered) {
            if (!Client::instance().take_event(event)) {
                return;
            }
            deliver_one(event);
        }
    } catch (...) {
    }
}

}  // namespace steammock
