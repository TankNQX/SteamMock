#include "bridge/json.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace steambridge {
namespace {

void append_escaped(std::string& out, std::string_view text) {
    out.push_back('"');
    for (const char ch : text) {
        const unsigned char byte = static_cast<unsigned char>(ch);
        switch (ch) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b"; break;
            case '\f': out += "\\f"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if (byte < 0x20u) {
                    char escape[8] = {};
                    std::snprintf(escape, sizeof(escape), "\\u%04X", byte);
                    out += escape;
                } else {
                    out.push_back(ch);
                }
                break;
        }
    }
    out.push_back('"');
}

// JSON has no way to spell a non-finite number, and "0" keeps a transcript
// readable instead of producing something no reader can parse.
//
// The value is written with the fewest digits that still read back as the exact
// same double, so a duration of 164 microseconds is "0.164" in a transcript
// rather than "0.16400000000000001". Two shapes are tried - scientific, and a
// plain decimal - and the shorter one wins, so a duration of one and a half
// seconds reads as "1500" rather than "1.5e+03". Either way the text is only
// accepted when strtod() gives the identical double back, so nothing is ever
// rounded away.
void append_double(std::string& out, double value) {
    if (!std::isfinite(value)) {
        out += "0";
        return;
    }

    std::string chosen;
    for (int precision = 1; precision <= 17; ++precision) {
        char buffer[64] = {};
        std::snprintf(buffer, sizeof(buffer), "%.*g", precision, value);
        if (std::strtod(buffer, nullptr) == value) {
            chosen = buffer;
            break;
        }
    }
    for (int decimals = 0; decimals <= 17; ++decimals) {
        char buffer[64] = {};
        std::snprintf(buffer, sizeof(buffer), "%.*f", decimals, value);
        if (std::strtod(buffer, nullptr) == value) {
            // A tie goes to the decimal form: "1500" reads better than "1.5e+03",
            // and a value too large for the buffer never matches here at all.
            if (chosen.empty() || std::strlen(buffer) <= chosen.size()) {
                chosen = buffer;
            }
            break;
        }
    }

    // A locale that writes a decimal comma would otherwise produce invalid JSON.
    for (char& ch : chosen) {
        if (ch == ',') {
            ch = '.';
        }
    }
    out += chosen;
}

void append_utf8(std::string& out, std::uint32_t code_point) {
    if (code_point <= 0x7Fu) {
        out.push_back(static_cast<char>(code_point));
    } else if (code_point <= 0x7FFu) {
        out.push_back(static_cast<char>(0xC0u | (code_point >> 6)));
        out.push_back(static_cast<char>(0x80u | (code_point & 0x3Fu)));
    } else if (code_point <= 0xFFFFu) {
        out.push_back(static_cast<char>(0xE0u | (code_point >> 12)));
        out.push_back(static_cast<char>(0x80u | ((code_point >> 6) & 0x3Fu)));
        out.push_back(static_cast<char>(0x80u | (code_point & 0x3Fu)));
    } else {
        out.push_back(static_cast<char>(0xF0u | (code_point >> 18)));
        out.push_back(static_cast<char>(0x80u | ((code_point >> 12) & 0x3Fu)));
        out.push_back(static_cast<char>(0x80u | ((code_point >> 6) & 0x3Fu)));
        out.push_back(static_cast<char>(0x80u | (code_point & 0x3Fu)));
    }
}

// A strict recursive descent parser for the protocol's subset. Strict on
// purpose: a half-read frame would silently corrupt a call, and this is the
// only untrusted input the stub ever sees.
class Parser {
public:
    explicit Parser(std::string_view text) noexcept : _text(text) {}

    bool parse(Json& out) {
        skip_whitespace();
        if (!parse_value(out, 0)) {
            return false;
        }
        skip_whitespace();
        return _position == _text.size();
    }

private:
    static const int kMaxDepth = 16;

    bool parse_value(Json& out, int depth) {
        if (depth > kMaxDepth || _position >= _text.size()) {
            return false;
        }
        switch (_text[_position]) {
            case '{': return parse_object(out, depth);
            case '[': return parse_array(out, depth);
            case '"': {
                std::string text;
                if (!parse_string(text)) {
                    return false;
                }
                out = Json::string(std::move(text));
                return true;
            }
            case 't': return literal("true", Json::boolean(true), out);
            case 'f': return literal("false", Json::boolean(false), out);
            case 'n': return literal("null", Json::null(), out);
            default: return parse_number(out);
        }
    }

    bool literal(std::string_view text, Json value, Json& out) {
        if (_text.compare(_position, text.size(), text) != 0) {
            return false;
        }
        _position += text.size();
        out = std::move(value);
        return true;
    }

