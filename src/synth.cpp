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

}  // namespace steammock
