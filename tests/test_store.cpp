// ============================================================================
//  C++ unit tests: the state file.
// ----------------------------------------------------------------------------
//  `bridge/store.hpp` makes three promises, and every one of them is a promise
//  about a *second* run: what a game wrote is still there, what only a scenario
//  wrote still answers to the scenario, and nothing is ever deleted for being
//  absent from either. A test that opened one store and read it back would prove
//  none of them, because they are all about what survives an open *and a close* -
//  so most of what is below writes through one store, closes it, and opens the
//  file again to see what the next run would be handed.
//
//  It links SQLite directly, on purpose, for two checks that nothing else can
//  make: that a row written into the file by hand - the thing a database is for -
//  is read back like any other, and that a file which is not one of these is
//  refused rather than written into.
//
//  Exits non-zero if a check fails.
// ============================================================================

#include <cstdint>
#include <cstdio>
#include <exception>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <sqlite3.h>

#include "bridge/inventory.hpp"
#include "bridge/json_read.hpp"
#include "bridge/leaderboard.hpp"
#include "bridge/session.hpp"
#include "bridge/store.hpp"

namespace
{

using steammock::Json;

int g_failures = 0;

void check(const char* what, bool ok)
{
    std::printf("  [%s] %s\n", ok ? "ok  " : "FAIL", what);
    if (!ok)
    {
        ++g_failures;
    }
}

// One state file per test, in the system's own temporary directory. Removed when the test
// that made it is done, and removed before it starts as well - a file left behind by a run
// that failed earlier would otherwise be a run of this test starting from the wrong state,
// which is the one thing a test of persistence cannot afford.
class TempState
{
  public:
    explicit TempState(const char* name)
        : _path(std::filesystem::temp_directory_path() /
                (std::string("steammock-test-") + name + ".sqlite"))
    {
        std::error_code ignored;
        std::filesystem::remove(_path, ignored);
    }

    ~TempState()
    {
        std::error_code ignored;
        std::filesystem::remove(_path, ignored);
    }

    TempState(const TempState&) = delete;
    TempState& operator=(const TempState&) = delete;

    std::string path() const { return _path.string(); }

