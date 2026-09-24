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
        case Wire::boolean: return Json(argument.bits != 0);
        case Wire::integer: return Json(static_cast<std::int64_t>(argument.bits));
        case Wire::real: {
            double number = 0.0;
            std::memcpy(&number, &argument.bits, sizeof(number));
            return Json(number);
        }
        case Wire::cstring:
            return argument.bits == 0 ? Json() : Json(reinterpret_cast<const char*>(argument.bits));
        case Wire::null_value: break;
    }
    // A value class the wire cannot carry, or an out-parameter nobody passed.
    return Json();
}

}  // namespace

// The calls a game can make through two objects that answer to the same name, where
// nothing in the arguments says which one it used. A process that hosts has a customer
// and a game server, each with its own ISteamNetworking, and a packet read through one
// of them cannot be told from a packet read through the other by anything else on this
// wire - so these calls carry the user handle they were made through, and the world
// reads the queue of the end that asked. Only these: every other call is answered for
// the interface it names.
bool needs_user_handle(const char* call) noexcept {
    constexpr const char* kPrefix = "SteamAPI_ISteamNetworking_";
    if (call == nullptr) {
        return false;
    }
    for (std::size_t index = 0; kPrefix[index] != '\0'; ++index) {
        if (call[index] != kPrefix[index]) {
            return false;
        }
    }
    return true;
}

