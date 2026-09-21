#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace steambridge {

// ---------------------------------------------------------------------------
//  Deliberately small JSON.
// ---------------------------------------------------------------------------
//  The bridge has no third party dependencies, and both ends must agree on a
//  format that a human can read in a transcript file, so the protocol is JSON
//  and this is the whole of the parser and writer the stub needs: six kinds, no
//  locale surprises in the numbers, and a strict parser that rejects malformed
//  input instead of half-reading it.
class Json {
public:
    enum class Kind : unsigned char { null_value, boolean, number, string, array, object };

    Json() noexcept = default;

    static Json null() noexcept { return Json{}; }
    static Json boolean(bool value) noexcept;
    static Json integer(std::int64_t value) noexcept;
    static Json real(double value) noexcept;
    static Json string(std::string value);
    static Json array();
    static Json object();

    Kind kind() const noexcept { return _kind; }
    bool is_null() const noexcept { return _kind == Kind::null_value; }
    bool is_number() const noexcept { return _kind == Kind::number; }
    bool is_string() const noexcept { return _kind == Kind::string; }
    bool is_array() const noexcept { return _kind == Kind::array; }
    bool is_object() const noexcept { return _kind == Kind::object; }

    // Tolerant on purpose: a hand-written scenario may spell a boolean as 1 or 0,
    // and reading that as "false" would be a silently wrong answer.
    bool as_bool() const noexcept { return _kind == Kind::boolean ? _bool : (as_int64() != 0); }
    double as_double() const noexcept { return _number; }
    std::int64_t as_int64() const noexcept;
    std::uint64_t as_uint64() const noexcept;
    const std::string& as_string() const noexcept { return _string; }

    const std::vector<Json>& items() const noexcept { return _items; }
    std::vector<Json>& items() noexcept { return _items; }
    const std::vector<std::pair<std::string, Json>>& members() const noexcept { return _members; }

    // Object helpers.
    Json& set(std::string key, Json value);
    const Json* find(std::string_view key) const noexcept;

    // Array helper.
    void push(Json value);

    std::string dump() const;
    static bool parse(std::string_view text, Json& out);

private:
    Kind _kind = Kind::null_value;
    bool _bool = false;
    bool _integral = false;  // numbers: keep integers exact, never via a double round trip
    std::int64_t _integer = 0;
    double _number = 0.0;
    std::string _string;
    std::vector<Json> _items;                            // array items
    std::vector<std::pair<std::string, Json>> _members;  // object members, insertion order
};

}  // namespace steambridge