  private:
    std::filesystem::path _path;
};

// SQL of this file's own, run the way a person at a SQLite prompt would. The point of a
// state file being a database is that somebody can look at it and add to it, so the checks
// that depend on that use the same door they would. It creates the file if it is not there,
// exactly as `sqlite3 <path>` does.
bool run_sql(const std::string& path, const char* sql)
{
    sqlite3* db = nullptr;
    if (sqlite3_open_v2(path.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr) !=
        SQLITE_OK)
    {
        if (db != nullptr)
        {
            sqlite3_close_v2(db);
        }
        return false;
    }
    const int status = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
    sqlite3_close_v2(db);
    return status == SQLITE_OK;
}

steammock::Profile profile_named(const char* name)
{
    steammock::Profile profile;
    profile.name = name;
    return profile;
}

// One value out of the file, read the way a person at a prompt would.
bool read_text(const std::string& path, const char* sql, std::string& out)
{
    sqlite3* db = nullptr;
    if (sqlite3_open_v2(path.c_str(), &db, SQLITE_OPEN_READONLY, nullptr) != SQLITE_OK)
    {
        if (db != nullptr)
        {
            sqlite3_close_v2(db);
        }
        return false;
    }
    sqlite3_stmt* statement = nullptr;
    const bool ok = sqlite3_prepare_v2(db, sql, -1, &statement, nullptr) == SQLITE_OK &&
                    sqlite3_step(statement) == SQLITE_ROW;
    if (ok)
    {
        const unsigned char* text = sqlite3_column_text(statement, 0);
        out = text == nullptr ? std::string() : std::string(reinterpret_cast<const char*>(text));
    }
    if (statement != nullptr)
    {
        sqlite3_finalize(statement);
    }
    sqlite3_close_v2(db);
    return ok;
}

// A profile as an authored scenario would leave it: one stat and one achievement that is
// not earned yet.
steammock::Profile seeded_hero()
{
    steammock::Profile profile = profile_named("hero");
    profile.stats.emplace_back("Deaths", 3);
    profile.achievements.push_back({"ACH_BOOTED", false});
    return profile;
}

steammock::Profile player(std::uint64_t steam_id)
{
    steammock::Profile profile = profile_named("player");
    profile.steam_id = steam_id;
    return profile;
}

// --- the promises ----------------------------------------------------------

void test_a_state_file_keeps_what_a_run_wrote()
{
    std::printf("[:] what a run wrote, the next run reads\n");

    TempState file("keeps");
    std::string error;
    std::unique_ptr<steammock::Store> store = steammock::Store::open(file.path(), error);
    check("a state file is made when there is none", store != nullptr);
    if (store == nullptr)
    {
        std::printf("        %s\n", error.c_str());
        return;
    }
    check("and it has nothing wrong with it", store->error().empty());

    store->seed(seeded_hero());
    // The game plays: it dies nine times and earns the achievement.
    store->write_stat("hero", "Deaths", 9);
    store->write_achievement("hero", "ACH_BOOTED", true);
    check("no write failed", store->error().empty());
    store.reset(); // the run ends

    std::string second_error;
    std::unique_ptr<steammock::Store> reopened = steammock::Store::open(file.path(), second_error);
    check("the file opens again as a store", reopened != nullptr);
    if (reopened == nullptr)
    {
        std::printf("        %s\n", second_error.c_str());
        return;
    }

    // The next run starts from the same authored scenario it always did.
    steammock::Profile merged = seeded_hero();
    reopened->merge_into(merged);

    std::int64_t deaths = 0;
    check("a stat a game wrote is still there", merged.find_stat("Deaths", deaths) && deaths == 9);
    const steammock::Achievement* unlocked = merged.find_achievement("ACH_BOOTED");
    check("an achievement a game unlocked is still unlocked",
          unlocked != nullptr && unlocked->achieved);
}

void test_the_scenario_seeds_and_a_game_has_the_last_word()
{
    std::printf(
        "[:] a scenario edit reaches a run the game has not written to, and not one it has\n");

    TempState file("precedence");
    std::string error;
    std::unique_ptr<steammock::Store> store = steammock::Store::open(file.path(), error);
    if (store == nullptr)
    {
        check("a state file is made when there is none", false);
        std::printf("        %s\n", error.c_str());
        return;
    }

    // Two stats, and the game writes only one of them.
    steammock::Profile authored = profile_named("hero");
    authored.stats.emplace_back("Deaths", 3);
    authored.stats.emplace_back("Wins", 0);
    store->seed(authored);
    store->write_stat("hero", "Wins", 7);
    store.reset();

    // The author then edits both values in the scenario.
    std::string second_error;
    std::unique_ptr<steammock::Store> reopened = steammock::Store::open(file.path(), second_error);
    if (reopened == nullptr)
    {
        check("the file opens again as a store", false);
        return;
    }
    steammock::Profile edited = profile_named("hero");
    edited.stats.emplace_back("Deaths", 5); // only a scenario ever wrote this one
    edited.stats.emplace_back("Wins", 0);   // the game wrote this one
    reopened->seed(edited);

    steammock::Profile merged = edited;
    reopened->merge_into(merged);

    std::int64_t deaths = 0;
    std::int64_t wins = 0;
    check("the edit reaches a stat only a scenario had written",
          merged.find_stat("Deaths", deaths) && deaths == 5);
    check("and does not reach one a game had written", merged.find_stat("Wins", wins) && wins == 7);
}

void test_nothing_is_ever_deleted()
{
    std::printf("[:] a row nobody declares any more is left alone\n");

    TempState file("undeclared");
    std::string error;
    std::unique_ptr<steammock::Store> store = steammock::Store::open(file.path(), error);
    if (store == nullptr)
    {
        check("a state file is made when there is none", false);
        return;
    }

    steammock::Profile authored = profile_named("hero");
    steammock::Friend peer;
    peer.steam_id = 76561198000000002ull;
    peer.persona_name = "Peer";
    authored.friends.push_back(peer);
    authored.achievements.push_back({"ACH_GONE", false});
    store->seed(authored);
    store.reset();

    // A run whose scenario declares none of it, and a friend added by hand the way a person
    // with a SQLite prompt would add one.
    check("a row can be written by hand",
          run_sql(file.path(), "INSERT INTO profile_friend (profile, steam_id, persona, source)"
                               " VALUES ('hero', 76561198000000077, 'HandAdded', 'game');"));

    std::string second_error;
    std::unique_ptr<steammock::Store> reopened = steammock::Store::open(file.path(), second_error);
    if (reopened == nullptr)
    {
        check("the file opens again as a store", false);
        return;
    }

    steammock::Profile empty = profile_named("hero");
    reopened->seed(empty); // nothing declared any more
    reopened->merge_into(empty);

    std::string named;
    check("the friend the scenario no longer declares is still there",
          empty.find_friend(76561198000000002ull, named) && named == "Peer");
    check("and so is the one written by hand",
          empty.find_friend(76561198000000077ull, named) && named == "HandAdded");
    check("and the achievement nobody declares either",
          empty.find_achievement("ACH_GONE") != nullptr);
}

void test_a_scenario_keeps_its_own_friends_but_the_store_adds()
{
    std::printf("[:] a friend the scenario names answers to the scenario\n");

    TempState file("friends");
    std::string error;
    std::unique_ptr<steammock::Store> store = steammock::Store::open(file.path(), error);
    if (store == nullptr)
    {
        check("a state file is made when there is none", false);
        return;
    }

    steammock::Profile authored = profile_named("hero");
    steammock::Friend peer;
    peer.steam_id = 76561198000000002ull;
    peer.persona_name = "OldName";
    authored.friends.push_back(peer);
    store->seed(authored);
    store.reset();

    // The author renames that friend, and the store still holds the old name.
    std::string second_error;
    std::unique_ptr<steammock::Store> reopened = steammock::Store::open(file.path(), second_error);
    if (reopened == nullptr)
    {
        check("the file opens again as a store", false);
        return;
    }
    steammock::Profile edited = profile_named("hero");
    peer.persona_name = "NewName";
    edited.friends.push_back(peer);
    reopened->seed(edited);

    steammock::Profile merged = edited;
    reopened->merge_into(merged);

    check("the friends list did not gain a second entry for one person",
          merged.friends.size() == 1u);
    check("the name the scenario gives is the one kept",
          merged.friends.size() == 1u && merged.friends[0].persona_name == "NewName");
}

// --- the worlds ------------------------------------------------------------

void test_boards_come_back_ranked()
{
    std::printf("[:] a board and its rows come back, ranked again\n");

    TempState file("boards");
    std::string error;
    std::unique_ptr<steammock::Store> store = steammock::Store::open(file.path(), error);
    if (store == nullptr)
    {
        check("a state file is made when there is none", false);
        return;
    }

    steammock::Leaderboard board;
    board.name = "Feet Traveled";
    // 1 is the SDK's ascending, and 2 its "this is a number": a board whose smallest score
    // wins, which is what makes the re-ranking below visible.
    board.sort_method = 1;
    board.display_type = 2;
    store->save_board(board);

    // Saved without a rank, deliberately: a rank is the board's own answer and not a fact
    // about a score, so nothing writes one here and the world has to work it out.
    steammock::LeaderboardRow mine;
    mine.steam_id = 76561198000000002ull;
    mine.score = 30;
    store->save_row(board.name, mine);
    steammock::LeaderboardRow theirs;
    theirs.steam_id = 76561198000000003ull;
    theirs.score = 10;
    store->save_row(board.name, theirs);
    check("no write failed", store->error().empty());
    store.reset();

    std::string second_error;
    std::unique_ptr<steammock::Store> reopened = steammock::Store::open(file.path(), second_error);
    if (reopened == nullptr)
    {
        check("the file opens again as a store", false);
        return;
    }

    steammock::LeaderboardWorld world;
    world.attach(reopened.get());

    const std::vector<steammock::Leaderboard>& boards = world.boards();
    check("the board is back", boards.size() == 1u);
    if (boards.size() != 1u)
    {
        return;
    }
    check("with its name", boards[0].name == "Feet Traveled");
    check("and what the game said its numbers mean",
          boards[0].sort_method == 1 && boards[0].display_type == 2);
    check("on a handle this run handed out, not the one the last run did",
          boards[0].id == steammock::LeaderboardWorld::kFirstBoardId);
    check("both rows are on it", boards[0].rows.size() == 2u);
    if (boards[0].rows.size() == 2u)
    {
        check("and the ascending one is ranked first",
              boards[0].rows[0].steam_id == 76561198000000003ull &&
                  boards[0].rows[0].global_rank == 1);
        check("with the other ranked after it",
              boards[0].rows[1].steam_id == 76561198000000002ull &&
                  boards[0].rows[1].global_rank == 2);
    }
}

void test_inventories_come_back_and_the_ids_keep_going()
{
    std::printf("[:] what a player holds comes back, and the next item id clears it\n");

    TempState file("inventory");
    std::string error;
    std::unique_ptr<steammock::Store> store = steammock::Store::open(file.path(), error);
    if (store == nullptr)
    {
        check("a state file is made when there is none", false);
        return;
    }

    // An item written by hand with an id inside the range this world mints its own from,
    // which is the id a grant afterwards has to clear. (An id *below* that range would be
    // cleared anyway - the counter starts high - so it could not show whether the store's
    // ids were read back at all, which is the thing being checked here.)
    constexpr std::uint64_t kPlayer = 76561198000000002ull;
    const std::uint64_t stored_item = steammock::InventoryWorld::kFirstItemId + 40u;
    const std::string insert =
        "INSERT INTO inventory_item (steam_id, item_id, definition, quantity, flags)"
        " VALUES (76561198000000002, " +
        std::to_string(stored_item) + ", 100, 1, 0);";
    check("an item can be written by hand", run_sql(file.path(), insert.c_str()));
    store.reset();

    std::string second_error;
    std::unique_ptr<steammock::Store> reopened = steammock::Store::open(file.path(), second_error);
    if (reopened == nullptr)
    {
        check("the file opens again as a store", false);
        return;
    }

    steammock::InventoryWorld inventory;
    inventory.attach(reopened.get());

    const std::vector<steammock::ItemDetails>& held = inventory.inventory_of(kPlayer);
    check("the player holds the item that was there",
          held.size() == 1u && held[0].item_id == stored_item && held[0].definition == 100);
    check("and a player nobody has heard of holds nothing",
          inventory.inventory_of(76561198000000099ull).empty());

    // Granting the promotions now: 100 is already held, so only 101 is new, and its id has to
    // be the one after the stored one - an id that came round again is an item a game is told
    // it has two of.
    steammock::Session session("session0", Json::object(), player(kPlayer));
    steammock::Answer answer;
    const Json no_args = Json::object();
    check("GrantPromoItems is answered",
          inventory.answer(session, "SteamAPI_ISteamInventory_GrantPromoItems", no_args, answer) &&
              answer.answered);
    check("nothing failed on the way", reopened->error().empty());

    const std::vector<steammock::ItemDetails>& after = inventory.inventory_of(kPlayer);
    check("the item the player already held was not granted twice", after.size() == 2u);
    if (after.size() == 2u)
    {
        check("and the new one has an id the stored one had not used",
              after[1].definition == 101 && after[1].item_id == stored_item + 1u);
    }
}

// --- the file itself, and what a store refuses -----------------------------

void test_the_file_is_kept_the_cheap_way()
{
    std::printf("[:] the file is kept so that a commit is cheap, and left as one file\n");

    TempState file("journal");
    std::string error;
    std::unique_ptr<steammock::Store> store = steammock::Store::open(file.path(), error);
    check("a state file is made when there is none", store != nullptr);
    if (store == nullptr)
    {
        std::printf("        %s\n", error.c_str());
        return;
    }

    std::string mode;
    check("the file's journal mode can be read back",
          read_text(file.path(), "PRAGMA journal_mode", mode));
    // The measurement behind this is in src/store.cpp: a committed row costs 1,290 us in
    // the default mode and 20 us here, and this write happens while the server's state
    // lock is held. What that costs a run is not visible in the file's contents, which is
    // exactly why it is checked here.
    check("and it is the write-ahead log, where a committed row costs 20 us and not 1,290",
          mode == "wal");

    store.reset();
    // The write-ahead log is a second file beside the database while a run has it open, and a
    // clean close checkpoints it away. This is therefore the case a person is left with: one
    // file to copy or keep. A run that was *killed* leaves the log behind instead and the next
    // open recovers from it, which is the route the state-file runs in test_end_to_end take.
    check("and a clean close leaves one file behind",
          std::filesystem::exists(file.path()) && !std::filesystem::exists(file.path() + "-wal"));
}

void test_a_file_that_is_not_a_state_file_is_refused()
{
    std::printf("[:] a file that is not one of these is refused, and not written into\n");

    TempState file("not-a-database");
    {
        std::ofstream out(file.path(), std::ios::binary | std::ios::trunc);
        out << "this is somebody's shopping list, not a database";
    }

    std::string error;
    std::unique_ptr<steammock::Store> store = steammock::Store::open(file.path(), error);
    check("a file that is not a database is not opened", store == nullptr);
    check("and the refusal says what it was doing", error.find(file.path()) != std::string::npos);

    // A database, but not this one: the version is the file's own statement about what it
    // holds, and a build that reads a different set of things has no business in it.
    TempState alien("other-schema");
    check("a database can be made by hand",
          run_sql(alien.path(), "CREATE TABLE meta (name TEXT PRIMARY KEY,"
                                " value TEXT NOT NULL);"
                                "INSERT INTO meta VALUES ('schema', '99');"));
    std::string alien_error;
    std::unique_ptr<steammock::Store> alien_store =
        steammock::Store::open(alien.path(), alien_error);
    check("a state file from another schema is not opened", alien_store == nullptr);
    check("and says which version it was", alien_error.find("99") != std::string::npos);

    // `:memory:` is SQLite's own name for a database that is not a file, and it means the
    // same thing here: the schema, and nothing kept after it goes.
    std::string memory_error;
    std::unique_ptr<steammock::Store> memory = steammock::Store::open(":memory:", memory_error);
    check("a store that is not a file opens", memory != nullptr);
    if (memory != nullptr)
    {
        memory->seed(seeded_hero());
        steammock::Profile merged = profile_named("hero");
        memory->merge_into(merged);
        std::int64_t deaths = 0;
        check("and holds what it is written", merged.find_stat("Deaths", deaths) && deaths == 3);
    }
}

} // namespace

int run()
{
    std::printf("[+] SteamMock state file tests\n\n");
    test_a_state_file_keeps_what_a_run_wrote();
    test_the_scenario_seeds_and_a_game_has_the_last_word();
    test_nothing_is_ever_deleted();
    test_a_scenario_keeps_its_own_friends_but_the_store_adds();
    test_boards_come_back_ranked();
    test_inventories_come_back_and_the_ids_keep_going();
    test_the_file_is_kept_the_cheap_way();
    test_a_file_that_is_not_a_state_file_is_refused();

    if (g_failures == 0)
    {
        std::printf("\n[+] all checks passed\n");
    }
    else
    {
        std::printf("\n[-] %d check(s) FAILED\n", g_failures);
    }
    return g_failures == 0 ? 0 : 1;
}

// An exception escaping `main` terminates the process with no message at all, and the
// only realistic source in a test is a failed allocation. Report it the way a failing
// check is reported instead, so ctest's output says what happened.
int main()
{
    try
    {
        return run();
    }
    catch (const std::exception& error)
    {
        std::printf("\n[-] the test itself threw: %s\n", error.what());
        return 1;
    }
    catch (...)
    {
        std::printf("\n[-] the test itself threw something that is not a std::exception\n");
        return 1;
    }
}
