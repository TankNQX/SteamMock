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
#include <string>

#include "bridge/interfaces.hpp"
#include "bridge/json.hpp"

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
    if (!steammock::Json::parse(text, parsed)) {
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

int main() {
    std::printf("[+] SteamMock interface-layout tests\n\n");
    test_rows();
    test_notes();
    test_refusals();
    test_document();

    if (g_failures == 0) {
        std::printf("\n[+] all checks passed\n");
    } else {
        std::printf("\n[-] %d check(s) FAILED\n", g_failures);
    }
    return g_failures == 0 ? 0 : 1;
}
