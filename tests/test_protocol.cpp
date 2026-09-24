// ============================================================================
//  C++ unit tests: the JSON subset and the frame header.
// ----------------------------------------------------------------------------
//  Both ends of the bridge have to agree on these exactly - a mismatch shows up
//  as a game hanging or a call silently going missing - so they are pinned here,
//  and cross-checked against the server by the end-to-end test.
//
//  Exits non-zero if a check fails.
// ============================================================================

#include <cstdint>
#include <cstdio>
#include <string>

#include "bridge/frame.hpp"
#include "bridge/json_read.hpp"

namespace {

int g_failures = 0;

void check(const char* what, bool ok) {
    std::printf("  [%s] %s\n", ok ? "ok  " : "FAIL", what);
    if (!ok) {
        ++g_failures;
    }
}

bool reparses(const steammock::Json& value, steammock::Json& out) {
    return steammock::parse(value.dump(), out);
}

void test_numbers() {
    std::printf("[:] numbers\n");

    // A Steam id is 17 digits: it has to survive exactly, which is why the
    // parser keeps integers away from doubles.
    steammock::Json steam_id = steammock::Json(76561198000000001LL);
    steammock::Json back;
    check("a 64-bit id survives the round trip",
          reparses(steam_id, back) && steammock::as_uint64(back) == 76561198000000001ull);

    steammock::Json negative = steammock::Json(-1234567890123LL);
    check("a negative integer survives",
          reparses(negative, back) && steammock::as_int64(back) == -1234567890123LL);

    steammock::Json zero = steammock::Json(0);
    check("zero survives", reparses(zero, back) && steammock::as_int64(back) == 0);

    steammock::Json real = steammock::Json(0.5);
    check("a fraction survives", reparses(real, back) && steammock::as_double(back) == 0.5);

    steammock::Json exponent;
    check("an exponent is understood",
          steammock::parse("1.5e3", exponent) && steammock::as_double(exponent) == 1500.0);

    steammock::Json parsed_int;
    check("a parsed integer is still an integer",
          steammock::parse("42", parsed_int) && parsed_int.dump() == "42");

    steammock::Json boolean = steammock::Json(true);
    check("a boolean survives", reparses(boolean, back) && steammock::as_bool(back));
}

void test_booleans() {
    std::printf("[:] booleans\n");

    steammock::Json value;
    check("true reads as true", steammock::parse("true", value) && steammock::as_bool(value));
    check("false reads as false", steammock::parse("false", value) && !steammock::as_bool(value));
    // A hand-written scenario may spell them as numbers.
    check("1 reads as true", steammock::parse("1", value) && steammock::as_bool(value));
    check("0 reads as false", steammock::parse("0", value) && !steammock::as_bool(value));
    check("a boolean dumps as a literal", steammock::Json(true).dump() == "true");
    check("a boolean is not a number node", steammock::parse("true", value) && !value.is_number());
}

void test_strings() {
    std::printf("[:] strings\n");

    const std::string nasty = "quote:\" backslash:\\ newline:\n tab:\t control:\x01";
    steammock::Json text = steammock::Json(nasty);
    steammock::Json back;
    check("escapes survive the round trip",
          reparses(text, back) && steammock::as_string(back) == nasty);
    check("the dump is one line and has no raw control bytes",
          text.dump().find('\n') == std::string::npos &&
              text.dump().find('\x01') == std::string::npos);

    steammock::Json utf8;
    // Written as explicit bytes rather than \u escapes: a narrow \u literal is
    // encoded in the execution character set, so it means different bytes on
    // MSVC (system code page) and on GCC (UTF-8). The bytes are the contract.
    check("a UTF-8 payload survives",
          steammock::parse("{\"s\":\"caf\xc3\xa9 \xe2\x9c\x93\"}", utf8) &&
              steammock::as_string(*steammock::json_member(utf8, "s")) ==
                  "caf\xc3\xa9 \xe2\x9c\x93");

    steammock::Json escaped;
    check("a \\u escape becomes UTF-8",
          steammock::parse("{\"s\":\"\\u00e9\"}", escaped) &&
              steammock::as_string(*steammock::json_member(escaped, "s")) == "\xc3\xa9");

    steammock::Json emoji;
    check("a surrogate pair becomes one code point",
          steammock::parse("{\"s\":\"\\ud83d\\ude00\"}", emoji) &&
              steammock::as_string(*steammock::json_member(emoji, "s")) == "\xf0\x9f\x98\x80");

    steammock::Json empty = steammock::Json("");
    check("an empty string survives", reparses(empty, back) && steammock::as_string(back).empty());

    steammock::Json null_string;
    check("null is distinct from an empty string", steammock::parse("null", null_string) &&
                                                       null_string.is_null() &&
                                                       !null_string.is_string());
}

void test_containers() {
    std::printf("[:] objects and arrays\n");

    steammock::Json object = steammock::Json::object();
    object["type"] = steammock::Json("call");
    object["seq"] = steammock::Json(7);
    steammock::Json args = steammock::Json::object();
    args["self"] = steammock::Json(4660);
    args["pchName"] = steammock::Json("Deaths");
    args["pnData"] = steammock::Json();
    object["args"] = args;

    steammock::Json parsed;
    check("a nested message survives", reparses(object, parsed));
    const steammock::Json* parsed_args = steammock::json_member(parsed, "args");
    check("the nested object is reachable",
          parsed_args != nullptr && parsed_args->is_object() &&
              steammock::as_string(*steammock::json_member(*parsed_args, "pchName")) == "Deaths");
    check("a null member stays null",
          parsed_args != nullptr && steammock::json_member(*parsed_args, "pnData")->is_null());
    check("a missing member is absent", steammock::json_member(parsed, "nope") == nullptr);
    check("members keep their order",
          object.dump().find("\"type\"") < object.dump().find("\"args\""));

    object["seq"] = steammock::Json(8);
    check("setting a member twice replaces it in place",
          steammock::as_int64(*steammock::json_member(object, "seq")) == 8 && object.size() == 3u);

    steammock::Json array = steammock::Json::array();
    array.push_back(steammock::Json(1));
    array.push_back(steammock::Json("two"));
    array.push_back(steammock::Json(false));
    steammock::Json parsed_array;
    check("an array survives", reparses(array, parsed_array) && parsed_array.size() == 3u &&
                                   steammock::as_string(parsed_array[1]) == "two");
    check("the dump is compact", array.dump() == "[1,\"two\",false]");
}

void test_strictness() {
    std::printf("[:] rejection of malformed input\n");

    steammock::Json value;
    check("trailing garbage is rejected", !steammock::parse("{} extra", value));
    check("an unterminated object is rejected", !steammock::parse("{\"a\":1", value));
    check("an unterminated string is rejected", !steammock::parse("\"abc", value));
    check("an unknown escape is rejected", !steammock::parse("\"\\q\"", value));
    check("a bare word is rejected", !steammock::parse("nope", value));
    check("an empty argument is rejected", !steammock::parse("", value));
    // A runaway length prefix is exactly what a confused peer sends.
    const std::string too_deep = std::string(40, '[') + std::string(40, ']');
    check("absurd nesting is rejected", !steammock::parse(too_deep, value));
    check("a valid message is still accepted", steammock::parse("{\"a\":[1,2]}", value));
}

void test_framing() {
    std::printf("[:] framing\n");

    char header[4] = {};
    for (const std::uint32_t length : {0u, 1u, 255u, 256u, 65535u, 0xFFFFFFFFu}) {
        steammock::write_frame_length(header, length);
        if (steammock::read_frame_length(header) != length) {
            check("a length survives the header", false);
            return;
        }
    }
    check("a length survives the header", true);

    steammock::write_frame_length(header, 0x04030201u);
    check("the length is little endian",
          header[0] == 0x01 && header[1] == 0x02 && header[2] == 0x03 && header[3] == 0x04);
    check("the frame limit is the one the server mirrors",
          steammock::kMaxFrameBytes == 4u * 1024u * 1024u);
}

}  // namespace

int main() {
    std::printf("[+] SteamMock protocol tests\n\n");
    test_numbers();
    test_booleans();
    test_strings();
    test_containers();
    test_strictness();
    test_framing();

    if (g_failures == 0) {
        std::printf("\n[+] all checks passed\n");
    } else {
        std::printf("\n[-] %d check(s) FAILED\n", g_failures);
    }
    return g_failures == 0 ? 0 : 1;
}
