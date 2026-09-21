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
#include "bridge/json.hpp"

namespace {

int g_failures = 0;

void check(const char* what, bool ok) {
    std::printf("  [%s] %s\n", ok ? "ok  " : "FAIL", what);
    if (!ok) {
        ++g_failures;
    }
}

bool reparses(const steambridge::Json& value, steambridge::Json& out) {
    return steambridge::Json::parse(value.dump(), out);
}

void test_numbers() {
    std::printf("[:] numbers\n");

    // A Steam id is 17 digits: it has to survive exactly, which is why the
    // parser keeps integers away from doubles.
    steambridge::Json steam_id = steambridge::Json::integer(76561198000000001LL);
    steambridge::Json back;
    check("a 64-bit id survives the round trip",
          reparses(steam_id, back) && back.as_uint64() == 76561198000000001ull);

    steambridge::Json negative = steambridge::Json::integer(-1234567890123LL);
    check("a negative integer survives", reparses(negative, back) && back.as_int64() == -1234567890123LL);

    steambridge::Json zero = steambridge::Json::integer(0);
    check("zero survives", reparses(zero, back) && back.as_int64() == 0);

    steambridge::Json real = steambridge::Json::real(0.5);
    check("a fraction survives", reparses(real, back) && back.as_double() == 0.5);

    steambridge::Json exponent;
    check("an exponent is understood",
          steambridge::Json::parse("1.5e3", exponent) && exponent.as_double() == 1500.0);

    steambridge::Json parsed_int;
    check("a parsed integer is still an integer",
          steambridge::Json::parse("42", parsed_int) && parsed_int.dump() == "42");

    steambridge::Json boolean = steambridge::Json::boolean(true);
    check("a boolean survives", reparses(boolean, back) && back.as_bool());
}

void test_booleans() {
    std::printf("[:] booleans\n");

    steambridge::Json value;
    check("true reads as true", steambridge::Json::parse("true", value) && value.as_bool());
    check("false reads as false", steambridge::Json::parse("false", value) && !value.as_bool());
    // A hand-written scenario may spell them as numbers.
    check("1 reads as true", steambridge::Json::parse("1", value) && value.as_bool());
    check("0 reads as false", steambridge::Json::parse("0", value) && !value.as_bool());
    check("a boolean dumps as a literal", steambridge::Json::boolean(true).dump() == "true");
    check("a boolean is not a number node",
          steambridge::Json::parse("true", value) && !value.is_number());
}

void test_strings() {
    std::printf("[:] strings\n");

    const std::string nasty = "quote:\" backslash:\\ newline:\n tab:\t control:\x01";
    steambridge::Json text = steambridge::Json::string(nasty);
    steambridge::Json back;
    check("escapes survive the round trip", reparses(text, back) && back.as_string() == nasty);
    check("the dump is one line and has no raw control bytes",
          text.dump().find('\n') == std::string::npos && text.dump().find('\x01') == std::string::npos);

    steambridge::Json utf8;
    // Written as explicit bytes rather than \u escapes: a narrow \u literal is
    // encoded in the execution character set, so it means different bytes on
    // MSVC (system code page) and on GCC (UTF-8). The bytes are the contract.
    check("a UTF-8 payload survives",
          steambridge::Json::parse("{\"s\":\"caf\xc3\xa9 \xe2\x9c\x93\"}", utf8) &&
              utf8.find("s")->as_string() == "caf\xc3\xa9 \xe2\x9c\x93");

    steambridge::Json escaped;
    check("a \\u escape becomes UTF-8",
          steambridge::Json::parse("{\"s\":\"\\u00e9\"}", escaped) &&
              escaped.find("s")->as_string() == "\xc3\xa9");

    steambridge::Json emoji;
    check("a surrogate pair becomes one code point",
          steambridge::Json::parse("{\"s\":\"\\ud83d\\ude00\"}", emoji) &&
              emoji.find("s")->as_string() == "\xf0\x9f\x98\x80");

    steambridge::Json empty = steambridge::Json::string("");
    check("an empty string survives", reparses(empty, back) && back.as_string().empty());

    steambridge::Json null_string;
    check("null is distinct from an empty string",
          steambridge::Json::parse("null", null_string) && null_string.is_null() &&
              !null_string.is_string());
}

void test_containers() {
    std::printf("[:] objects and arrays\n");

    steambridge::Json object = steambridge::Json::object();
    object.set("type", steambridge::Json::string("call"));
    object.set("seq", steambridge::Json::integer(7));
    steambridge::Json args = steambridge::Json::object();
    args.set("self", steambridge::Json::integer(4660));
    args.set("pchName", steambridge::Json::string("Deaths"));
    args.set("pnData", steambridge::Json::null());
    object.set("args", args);

    steambridge::Json parsed;
    check("a nested message survives", reparses(object, parsed));
    const steambridge::Json* parsed_args = parsed.find("args");
    check("the nested object is reachable",
          parsed_args != nullptr && parsed_args->is_object() &&
              parsed_args->find("pchName")->as_string() == "Deaths");
    check("a null member stays null",
          parsed_args != nullptr && parsed_args->find("pnData")->is_null());
    check("a missing member is absent", parsed.find("nope") == nullptr);
    check("members keep their order", object.dump().find("\"type\"") < object.dump().find("\"args\""));

    object.set("seq", steambridge::Json::integer(8));
    check("setting a member twice replaces it in place",
          object.find("seq")->as_int64() == 8 && object.members().size() == 3u);

    steambridge::Json array = steambridge::Json::array();
    array.push(steambridge::Json::integer(1));
    array.push(steambridge::Json::string("two"));
    array.push(steambridge::Json::boolean(false));
    steambridge::Json parsed_array;
    check("an array survives",
          reparses(array, parsed_array) && parsed_array.items().size() == 3u &&
              parsed_array.items()[1].as_string() == "two");
    check("the dump is compact", array.dump() == "[1,\"two\",false]");
}

void test_strictness() {
    std::printf("[:] rejection of malformed input\n");

    steambridge::Json value;
    check("trailing garbage is rejected", !steambridge::Json::parse("{} extra", value));
    check("an unterminated object is rejected", !steambridge::Json::parse("{\"a\":1", value));
    check("an unterminated string is rejected", !steambridge::Json::parse("\"abc", value));
    check("an unknown escape is rejected", !steambridge::Json::parse("\"\\q\"", value));
    check("a bare word is rejected", !steambridge::Json::parse("nope", value));
    check("an empty argument is rejected", !steambridge::Json::parse("", value));
    // A runaway length prefix is exactly what a confused peer sends.
    const std::string too_deep = std::string(40, '[') + std::string(40, ']');
    check("absurd nesting is rejected", !steambridge::Json::parse(too_deep, value));
    check("a valid message is still accepted", steambridge::Json::parse("{\"a\":[1,2]}", value));
}

void test_framing() {
    std::printf("[:] framing\n");

    char header[4] = {};
    for (const std::uint32_t length : {0u, 1u, 255u, 256u, 65535u, 0xFFFFFFFFu}) {
        steambridge::write_frame_length(header, length);
        if (steambridge::read_frame_length(header) != length) {
            check("a length survives the header", false);
            return;
        }
    }
    check("a length survives the header", true);

    steambridge::write_frame_length(header, 0x04030201u);
    check("the length is little endian",
          header[0] == 0x01 && header[1] == 0x02 && header[2] == 0x03 && header[3] == 0x04);
    check("the frame limit is the one the server mirrors",
          steambridge::kMaxFrameBytes == 4u * 1024u * 1024u);
}

}  // namespace

int main() {
    std::printf("[+] SteamApiBridge protocol tests\n\n");
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
