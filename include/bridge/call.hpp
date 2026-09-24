#pragma once

#include <cstdint>
#include <string>

#include "bridge/client.hpp"
#include "bridge/export.hpp"
#include "bridge/json_read.hpp"

// ---------------------------------------------------------------------------
//  Helpers the generated trampolines use.
// ---------------------------------------------------------------------------
//  Nothing here is API specific: the generator writes one function per IDL
//  entry, and these are the four shapes it needs - build an argument object,
//  send it, read a typed return, write an out-parameter back. Keeping the
//  per-call code in the generated file means the stub carries no runtime
//  description of the API and pays nothing per call for one.

namespace steammock {

inline Json arg_bool(bool value) noexcept { return Json(value); }

inline Json arg_int(std::int64_t value) noexcept { return Json(value); }

inline Json arg_uint(std::uint64_t value) noexcept {
    return Json(static_cast<std::int64_t>(value));
}

inline Json arg_real(double value) noexcept { return Json(value); }

inline Json arg_pointer(const void* value) noexcept {
    return Json(static_cast<std::int64_t>(reinterpret_cast<std::uintptr_t>(value)));
}

inline Json arg_cstring(const char* value) { return value != nullptr ? Json(value) : Json(); }

inline bool reply_bool(const Json& reply) noexcept {
    const Json* ret = json_member(reply, "ret");
    return ret != nullptr && as_bool(*ret);
}

inline std::int64_t reply_int(const Json& reply) noexcept {
    const Json* ret = json_member(reply, "ret");
    return ret != nullptr ? as_int64(*ret) : 0;
}

inline std::uint64_t reply_uint(const Json& reply) noexcept {
    const Json* ret = json_member(reply, "ret");
    return ret != nullptr ? as_uint64(*ret) : 0;
}

inline double reply_real(const Json& reply) noexcept {
    const Json* ret = json_member(reply, "ret");
    return ret != nullptr ? as_double(*ret) : 0.0;
}

// An interface pointer the backend handed back, as an opaque token that the game
// will pass straight into other calls - and that those calls send back to the
// backend unchanged.
inline void* reply_pointer(const Json& reply) noexcept {
    const Json* ret = json_member(reply, "ret");
    if (ret == nullptr) {
        return nullptr;
    }
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(as_uint64(*ret)));
}

// A string a game receives has to outlive the call, and the reply object dies
// with it, so the text is copied into a per-thread buffer and that is what the
// caller gets. It stays valid until the next call on the same thread, which
// covers the way these are used in practice (copied, compared or printed
// immediately); a game that keeps the pointer for later would be reading its own
// next call's text, so this is documented rather than hidden.
inline const char* reply_cstring(const Json& reply, const char* fallback = "") {
    const Json* ret = json_member(reply, "ret");
    if (ret == nullptr || !ret->is_string()) {
        return fallback;
    }
    static thread_local std::string buffer;
    buffer = as_string(*ret);
    return buffer.c_str();
}

// Returns nullptr when the backend did not send this out-parameter, in which
// case the value the game passed in is left exactly as it was - that is what
// makes "answer only the calls you care about" work for out-parameters too.
inline const Json* reply_out(const Json& reply, const char* name) noexcept {
    const Json* out = json_member(reply, "out");
    return out != nullptr ? json_member(*out, name) : nullptr;
}

}  // namespace steammock
