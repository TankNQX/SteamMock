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