bool run_slot(std::int32_t hSteamUser, const SlotInfo& info, const Arg* args, std::size_t count,
              Json& reply) noexcept {
    try {
        Json request = Json::object();
        for (std::size_t index = 0; index < count; ++index) {
            request[name_at(info, index)] = packed_value(args[index]);
        }
        if (hSteamUser != 0 && needs_user_handle(info.call)) {
            // The handle this was called through, under the name the SDK's own accessor
            // calls it by - the same word, because it is the same fact.
            request["hSteamUser"] = Json(hSteamUser);
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
using RunPayloadFunction = void(STEAMMOCK_MEMBER_CALL*)(void* self, void* payload);

// The third slot is the SDK's GetCallbackSizeBytes, which the stub no longer asks
// for: a payload nobody asked for names the callback it belongs to instead of being
// matched by size, because two of them can be the same size and one of them is not
// the one that was meant.
static_assert(sizeof(RunFunction) == sizeof(void*), "a vtable slot is one pointer");

void call_object(void* object, const EventInfo& event, const Json* fields, std::uint64_t call,
                 bool call_result) noexcept {
    // Zeroed, and larger than any payload this declares: a layout that is wrong
    // then reads zeros rather than past what the stub wrote.
    alignas(std::uint64_t) unsigned char payload[64] = {};
    if (fields != nullptr) {
        event.fill(*fields, payload);
    }

    void* const* const vtable = *reinterpret_cast<void* const* const*>(object);
    if (vtable == nullptr) {
        return;
    }
    // Each slot takes its own argument list, and on x86 the callee pops what it was
    // declared with - so a callback gets called as a callback and a call result as
    // a call result, rather than one of them being handed the other's stack.
    if (call_result) {
        if (vtable[1] == nullptr) {
            log_write(LogLevel::warn, "a callback object has no slot to run a call result in");
            return;
        }
        const RunFunction run = reinterpret_cast<RunFunction>(vtable[1]);
        run(object, payload, false, call);
        return;
    }
    if (vtable[0] == nullptr) {
        log_write(LogLevel::warn, "a callback object has no slot to run a callback in");
        return;
    }
    const RunPayloadFunction run = reinterpret_cast<RunPayloadFunction>(vtable[0]);
    run(object, payload);
    // Said after the game's own code has run and returned, so a payload that goes in
    // and does not come back - a handler that never returns - is visible as itself
    // rather than as a missing line somewhere else.
    log_write(LogLevel::debug, "the game returned from a " + std::string(event.name) + " callback");
}

// One event, as the backend spells it: the payload's name, and either the call it
// completes, the callback id it belongs to, or neither - in which case it is
// something that happened to a game rather than an answer to something it asked.
void deliver_one(const Json& event) noexcept {
    const Json* name = json_member(event, "event");
    if (name == nullptr || !name->is_string()) {
        return;
    }
    const EventInfo* info = find_event(as_string(*name).c_str());
    if (info == nullptr) {
        log_write(LogLevel::warn, "an event the layouts do not declare: " + as_string(*name));
        return;
    }

    const Json* fields = json_member(event, "in");
    void* object = nullptr;
    std::uint64_t call = 0;
    bool call_result = false;
    {
        const std::lock_guard<std::mutex> lock(registry_mutex());
        const Json* handle = json_member(event, "call");
        const Json* id = json_member(event, "id");
        if (handle != nullptr && handle->is_number()) {
            call = as_uint64(*handle);
            const auto found = results_by_call().find(call);
            if (found != results_by_call().end()) {
                object = found->second;
                call_result = true;
            }
        } else if (id != nullptr && id->is_number()) {
            const auto found = callbacks_by_id().find(static_cast<std::int32_t>(as_int64(*id)));
            if (found != callbacks_by_id().end()) {
                object = found->second;
                log_write(LogLevel::debug, "delivering " + as_string(*name) +
                                               " to the callback the game registered for id " +
                                               std::to_string(as_int64(*id)));
            }
        } else {
            // Nobody asked for this, because it is not an answer to anything: a room
            // changed, a packet arrived. What ties it to an object is the payload's
            // own name - the SDK's callback id - which is what the game registered
            // under when it said it wanted to hear about this.
            const auto found = callbacks_by_id().find(info->callback);
            if (found != callbacks_by_id().end()) {
                object = found->second;
                log_write(LogLevel::debug, "delivering " + as_string(*name) +
                                               " to the callback registered for id " +
                                               std::to_string(info->callback));
            }
        }
    }

    if (object == nullptr) {
        // Nobody is waiting: a result the game never registered, one it has already
        // unregistered, or something it never asked to hear about. The real SDK
        // drops those too - but it says so, because an event that goes nowhere is
        // the hardest kind of silence.
        log_write(LogLevel::warn,
                  "an event nobody is waiting for: " + as_string(*name) +
                      (call_result ? std::string()
                                   : " (callback " + std::to_string(info->callback) + ")"));
        return;
    }
    call_object(object, *info, fields, call, call_result);
}

}  // namespace

void callback_registered(void* object, std::int32_t id) noexcept {
    try {
        const std::lock_guard<std::mutex> lock(registry_mutex());
        // Said out loud, because this is the one path in all of this with no other line - and
        // because one id can have two objects on it. A process that runs a game server inside
        // a game has both sides registering callbacks, and this map keeps only the last of
        // them, so which one it is decides which side hears about an event. The addresses are
        // here to tell the two apart: they are the only thing about them we can see.
        const auto found = callbacks_by_id().find(id);
        const std::string address = std::to_string(reinterpret_cast<std::uintptr_t>(object));
        if (found != callbacks_by_id().end()) {
            log_write(LogLevel::debug,
                      "callback id " + std::to_string(id) + ": keeping " + address + " over " +
                          std::to_string(reinterpret_cast<std::uintptr_t>(found->second)));
        } else {
            log_write(LogLevel::debug, "callback id " + std::to_string(id) + ": first, " + address);
        }
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
            // Said out loud, because a payload that comes out of the queue and then does
            // nothing is the one thing here that is otherwise invisible.
            const Json* name = json_member(event, "event");
            log_write(LogLevel::debug,
                      "taking a payload out of the queue to hand over: " +
                          (name != nullptr && name->is_string() ? as_string(*name)
                                                                : std::string("<no name>")));
            deliver_one(event);
        }
    } catch (...) {
    }
}

}  // namespace steammock