    bool parse_object(Json& out, int depth) {
        ++_position;  // '{'
        out = Json::object();
        skip_whitespace();
        if (consume('}')) {
            return true;
        }
        for (;;) {
            skip_whitespace();
            std::string key;
            if (!parse_string(key)) {
                return false;
            }
            skip_whitespace();
            if (!consume(':')) {
                return false;
            }
            skip_whitespace();
            Json value;
            if (!parse_value(value, depth + 1)) {
                return false;
            }
            out.set(std::move(key), std::move(value));
            skip_whitespace();
            if (consume(',')) {
                continue;
            }
            return consume('}');
        }
    }

    bool parse_array(Json& out, int depth) {
        ++_position;  // '['
        out = Json::array();
        skip_whitespace();
        if (consume(']')) {
            return true;
        }
        for (;;) {
            skip_whitespace();
            Json value;
            if (!parse_value(value, depth + 1)) {
                return false;
            }
            out.push(std::move(value));
            skip_whitespace();
            if (consume(',')) {
                continue;
            }
            return consume(']');
        }
    }

    bool parse_string(std::string& out) {
        if (!consume('"')) {
            return false;
        }
        out.clear();
        while (_position < _text.size()) {
            const char ch = _text[_position++];
            if (ch == '"') {
                return true;
            }
            if (ch != '\\') {
                out.push_back(ch);
                continue;
            }
            if (_position >= _text.size()) {
                return false;
            }
            const char escape = _text[_position++];
            switch (escape) {
                case '"': out.push_back('"'); break;
                case '\\': out.push_back('\\'); break;
                case '/': out.push_back('/'); break;
                case 'b': out.push_back('\b'); break;
                case 'f': out.push_back('\f'); break;
                case 'n': out.push_back('\n'); break;
                case 'r': out.push_back('\r'); break;
                case 't': out.push_back('\t'); break;
                case 'u': {
                    std::uint32_t code_point = 0;
                    if (!parse_hex4(code_point)) {
                        return false;
                    }
                    // Surrogate pair: a lone half becomes U+FFFD rather than
                    // invalid UTF-8, because a transcript should stay readable.
                    if (code_point >= 0xD800u && code_point <= 0xDBFFu) {
                        const std::size_t save = _position;
                        std::uint32_t low = 0;
                        if (consume('\\') && consume('u') && parse_hex4(low) && low >= 0xDC00u &&
                            low <= 0xDFFFu) {
                            code_point =
                                0x10000u + ((code_point - 0xD800u) << 10) + (low - 0xDC00u);
                        } else {
                            _position = save;
                            code_point = 0xFFFDu;
                        }
                    } else if (code_point >= 0xDC00u && code_point <= 0xDFFFu) {
                        code_point = 0xFFFDu;
                    }
                    append_utf8(out, code_point);
                    break;
                }
                default: return false;
            }
        }
        return false;  // unterminated
    }

    bool parse_hex4(std::uint32_t& out) {
        if (_position + 4u > _text.size()) {
            return false;
        }
        std::uint32_t value = 0;
        for (int i = 0; i < 4; ++i) {
            const char ch = _text[_position++];
            value <<= 4;
            if (ch >= '0' && ch <= '9') {
                value |= static_cast<std::uint32_t>(ch - '0');
            } else if (ch >= 'a' && ch <= 'f') {
                value |= static_cast<std::uint32_t>(ch - 'a' + 10);
            } else if (ch >= 'A' && ch <= 'F') {
                value |= static_cast<std::uint32_t>(ch - 'A' + 10);
            } else {
                return false;
            }
        }
        out = value;
        return true;
    }

