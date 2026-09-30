// ============================================================================
//  C++ unit tests: reading gen/steam_interfaces.json.
// ----------------------------------------------------------------------------
//  The layouts are hand-maintained data, and what this reads them into decides
//  the vtable a game calls through - so the reading is the one thing that has to
//  be exact, and what it refuses is what keeps a hand edit from becoming a call
//  the game reads the wrong way. Both are pinned here: the row format, and the
//  mistakes it has to catch rather than pass through.
//
//  The real file is read by `generated_files_are_current` (through
//  steammock_codegen --check), which is what says these rules still fit it.
//
//  Exits non-zero if a check fails.
// ============================================================================

#include <cstdio>
#include <exception>
#include <string>

#include "bridge/interfaces.hpp"
#include "bridge/json_read.hpp"

namespace {

int g_failures = 0;

void check(const char* what, bool ok) {
    std::printf("  [%s] %s\n", ok ? "ok  " : "FAIL", what);
    if (!ok) {
        ++g_failures;
    }
}

// One version with the slots under test, plus a value class and a structure for
// a row to name - the two kinds of type a row can write instead of a kind.
std::string document(const std::string& slots) {
    return std::string("{\"value_types\":[{\"name\":\"CGameID\",\"wire\":\"uint64\","
                       "\"member\":\"m_gameid\",\"size\":8}],"
                       "\"structures\":[{\"name\":\"Motion_t\",\"size\":40,"
                       "\"members\":[[\"float\",\"x\"]]}],"
                       "\"interfaces\":[{\"name\":\"ISteamUser\",\"version\":\"SteamUser020\","
                       "\"slots\":[") +
           slots + "]}]}";
}

bool reads(const std::string& text, steammock::Interfaces& out, std::string& error) {
    steammock::Json parsed;
    if (!steammock::parse(text, parsed)) {
        error = "the test's own JSON does not parse";
        return false;
    }
    return steammock::Interfaces::from_json(parsed, out, error);
}

// Whether the one version in `document(slots)` reads, and what the first slot
// came out as.
bool reads_slots(const std::string& slots, steammock::InterfaceSlot& first, std::string& error) {
    steammock::Interfaces layouts;
    if (!reads(document(slots), layouts, error)) {
        return false;
    }
    if (layouts.versions().empty() || layouts.versions()[0].slots.empty()) {
        error = "no version came out";
        return false;
    }
    first = layouts.versions()[0].slots[0];
    return true;
}

bool refuses(const std::string& slots, std::string& error) {
    steammock::InterfaceSlot slot;
    return !reads_slots(slots, slot, error);
}

void test_rows() {
    std::printf("[:] a row\n");

    steammock::InterfaceSlot slot;
    std::string error;

    check("a method and a kind read", reads_slots("[\"GetSteamID\", \"uint64\"]", slot, error) &&
                                          slot.method == "GetSteamID" && slot.returns == "uint64" &&
                                          slot.returns_cpp == "std::uint64_t");
    check("a call with no name of its own derives the flat one",
          slot.call == "SteamAPI_ISteamUser_GetSteamID");
    check("a void row reads", reads_slots("[\"RunFrame\", \"void\"]", slot, error) &&
                                  slot.returns == "void" && slot.returns_cpp == "void" &&
                                  !slot.destructor);
    check("a value class is a type of its own",
          reads_slots("[\"GetSteamID\", \"CGameID\"]", slot, error) && slot.returns == "value" &&
              slot.returns_decl == "CGameID" && slot.returns_cpp == "CGameID");
    check("a structure is a type of its own",
          reads_slots("[\"GetMotionData\", \"Motion_t\"]", slot, error) &&
              slot.returns == "struct" && slot.returns_decl == "Motion_t");

    check("a parameter reads",
          reads_slots("[\"Set\", \"void\", [[\"nData\", \"int32\"]]]", slot, error) &&
              slot.params.size() == 1u && slot.params[0].name == "nData" &&
              slot.params[0].kind == "int32" && slot.params[0].cpp == "std::int32_t" &&
              !slot.params[0].out);
    check("out makes the declaration a pointer",
          reads_slots("[\"Set\", \"void\", [[\"pData\", \"int32\", \"out\"]]]", slot, error) &&
              slot.params[0].out && slot.params[0].cpp == "std::int32_t*");
    check("a named parameter type reads",
          reads_slots("[\"Set\", \"void\", [[\"gameID\", \"CGameID\"]]]", slot, error) &&
              slot.params[0].kind == "value" && slot.params[0].decl == "CGameID");
    check("unmarshalable is recorded",
          reads_slots("[\"Set\", \"void\", [[\"pchName\", \"cstring\", \"unmarshalable\"]]]", slot,
                      error) &&
              slot.params[0].opaque && !slot.params[0].out);
    check("both flags read together",
          reads_slots("[\"Set\", \"void\", [[\"nData\", \"uint32\", \"out\", \"unmarshalable\"]]]",
                      slot, error) &&
              slot.params[0].out && slot.params[0].opaque);
    check("no parameters at all is a row with two elements",
          reads_slots("[\"GetAppID\", \"uint32\"]", slot, error) && slot.params.empty() &&
              !slot.destructor);
    check("a byte buffer names the parameter carrying its length",
          reads_slots("[\"Send\", \"void\", [[\"pv\", \"bytes\", \"cb\"], [\"cb\", \"int32\"]]]",
                      slot, error) &&
              slot.params.size() == 2u && slot.params[0].kind == "bytes" &&
              slot.params[0].length == "cb");
    // A structure handed back through a pointer the caller owns. The SDK declares most of these
    // as `void *` - steam_api_flat.h has no other way to spell "the caller's struct" - so the
    // file is the only thing that can say it is one structure and which one it is. See
    // SINGLE_STRUCTS in tools/steamworks_sdk_import.py and the store the generator writes for it.
    check("a structure a call fills in reads as that structure",
          reads_slots("[\"Get\", \"bool\", [[\"pEntry\", \"Motion_t\", \"out\"]]]", slot, error) &&
              slot.params.size() == 1u && slot.params[0].kind == "struct" &&
              slot.params[0].decl == "Motion_t" && slot.params[0].cpp == "Motion_t*" &&
              slot.params[0].out);
    // A text buffer is a byte buffer with text in it: a `char *` the caller owns and the
    // parameter that says how long it is, which is the second half of the value rather than
    // something the call sent. `out` is not written on one of these - the kind's name says the
    // buffer is written, which is what keeps the declared type the SDK's own `char *`.
    check("a text buffer names the parameter carrying its length",
          reads_slots("[\"GetName\", \"bool\", [[\"pchValue\", \"out_text\", \"pcbValue\"], "
                      "[\"pcbValue\", \"uint32\", \"out\"]]]",
                      slot, error) &&
              slot.params.size() == 2u && slot.params[0].kind == "out_text" &&
              slot.params[0].cpp == "char*" && !slot.params[0].out &&
              slot.params[0].length == "pcbValue");
    // The same buffer with a length that is the caller's own pointer, which is the two-call
    // shape: a game asks with a null buffer and a count of zero, and is told how much it needs.
    check("a buffer whose length is the caller's pointer reads as one value",
          reads_slots("[\"GetName\", \"bool\", [[\"pchValue\", \"out_text\", \"punValue\"], "
                      "[\"punValue\", \"uint32\", \"out\"]]]",
                      slot, error) &&
              slot.params[1].cpp == "std::uint32_t*" && slot.params[1].out);
    // A list: one named type and the parameter carrying how many of them. The count is not part
    // of the value and is not `out` here either - it is the buffer's, and what the generator
    // writes for it is a length rather than a parameter of the call.
    check(
        "a list reads as the structure it is a list of, with its count",
        reads_slots("[\"GetItems\", \"bool\", [[\"pItems\", \"Motion_t\", \"out\", \"pcbItems\"], "
                    "[\"pcbItems\", \"uint32\", \"out\"]]]",
                    slot, error) &&
            slot.params.size() == 2u && slot.params[0].kind == "struct" &&
            slot.params[0].decl == "Motion_t" && slot.params[0].cpp == "Motion_t*" &&
            slot.params[0].out && slot.params[0].length == "pcbItems");
}

void test_notes() {
    std::printf("[:] the notes a row may carry\n");

    steammock::InterfaceSlot slot;
    std::string error;

    check("a recorded call wins over the derived one",
          reads_slots(
              "[\"GetStat\", \"int32\", [], {\"call\": \"SteamAPI_ISteamUserStats_GetStatInt32\"}]",
              slot, error) &&
              slot.call == "SteamAPI_ISteamUserStats_GetStatInt32");
    check("private is recorded",
          reads_slots("[\"RunFrame\", \"void\", [], {\"private\": true}]", slot, error) &&
              slot.private_api);
    check("an unmarshalable return is recorded",
          reads_slots("[\"GetData\", \"void\", [], {\"unmarshalable\": true}]", slot, error) &&
              slot.returns_unmarshalable);
    check("notes after parameters read",
          reads_slots("[\"Set\", \"void\", [[\"nData\", \"uint32\"]], {\"private\": true}]", slot,
                      error) &&
              slot.private_api && slot.params.size() == 1u);

    check("the destructor slot is a row of its own",
          reads_slots("[\"~\"]", slot, error) && slot.destructor && slot.returns_cpp == "void");
    check(
        "a destructor may record the name the SDK gives it",
        reads_slots("[\"~\", {\"call\": \"SteamAPI_ISteamHTMLSurface_Destruct\"}]", slot, error) &&
            slot.destructor && slot.call == "SteamAPI_ISteamHTMLSurface_Destruct");
}

void test_refusals() {
    std::printf("[:] what a row may not be\n");

    std::string error;

    check("a type that is neither a kind nor a declared type",
          refuses("[\"GetSteamID\", \"uint66\"]", error));
    check("  and it names what it could not place", error.find("uint66") != std::string::npos);
    check("a misspelt declared type", refuses("[\"GetSteamID\", \"CGameId\"]", error));
    check("a flag that is not 'out' or 'unmarshalable'",
          refuses("[\"Set\", \"void\", [[\"nData\", \"uint32\", \"inout\"]]]", error));
    check("an out parameter of a kind the wire cannot carry",
          refuses("[\"Set\", \"void\", [[\"pchName\", \"cstring\", \"out\"]]]", error));
    check("a row with no return type", refuses("[\"GetSteamID\"]", error));
    check("a row that is an object rather than a row",
          refuses("{\"method\": \"GetSteamID\", \"returns\": \"uint64\"}", error));
    check("a parameter that is not a row", refuses("[\"Set\", \"void\", [\"pchName\"]]", error));
    check("a row with more than its four elements",
          refuses("[\"Set\", \"void\", [], {}, 1]", error));
    check("notes that are not an object", refuses("[\"Set\", \"void\", [], \"private\"]", error));
    check("a destructor with a return type", refuses("[\"~\", \"uint32\"]", error));
    check("a version with no slots", refuses("", error));
    // A byte buffer is the bytes and how many of them, and the row says which of its
    // siblings carries the second. Without one the generator wrote `steammock::Bytes{name, }`
    // and the compiler answered - about the generated file rather than about the row.
    check("a byte buffer with no length parameter",
          refuses("[\"Send\", \"void\", [[\"pv\", \"bytes\"]]]", error));
    check("a byte buffer whose length is not a parameter of the call",
          refuses("[\"Send\", \"void\", [[\"pv\", \"bytes\", \"cb\"], [\"cub\", \"int32\"]]]",
                  error));
    check("a text buffer with no length parameter",
          refuses("[\"GetName\", \"bool\", [[\"pchValue\", \"out_text\"]]]", error));
    check("  and it says the length is what it is missing",
          error.find("carrying its length") != std::string::npos);
    // One of a named type is what `out` alone says. A length on one is a list of them, and a
    // list is written through the caller's pointer or not at all.
    check("a named type with a length that is not an out parameter",
          refuses("[\"GetItems\", \"bool\", [[\"pItems\", \"Motion_t\", \"pcbItems\"], "
                  "[\"pcbItems\", \"uint32\"]]]",
                  error));
    // Two halves of one value, so one length and not two.
    check("a buffer that names two length parameters",
          refuses("[\"Send\", \"void\", [[\"pv\", \"bytes\", \"cb\", \"cub\"], "
                  "[\"cb\", \"int32\"], [\"cub\", \"int32\"]]]",
                  error));
    // What the call reads is already in the caller's memory, so a pointer there is a room
    // nothing could report against - and `Bytes` has nowhere to put a number.
    check("a buffer the call reads whose length is a pointer",
          refuses("[\"Send\", \"void\", [[\"pv\", \"bytes\", \"pcb\"], "
                  "[\"pcb\", \"uint32\", \"out\"]]]",
                  error));
}

// What the generator *writes* for a structure, which is the other half of the reading above and
// the half no test has ever looked at directly: the file it renders is compared byte for byte
// against the tree's own copy by `generated_files_are_current`, which says the generator agrees
// with itself but not what it produced. A structure a call fills in has to come out with a way to
// fill it - and one that is only ever reported must not, because a member write nobody reads is a
// warning, and a warning is an error in this build.
void test_what_is_written() {
    std::printf("[:] what the generator writes for a structure\n");

    steammock::Interfaces filled;
    std::string error;
    if (!reads(document("[\"Get\", \"bool\", [[\"pEntry\", \"Motion_t\", \"out\"]]]"), filled,
               error)) {
        check("the filling document reads", false);
        return;
    }
    const std::string written = steammock::render_api_interfaces(filled);
    check("a structure a call fills in is given a store to be filled by",
          written.find("void store_Motion_t(") != std::string::npos);
    check("  and it writes the members the layouts declare, by their kinds",
          written.find("target->x = static_cast<float>(steammock::as_double(*field));") !=
              std::string::npos);
    check("  and the trait for it calls that store",
          written.find("store_Motion_t(target, value);") != std::string::npos);

    steammock::Interfaces reported;
    if (!reads(document("[\"Get\", \"bool\"]"), reported, error)) {
        check("the reporting document reads", false);
        return;
    }
    const std::string plain = steammock::render_api_interfaces(reported);
    check("a structure nothing fills in gets no store",
          plain.find("store_Motion_t") == std::string::npos);
}

// And what it writes for a buffer the caller owns, which is the half of a call the arguments
// carry rather than the signature: the buffer and the thing that says how long it is travel
// together, and the length is the buffer's own - sent, and written back through the caller's
// pointer when the caller passed one.
void test_what_is_written_for_a_buffer() {
    std::printf("[:] what the generator writes for a buffer\n");

    steammock::Interfaces text;
    std::string error;
    if (!reads(document("[\"GetName\", \"bool\", [[\"pchValue\", \"out_text\", \"punSize\"], "
                        "[\"punSize\", \"uint32\", \"out\"]]]"),
               text, error)) {
        check("the text buffer document reads", false);
        return;
    }
    const std::string written = steammock::render_api_interfaces(text);
    check("a text buffer is wrapped with the parameter that says how long it is",
          written.find("steammock::TextOut{pchValue, punSize}") != std::string::npos);
    check("  and the length is sent as the value it points at, not stored",
          written.find("steammock::BufferLength{punSize}") != std::string::npos);
    check("  and the declared type is still the one the SDK declares",
          written.find("bool GetName(char* pchValue, std::uint32_t* punSize)") !=
              std::string::npos);

    steammock::Interfaces list;
    if (!reads(
            document("[\"GetItems\", \"bool\", [[\"pItems\", \"Motion_t\", \"out\", \"punCount\"], "
                     "[\"punCount\", \"uint32\", \"out\"]]]"),
            list, error)) {
        check("the list document reads", false);
        return;
    }
    const std::string arrays = steammock::render_api_interfaces(list);
    check("a list is wrapped with its count",
          arrays.find("steammock::ArrayOut<Motion_t>{pItems, punCount}") != std::string::npos);
    check("  and its structure gets the store a list writes each element with",
          arrays.find("void store_Motion_t(Motion_t* target, const Json& fields)") !=
                  std::string::npos &&
              arrays.find("store_Motion_t(target, value);") != std::string::npos);
    check("  and its count is the buffer's, sent rather than stored",
          arrays.find("steammock::BufferLength{punCount}") != std::string::npos);

    // A length passed by value has nowhere to write an answer, so there is nothing to own and
    // nothing to wrap: it travels as the value it is.
    steammock::Interfaces by_value;
    if (!reads(document("[\"Fill\", \"void\", [[\"pchValue\", \"out_text\", \"cchValue\"], "
                        "[\"cchValue\", \"int32\"]]]"),
               by_value, error)) {
        check("the by-value document reads", false);
        return;
    }
    const std::string values = steammock::render_api_interfaces(by_value);
    check("a length passed by value is the value and nothing else",
          values.find("steammock::TextOut{pchValue, cchValue}") != std::string::npos &&
              values.find("BufferLength") == std::string::npos);
}

void test_document() {
    std::printf("[:] the document\n");

    steammock::Interfaces layouts;
    std::string error;

    check("something that is not an object is refused", !reads("[1, 2]", layouts, error));
    check("a document with no 'interfaces' is refused",
          !reads("{\"value_types\": []}", layouts, error));

    check("the same version twice is refused",
          !reads("{\"interfaces\": ["
                 "{\"name\": \"ISteamUser\", \"version\": \"SteamUser020\", \"slots\": "
                 "[[\"GetSteamID\", \"uint64\"]]},"
                 "{\"name\": \"ISteamUser\", \"version\": \"SteamUser020\", \"slots\": "
                 "[[\"GetSteamID\", \"uint64\"]]}]}",
                 layouts, error));

    check("a version without a name is refused",
          !reads("{\"interfaces\": [{\"version\": \"SteamUser020\", \"slots\": "
                 "[[\"GetSteamID\", \"uint64\"]]}]}",
                 layouts, error));

    // Two version strings, one identifier. `identified()` turns everything that is not
    // alphanumeric into an underscore, so these two name one class, one array of objects and
    // one table entry between them - which the compiler reports as a redefinition in the
    // generated file, where nothing points back at the layouts.
    check("two versions that are one identifier are refused",
          !reads("{\"interfaces\": ["
                 "{\"name\": \"ISteamUser\", \"version\": \"SteamUser1.0\", \"slots\": "
                 "[[\"GetSteamID\", \"uint64\"]]},"
                 "{\"name\": \"ISteamUser\", \"version\": \"SteamUser1_0\", \"slots\": "
                 "[[\"GetSteamID\", \"uint64\"]]}]}",
                 layouts, error));
    check("  and it names both of them", error.find("SteamUser1.0") != std::string::npos &&
                                             error.find("SteamUser1_0") != std::string::npos);

    // A size the generated file declares a structure to be, so it has to be an int the
    // layouts meant: it used to be narrowed with a cast, and a negative or oversized one
    // became a number the file's own declaration no longer matched.
    const std::string one_version = "\"interfaces\": [{\"name\": \"ISteamUser\", "
                                    "\"version\": \"SteamUser020\", \"slots\": "
                                    "[[\"GetSteamID\", \"uint64\"]]}]}";
    check("a structure with a negative size is refused",
          !reads("{\"structures\": [{\"name\": \"Motion_t\", \"size\": -1, "
                 "\"members\": [[\"float\", \"x\"]]}], " +
                     one_version,
                 layouts, error));
    check("a structure whose size does not fit an int is refused",
          !reads("{\"structures\": [{\"name\": \"Motion_t\", \"size\": 2147483648, "
                 "\"members\": [[\"float\", \"x\"]]}], " +
                     one_version,
                 layouts, error));
    check("  and it says the size is what it cannot use", error.find("size") != std::string::npos);

    // The version's own name is what an unrecorded call name is derived from.
    check("two versions, named by their own interfaces",
          reads("{\"interfaces\": ["
                "{\"name\": \"ISteamUser\", \"version\": \"SteamUser020\", \"slots\": "
                "[[\"GetSteamID\", \"uint64\"]]},"
                "{\"name\": \"ISteamUtils\", \"version\": \"SteamUtils009\", \"slots\": "
                "[[\"GetAppID\", \"uint32\"]]}]}",
                layouts, error) &&
              layouts.versions().size() == 2u &&
              layouts.versions()[0].slots[0].call == "SteamAPI_ISteamUser_GetSteamID" &&
              layouts.versions()[1].slots[0].call == "SteamAPI_ISteamUtils_GetAppID");
}

}  // namespace

// An exception escaping `main` terminates the process with no message at all, and the only
// realistic source in a test is a failed allocation - which the checks above can now make
// directly, since one of them builds its document out of a std::string and not only out of
// literals. Report it the way a failing check is reported instead, so ctest's output says what
// happened. The other four test files answer this the same way; this one had nothing that could
// throw in its own body until it did.
int main() {
    try {
        std::printf("[+] SteamMock interface-layout tests\n\n");
        test_rows();
        test_notes();
        test_refusals();
        test_what_is_written();
        test_what_is_written_for_a_buffer();
        test_document();

        if (g_failures == 0) {
            std::printf("\n[+] all checks passed\n");
        } else {
            std::printf("\n[-] %d check(s) FAILED\n", g_failures);
        }
        return g_failures == 0 ? 0 : 1;
    } catch (const std::exception& error) {
        std::printf("\n[-] the test itself threw: %s\n", error.what());
        return 1;
    } catch (...) {
        std::printf("\n[-] the test itself threw something that is not a std::exception\n");
        return 1;
    }
}
