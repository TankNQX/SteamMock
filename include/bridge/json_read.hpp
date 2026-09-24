#pragma once

#include <nlohmann/json.hpp>

#include <cstdint>
#include <string>
#include <string_view>

namespace steammock {

// ---------------------------------------------------------------------------
//  The JSON the bridge speaks, and how this harness reads a field out of it
// ---------------------------------------------------------------------------
//  nlohmann/json owns the storage, the parser and the writer: it is a submodule
//  under external/, and there is no JSON implementation in this tree any more. What
//  is here is the three things the wire needs on top of it, and nothing else.
//
//  The alias, because `nlohmann::ordered_json` is a mouthful and the *order* of an
//  object's members is part of what this harness is for: a transcript is read in the
//  order a run answered the calls, and nlohmann's plain `json` sorts keys.
//
//  The readers, because every one of them reads a field a game or a backend sent,
//  and a field that is missing, null or the wrong type has to be a default rather
//  than an exception - nlohmann throws, and a stub that throws inside a game's call
//  is a game that dies rather than one that gets an answer. They are named after the
//  accessors they replace, so a reader still says what it wants: as_int64(value).
//
//  And one parse, because nlohmann's has no depth limit: a message with a hundred
//  nested arrays would recurse until the stack gave out, and a confused peer sending
//  one is exactly what the frame length is meant to catch. Sixteen is the depth this
//  harness has always refused beyond.
using Json = nlohmann::ordered_json;

constexpr int kMaxJsonDepth = 16;

// The member named `key`, or nullptr when the object has none - a pointer, because
// that is what a caller can test in an `if` without touching the object's lifetime.
// Named `json_member` rather than `member`: the lobby and server code has members
// of its own, and a name that a local can shadow is a name that will be shadowed.
inline const Json* json_member(const Json& object, std::string_view key) noexcept {
    if (!object.is_object()) {
        return nullptr;
    }
    // Through contains() and at() rather than find()/end(): an ordered_json's own
    // iterators are its ordered_map's, and they do not compare with the json's.
    const std::string name(key);
    if (!object.contains(name)) {
        return nullptr;
    }
    return &object.at(name);
}

inline std::int64_t as_int64(const Json& value, std::int64_t fallback = 0) noexcept {
    if (value.is_number_integer()) {
        return value.get<std::int64_t>();
    }
    if (value.is_number_unsigned()) {
        return static_cast<std::int64_t>(value.get<std::uint64_t>());
    }
    if (value.is_number_float()) {
        return static_cast<std::int64_t>(value.get<double>());
    }
    // A game that sends 1 where a boolean belongs is a game being readable rather
    // than wrong, which is why this is not a type error.
    if (value.is_boolean()) {
        return value.get<bool>() ? 1 : 0;
    }
    return fallback;
}

inline std::uint64_t as_uint64(const Json& value, std::uint64_t fallback = 0) noexcept {
    if (value.is_number_unsigned()) {
        return value.get<std::uint64_t>();
    }
    if (value.is_number_integer()) {
        const std::int64_t number = value.get<std::int64_t>();
        return number < 0 ? fallback : static_cast<std::uint64_t>(number);
    }
    if (value.is_number_float()) {
        return static_cast<std::uint64_t>(value.get<double>());
    }
    if (value.is_boolean()) {
        return value.get<bool>() ? 1u : 0u;
    }
    return fallback;
}

inline double as_double(const Json& value, double fallback = 0.0) noexcept {
    if (value.is_number()) {
        return value.get<double>();
    }
    return fallback;
}

inline bool as_bool(const Json& value, bool fallback = false) noexcept {
    if (value.is_boolean()) {
        return value.get<bool>();
    }
    if (value.is_number()) {
        return value.get<double>() != 0.0;
    }
    return fallback;
}

inline std::string as_string(const Json& value, std::string fallback = std::string()) {
    if (value.is_string()) {
        return value.get<std::string>();
    }
    return fallback;
}

// Strict, and bounded: trailing text, an unterminated string and a bad escape are
// all failures, and so is nesting past kMaxJsonDepth.
inline bool parse(std::string_view text, Json& out) {
    // The depth is checked here because it is the one thing nlohmann does not bound:
    // its parser recurses as deep as the text goes, and a peer that sends a hundred
    // nested arrays is a stack overflow rather than a rejected message.
    int depth = 0;
    bool in_string = false;
    for (std::size_t index = 0; index < text.size(); ++index) {
        const char ch = text[index];
        if (in_string) {
            if (ch == '\\') {
                ++index;
            } else if (ch == '"') {
                in_string = false;
            }
            continue;
        }
        if (ch == '"') {
            in_string = true;
        } else if (ch == '[' || ch == '{') {
            if (++depth > kMaxJsonDepth) {
                return false;
            }
        } else if (ch == ']' || ch == '}') {
            --depth;  // text that closes more than it opens is nlohmann's to reject
        }
    }

    out = Json::parse(text.begin(), text.end(), nullptr, false);
    return !out.is_discarded();
}

}  // namespace steammock