    // Numbers are accumulated by hand so the stub's parsing never depends on the
    // process locale, and so an integer stays an integer to the last digit.
    bool parse_number(Json& out) {
        const std::size_t start = _position;
        bool negative = consume('-');
        if (_position >= _text.size() || !is_digit(_text[_position])) {
            _position = start;
            return false;
        }
        while (_position < _text.size() && is_digit(_text[_position])) {
            ++_position;
        }
        bool integral = true;
        double fraction = 0.0;
        double scale = 1.0;
        if (consume('.')) {
            integral = false;
            if (_position >= _text.size() || !is_digit(_text[_position])) {
                return false;
            }
            while (_position < _text.size() && is_digit(_text[_position])) {
                scale *= 10.0;
                fraction += static_cast<double>(_text[_position] - '0') / scale;
                ++_position;
            }
        }
        int exponent = 0;
        if (_position < _text.size() && (_text[_position] == 'e' || _text[_position] == 'E')) {
            integral = false;
            ++_position;
            bool exponent_negative = consume('-');
            if (!exponent_negative) {
                consume('+');
            }
            if (_position >= _text.size() || !is_digit(_text[_position])) {
                return false;
            }
            while (_position < _text.size() && is_digit(_text[_position])) {
                if (exponent < 100000) {
                    exponent = exponent * 10 + (_text[_position] - '0');
                }
                ++_position;
            }
            if (exponent_negative) {
                exponent = -exponent;
            }
        }

        const std::string_view digits = _text.substr(start, _position - start);
        if (integral) {
            std::int64_t value = 0;
            std::size_t index = negative ? 1u : 0u;
            bool overflowed = false;
            for (; index < digits.size(); ++index) {
                if (value > (9223372036854775807LL - (digits[index] - '0')) / 10) {
                    overflowed = true;
                    break;
                }
                value = value * 10 + (digits[index] - '0');
            }
            if (!overflowed) {
                out = Json::integer(negative ? -value : value);
                return true;
            }
            // Too large for int64: fall through and keep it as a double, which
            // is what a JSON number beyond 2^63 means anywhere else.
        }

        double magnitude = 0.0;
        for (std::size_t index = negative ? 1u : 0u; index < digits.size(); ++index) {
            const char ch = digits[index];
            if (!is_digit(ch)) {
                break;
            }
            magnitude = magnitude * 10.0 + static_cast<double>(ch - '0');
        }
        double value = magnitude + fraction;
        if (exponent > 0) {
            for (int i = 0; i < exponent; ++i) {
                value *= 10.0;
            }
        } else {
            for (int i = 0; i < -exponent; ++i) {
                value /= 10.0;
            }
        }
        out = Json::real(negative ? -value : value);
        return true;
    }

    static bool is_digit(char ch) noexcept { return ch >= '0' && ch <= '9'; }

    void skip_whitespace() noexcept {
        while (_position < _text.size()) {
            const char ch = _text[_position];
            if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
                ++_position;
            } else {
                break;
            }
        }
    }

    bool consume(char expected) noexcept {
        if (_position < _text.size() && _text[_position] == expected) {
            ++_position;
            return true;
        }
        return false;
    }

    std::string_view _text;
    std::size_t _position = 0;
};

}  // namespace

Json Json::boolean(bool value) noexcept {
    Json json;
    json._kind = Kind::boolean;
    json._bool = value;
    return json;
}

Json Json::integer(std::int64_t value) noexcept {
    Json json;
    json._kind = Kind::number;
    json._integral = true;
    json._integer = value;
    json._number = static_cast<double>(value);
    return json;
}

Json Json::real(double value) noexcept {
    Json json;
    json._kind = Kind::number;
    json._integral = false;
    json._number = value;
    json._integer = static_cast<std::int64_t>(value);
    return json;
}

Json Json::string(std::string value) {
    Json json;
    json._kind = Kind::string;
    json._string = std::move(value);
    return json;
}

Json Json::array() {
    Json json;
    json._kind = Kind::array;
    return json;
}

Json Json::object() {
    Json json;
    json._kind = Kind::object;
    return json;
}

std::int64_t Json::as_int64() const noexcept {
    return _integral ? _integer : static_cast<std::int64_t>(_number);
}

std::uint64_t Json::as_uint64() const noexcept {
    return _integral ? static_cast<std::uint64_t>(_integer) : static_cast<std::uint64_t>(_number);
}

Json& Json::set(std::string key, Json value) {
    for (auto& member : _members) {
        if (member.first == key) {
            member.second = std::move(value);
            return *this;
        }
    }
    _members.emplace_back(std::move(key), std::move(value));
    return *this;
}

const Json* Json::find(std::string_view key) const noexcept {
    for (const auto& member : _members) {
        if (member.first == key) {
            return &member.second;
        }
    }
    return nullptr;
}

void Json::push(Json value) { _items.push_back(std::move(value)); }

std::string Json::dump() const {
    std::string out;
    switch (_kind) {
        case Kind::null_value: out += "null"; break;
        case Kind::boolean: out += _bool ? "true" : "false"; break;
        case Kind::number:
            if (_integral) {
                out += std::to_string(_integer);
            } else {
                append_double(out, _number);
            }
            break;
        case Kind::string: append_escaped(out, _string); break;
        case Kind::array: {
            out.push_back('[');
            for (std::size_t i = 0; i < _items.size(); ++i) {
                if (i != 0u) {
                    out.push_back(',');
                }
                out += _items[i].dump();
            }
            out.push_back(']');
            break;
        }
        case Kind::object: {
            out.push_back('{');
            for (std::size_t i = 0; i < _members.size(); ++i) {
                if (i != 0u) {
                    out.push_back(',');
                }
                append_escaped(out, _members[i].first);
                out.push_back(':');
                out += _members[i].second.dump();
            }
            out.push_back('}');
            break;
        }
    }
    return out;
}

bool Json::parse(std::string_view text, Json& out) {
    Parser parser(text);
    return parser.parse(out);
}

}  // namespace steambridge
