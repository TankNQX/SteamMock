#include "bridge/store.hpp"

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <sqlite3.h>

#include "bridge/session.hpp"

namespace steammock {

namespace {

// ---------------------------------------------------------------------------
//  The schema.
// ---------------------------------------------------------------------------
//  One table per kind of thing a game leaves behind, rather than one table of
//  names and values: these are different things, and somebody with a SQL prompt
//  should be able to see which is which without being told. A row a person writes
//  by hand into any of them is read back like any other, with one exception noted
//  at `seed` below.
//
//  `source` is the column the whole reconciliation turns on. It says who put a
//  value there - a scenario, or a game - and it is only ever read when a scenario
//  is seeding, because that is the one moment the difference matters.
//
//  Every id here is an INTEGER. The largest SteamID64 in use is about 7.6e16 and
//  an item id is a counter, so both fit SQLite's signed 64-bit integer whole.
//
//  Nothing is APP-scoped, deliberately: it mirrors how this backend's worlds
//  already key themselves - a board by its name, an inventory by the player
//  holding it - rather than inventing a key the in-memory model does not have.
//  Two apps with a board of the same name share one row here for the same reason
//  they share one board within a run. See docs/architecture.md.
constexpr const char* const kSchema[] = {
    "CREATE TABLE IF NOT EXISTS meta ("
    "  name  TEXT PRIMARY KEY,"
    "  value TEXT NOT NULL);",

    // Which version of this schema the file was written with, so a file from a
    // build that kept something different is refused rather than half-read.
    "INSERT OR IGNORE INTO meta (name, value) VALUES ('schema', '1');",

    "CREATE TABLE IF NOT EXISTS profile_stat ("
    "  profile TEXT NOT NULL,"
    "  name    TEXT NOT NULL,"
    "  value   INTEGER NOT NULL,"
    "  source  TEXT NOT NULL,"
    "  PRIMARY KEY (profile, name));",

    "CREATE TABLE IF NOT EXISTS profile_achievement ("
    "  profile  TEXT NOT NULL,"
    "  name     TEXT NOT NULL,"
    "  achieved INTEGER NOT NULL,"
    "  source   TEXT NOT NULL,"
    "  PRIMARY KEY (profile, name));",

    "CREATE TABLE IF NOT EXISTS profile_friend ("
    "  profile  TEXT NOT NULL,"
    "  steam_id INTEGER NOT NULL,"
    "  persona  TEXT NOT NULL,"
    "  source   TEXT NOT NULL,"
    "  PRIMARY KEY (profile, steam_id));",

    "CREATE TABLE IF NOT EXISTS leaderboard ("
    "  name         TEXT PRIMARY KEY,"
    "  sort_method  INTEGER NOT NULL,"
    "  display_type INTEGER NOT NULL);",

    "CREATE TABLE IF NOT EXISTS leaderboard_row ("
    "  board    TEXT NOT NULL,"
    "  steam_id INTEGER NOT NULL,"
    "  score    INTEGER NOT NULL,"
    "  PRIMARY KEY (board, steam_id));",

    "CREATE TABLE IF NOT EXISTS inventory_item ("
    "  steam_id   INTEGER NOT NULL,"
    "  item_id    INTEGER NOT NULL,"
    "  definition INTEGER NOT NULL,"
    "  quantity   INTEGER NOT NULL,"
    "  flags      INTEGER NOT NULL,"
    "  PRIMARY KEY (steam_id, item_id));",
};

// The one schema version this build writes and reads. A file that says anything
// else was written by a build that kept a different set of things, and guessing at
// what it meant is how a state file becomes a mystery. What goes in the `source`
// column - 'scenario' or 'game' - is written out at each statement that uses it
// rather than held in a constant, because it is part of the SQL and reads as such.
constexpr const char* const kSchemaVersion = "1";

// ---------------------------------------------------------------------------
//  One prepared statement, finalized however its scope is left.
// ---------------------------------------------------------------------------
//  SQLite hands back a bare pointer that nobody owns, and every early return in
//  the functions below would otherwise be a leak SQLite does not mention. There is
//  no statement cache: preparing one of these is a few microseconds, and this store
//  is written to once per call a game makes, at most.
class Statement {
public:
    Statement(sqlite3* db, const char* sql) {
        if (sqlite3_prepare_v2(db, sql, -1, &_statement, nullptr) != SQLITE_OK) {
            _statement = nullptr;
        }
    }

    ~Statement() {
        if (_statement != nullptr) {
            sqlite3_finalize(_statement);
        }
    }

    Statement(const Statement&) = delete;
    Statement& operator=(const Statement&) = delete;
    Statement(Statement&&) = delete;
    Statement& operator=(Statement&&) = delete;

    sqlite3_stmt* get() const noexcept { return _statement; }
    bool ok() const noexcept { return _statement != nullptr; }

private:
    sqlite3_stmt* _statement = nullptr;
};

void bind_text(sqlite3_stmt* statement, int index, const std::string& value) {
    // SQLITE_TRANSIENT: SQLite copies it, so the caller's string may die with the call.
    sqlite3_bind_text(statement, index, value.c_str(), static_cast<int>(value.size()),
                      SQLITE_TRANSIENT);
}

void bind_i64(sqlite3_stmt* statement, int index, std::int64_t value) {
    sqlite3_bind_int64(statement, index, value);
}

std::string column_text(sqlite3_stmt* statement, int index) {
    const unsigned char* text = sqlite3_column_text(statement, index);
    return text == nullptr ? std::string() : std::string(reinterpret_cast<const char*>(text));
}

// ---------------------------------------------------------------------------
//  SQLite, and the only code in this repository that has ever heard of it.
// ---------------------------------------------------------------------------
//  Confined to this file deliberately. `bridge/store.hpp` names no SQLite type and
//  no SQL, so what a state file *is* is a decision this translation unit holds
//  rather than one every caller reads past - and replacing the file with something
//  else is replacing this file.
class SqliteStore final : public Store {
public:
    explicit SqliteStore(sqlite3* db) : _db(db) {
        // So two processes pointed at one state file queue behind each other rather
        // than the second one failing every write it makes. Five seconds is far
        // longer than any write here takes and short enough that a wedged process
        // does not hold a run up forever.
        sqlite3_busy_timeout(_db, 5000);
        choose_journal();
        create_schema();
    }

    ~SqliteStore() override { sqlite3_close_v2(_db); }

    const std::string& error() const noexcept override { return _error; }

    void seed(const Profile& profile) override;
    void merge_into(Profile& profile) const override;
    void write_stat(const std::string& profile, const std::string& name,
                    std::int64_t value) override;
    void write_achievement(const std::string& profile, const std::string& name,
                           bool achieved) override;

    void load_boards(std::vector<StoredBoard>& boards) const override;
    void save_board(const Leaderboard& board) override;
    void save_row(const std::string& board, const LeaderboardRow& row) override;

    void
    load_inventories(std::map<std::uint64_t, std::vector<ItemDetails>>& inventories) const override;
    void save_item(std::uint64_t steam_id, const ItemDetails& item) override;

private:
    // Records the first thing that went wrong and never touches it again. Later
    // failures are the same failure arriving again - a state file that has stopped
    // accepting writes keeps not accepting them - and the first one is the one that
    // says why.
    //
    // Const, and `_error` is mutable, because the reads that can fail - merging a
    // profile, loading the boards, loading the inventories - are const questions whose
    // failure still has to be written down somewhere.
    void note(std::string what) const;
    void note_sqlite() const;

    // One statement, run once, with nothing to hand back. Every write here is this.
    //
    // A template rather than a std::function, because this is called once per call a
    // game makes and a std::function would be an allocation for a lambda that is a
    // handful of binds.
    template <typename Bind> bool run(const char* sql, Bind&& bind) {
        Statement statement(_db, sql);
        if (!statement.ok()) {
            note_sqlite();
            return false;
        }
        bind(statement.get());
        if (sqlite3_step(statement.get()) == SQLITE_DONE) {
            return true;
        }
        note_sqlite();
        return false;
    }

    void create_schema();
    bool check_schema_version();
    void choose_journal();

    sqlite3* _db = nullptr;
    mutable std::string _error;
};

void SqliteStore::note(std::string what) const {
    if (_error.empty()) {
        _error = std::move(what);
    }
}

void SqliteStore::note_sqlite() const {
    if (sqlite3_errcode(_db) != SQLITE_OK) {
        note(sqlite3_errmsg(_db));
    }
}

void SqliteStore::create_schema() {
    for (const char* const statement : kSchema) {
        char* message = nullptr;
        const int status = sqlite3_exec(_db, statement, nullptr, nullptr, &message);
        if (status == SQLITE_OK) {
            continue;
        }
        std::string text =
            message != nullptr ? std::string(message) : std::string(sqlite3_errmsg(_db));
        if (message != nullptr) {
            sqlite3_free(message);
        }
        note(std::move(text));
        return;  // every later statement is about a table this one did not make
    }
    check_schema_version();
}

void SqliteStore::choose_journal() {
    // Fast commits where SQLite says it can be fast, and the durable default where it
    // cannot.
    //
    // Every write in this file is one row in a transaction of its own, and it happens
    // while the server's state lock is held - so what a commit costs is what every other
    // game attached to the run waits for. Measured here as one committed row at a time,
    // 5,000 of them: **1,290 us with the defaults, 267 us with the write-ahead log, and
    // 20 us with the write-ahead log and `synchronous=NORMAL`.** The last pair is also
    // the one combination that cannot corrupt the file: out of a write-ahead log,
    // `NORMAL` risks a torn page after a power loss; in one, it can only lose the commits
    // since the last checkpoint, which for a state file is a round or two of play.
    //
    // A write-ahead log needs shared memory and a filesystem that has it, so a state file
    // on a network share does not get one. There the mode is left alone and `FULL` stays:
    // a slow run is a slow run, and a file somebody is managing is not one to corrupt for
    // speed. The mode the file is actually in is what decides, which is why this reads it
    // back rather than assuming the pragma was obeyed.
    Statement statement(_db, "PRAGMA journal_mode=WAL");
    if (!statement.ok() || sqlite3_step(statement.get()) != SQLITE_ROW) {
        return;
    }
    if (column_text(statement.get(), 0) != "wal") {
        return;
    }
    // Ignored on purpose: this is a cost rather than a correctness setting, and a file
    // that would not take it is still a file this store can use.
    sqlite3_exec(_db, "PRAGMA synchronous=NORMAL", nullptr, nullptr, nullptr);
}

bool SqliteStore::check_schema_version() {
    Statement statement(_db, "SELECT value FROM meta WHERE name = 'schema'");
    if (!statement.ok()) {
        note_sqlite();
        return false;
    }
    if (sqlite3_step(statement.get()) != SQLITE_ROW) {
        // create_schema inserts this row, so there being none means the file is not
        // the one this build just made - which is worth saying rather than reading as
        // "no opinion" and carrying on to write into something unknown.
        note("the state file says nothing about which schema it was written with");
        return false;
    }
    const std::string version = column_text(statement.get(), 0);
    if (version == kSchemaVersion) {
        return true;
    }
    note("the state file was written with schema version " + version + ", and this build reads " +
         kSchemaVersion);
    return false;
}

// --- what one identity has -------------------------------------------------

void SqliteStore::seed(const Profile& profile) {
    // A key the store has never been told about, and one only a scenario has ever
    // written. The `WHERE` on the update is the whole of that rule: a row a game
    // wrote does not match it, so the game's value stays.
    constexpr const char* kStat =
        "INSERT INTO profile_stat (profile, name, value, source) VALUES (?1, ?2, ?3, 'scenario')"
        " ON CONFLICT(profile, name) DO UPDATE SET value = excluded.value, source = 'scenario'"
        " WHERE profile_stat.source = 'scenario'";
    // Iterated as a pair rather than through a structured binding, because the lambda below
    // wants them: capturing a structured binding is a C++20 extension, and this tree is
    // C++17 - which clang says out loud and MSVC does not.
    for (const std::pair<std::string, std::int64_t>& stat : profile.stats) {
        run(kStat, [&](sqlite3_stmt* statement) {
            bind_text(statement, 1, profile.name);
            bind_text(statement, 2, stat.first);
            bind_i64(statement, 3, stat.second);
        });
    }

    constexpr const char* kAchievement =
        "INSERT INTO profile_achievement (profile, name, achieved, source)"
        " VALUES (?1, ?2, ?3, 'scenario')"
        " ON CONFLICT(profile, name) DO UPDATE SET achieved = excluded.achieved,"
        " source = 'scenario' WHERE profile_achievement.source = 'scenario'";
    for (const Achievement& achievement : profile.achievements) {
        run(kAchievement, [&](sqlite3_stmt* statement) {
            bind_text(statement, 1, profile.name);
            bind_text(statement, 2, achievement.name);
            bind_i64(statement, 3, achievement.achieved ? 1 : 0);
        });
    }

    // A friends list, on the same rule. A friend whose id is zero is one the scenario
    // only gave a name for - the dispatcher fills an id in from the profile the entry
    // names, and an entry naming nothing never got one - and an id of zero is "nobody"
    // rather than a friend, so it is not written down.
    constexpr const char* kFriend =
        "INSERT INTO profile_friend (profile, steam_id, persona, source)"
        " VALUES (?1, ?2, ?3, 'scenario')"
        " ON CONFLICT(profile, steam_id) DO UPDATE SET persona = excluded.persona,"
        " source = 'scenario' WHERE profile_friend.source = 'scenario'";
    for (const Friend& friend_entry : profile.friends) {
        if (friend_entry.steam_id == 0) {
            continue;
        }
        run(kFriend, [&](sqlite3_stmt* statement) {
            bind_text(statement, 1, profile.name);
            bind_i64(statement, 2, static_cast<std::int64_t>(friend_entry.steam_id));
            bind_text(statement, 3, friend_entry.persona_name);
        });
    }
}

void SqliteStore::merge_into(Profile& profile) const {
    {
        Statement statement(_db, "SELECT name, value FROM profile_stat WHERE profile = ?1");
        if (statement.ok()) {
            bind_text(statement.get(), 1, profile.name);
            while (sqlite3_step(statement.get()) == SQLITE_ROW) {
                // Read apart rather than in one call, so which column is which does not
                // depend on the order two arguments happen to be evaluated in.
                const std::string name = column_text(statement.get(), 0);
                const std::int64_t value = sqlite3_column_int64(statement.get(), 1);
                // `set_stat` rather than a lookup and a write: it is the one place a
                // stat is ever put into a profile, and a lookup's answer can dangle.
                profile.set_stat(name, value);
            }
        }
    }

    {
        Statement statement(_db,
                            "SELECT name, achieved FROM profile_achievement WHERE profile = ?1");
        if (statement.ok()) {
            bind_text(statement.get(), 1, profile.name);
            while (sqlite3_step(statement.get()) == SQLITE_ROW) {
                const std::string name = column_text(statement.get(), 0);
                const bool achieved = sqlite3_column_int64(statement.get(), 1) != 0;
                // An achievement the scenario declared keeps its own place in the list -
                // that order is what a game asking for the nth name is answered from - so
                // a stored one is written into the entry that is already there, and one the
                // scenario does not have is added at the end.
                if (Achievement* existing = profile.find_achievement(name); existing != nullptr) {
                    existing->achieved = achieved;
                } else {
                    Achievement added;
                    added.name = name;
                    added.achieved = achieved;
                    profile.achievements.push_back(std::move(added));
                }
            }
        }
    }

    {
        Statement statement(_db, "SELECT steam_id, persona FROM profile_friend WHERE profile = ?1");
        if (statement.ok()) {
            bind_text(statement.get(), 1, profile.name);
            while (sqlite3_step(statement.get()) == SQLITE_ROW) {
                const std::uint64_t steam_id =
                    static_cast<std::uint64_t>(sqlite3_column_int64(statement.get(), 0));
                std::string named;
                if (profile.find_friend(steam_id, named)) {
                    // The scenario names this one, and the scenario is what a person
                    // edits - so its name is the one kept, whatever a previous run wrote.
                    continue;
                }
                Friend added;
                added.steam_id = steam_id;
                added.persona_name = column_text(statement.get(), 1);
                profile.friends.push_back(std::move(added));
            }
        }
    }
}

void SqliteStore::write_stat(const std::string& profile, const std::string& name,
                             std::int64_t value) {
    // A game wrote this, so it is the store's whatever a scenario says next time.
    constexpr const char* kSql =
        "INSERT INTO profile_stat (profile, name, value, source) VALUES (?1, ?2, ?3, 'game')"
        " ON CONFLICT(profile, name) DO UPDATE SET value = excluded.value, source = 'game'";
    run(kSql, [&](sqlite3_stmt* statement) {
        bind_text(statement, 1, profile);
        bind_text(statement, 2, name);
        bind_i64(statement, 3, value);
    });
}

void SqliteStore::write_achievement(const std::string& profile, const std::string& name,
                                    bool achieved) {
    constexpr const char* kSql =
        "INSERT INTO profile_achievement (profile, name, achieved, source)"
        " VALUES (?1, ?2, ?3, 'game')"
        " ON CONFLICT(profile, name) DO UPDATE SET achieved = excluded.achieved, source = 'game'";
    run(kSql, [&](sqlite3_stmt* statement) {
        bind_text(statement, 1, profile);
        bind_text(statement, 2, name);
        bind_i64(statement, 3, achieved ? 1 : 0);
    });
}

// --- what the run's players have done --------------------------------------

void SqliteStore::load_boards(std::vector<StoredBoard>& boards) const {
    // By name, so the order the boards come back in does not depend on the order rows
    // happen to sit in the file. Nothing about a board's *handles* is stored: a handle
    // is a number this run hands a game, and the next run hands out its own.
    Statement statement(_db, "SELECT name, sort_method, display_type FROM leaderboard"
                             " ORDER BY name");
    if (!statement.ok()) {
        note_sqlite();
        return;
    }
    while (sqlite3_step(statement.get()) == SQLITE_ROW) {
        StoredBoard board;
        board.name = column_text(statement.get(), 0);
        board.sort_method = static_cast<std::int32_t>(sqlite3_column_int64(statement.get(), 1));
        board.display_type = static_cast<std::int32_t>(sqlite3_column_int64(statement.get(), 2));

        Statement rows(_db, "SELECT steam_id, score FROM leaderboard_row WHERE board = ?1"
                            " ORDER BY steam_id");
        if (!rows.ok()) {
            note_sqlite();
            return;
        }
        bind_text(rows.get(), 1, board.name);
        while (sqlite3_step(rows.get()) == SQLITE_ROW) {
            board.scores.emplace_back(
                static_cast<std::uint64_t>(sqlite3_column_int64(rows.get(), 0)),
                static_cast<std::int32_t>(sqlite3_column_int64(rows.get(), 1)));
        }
        boards.push_back(std::move(board));
    }
}

void SqliteStore::save_board(const Leaderboard& board) {
    constexpr const char* kSql =
        "INSERT INTO leaderboard (name, sort_method, display_type) VALUES (?1, ?2, ?3)"
        " ON CONFLICT(name) DO UPDATE SET sort_method = excluded.sort_method,"
        " display_type = excluded.display_type";
    run(kSql, [&](sqlite3_stmt* statement) {
        bind_text(statement, 1, board.name);
        bind_i64(statement, 2, board.sort_method);
        bind_i64(statement, 3, board.display_type);
    });
}

void SqliteStore::save_row(const std::string& board, const LeaderboardRow& row) {
    constexpr const char* kSql =
        "INSERT INTO leaderboard_row (board, steam_id, score) VALUES (?1, ?2, ?3)"
        " ON CONFLICT(board, steam_id) DO UPDATE SET score = excluded.score";
    run(kSql, [&](sqlite3_stmt* statement) {
        bind_text(statement, 1, board);
        bind_i64(statement, 2, static_cast<std::int64_t>(row.steam_id));
        bind_i64(statement, 3, row.score);
    });
}

void SqliteStore::load_inventories(
    std::map<std::uint64_t, std::vector<ItemDetails>>& inventories) const {
    Statement statement(_db, "SELECT steam_id, item_id, definition, quantity, flags"
                             " FROM inventory_item ORDER BY steam_id, item_id");
    if (!statement.ok()) {
        note_sqlite();
        return;
    }
    while (sqlite3_step(statement.get()) == SQLITE_ROW) {
        ItemDetails item;
        item.item_id = static_cast<std::uint64_t>(sqlite3_column_int64(statement.get(), 1));
        item.definition = static_cast<std::int32_t>(sqlite3_column_int64(statement.get(), 2));
        item.quantity = static_cast<std::uint16_t>(sqlite3_column_int64(statement.get(), 3));
        item.flags = static_cast<std::uint16_t>(sqlite3_column_int64(statement.get(), 4));
        inventories[static_cast<std::uint64_t>(sqlite3_column_int64(statement.get(), 0))].push_back(
            item);
    }
}

void SqliteStore::save_item(std::uint64_t steam_id, const ItemDetails& item) {
    constexpr const char* kSql =
        "INSERT INTO inventory_item (steam_id, item_id, definition, quantity, flags)"
        " VALUES (?1, ?2, ?3, ?4, ?5)"
        " ON CONFLICT(steam_id, item_id) DO UPDATE SET definition = excluded.definition,"
        " quantity = excluded.quantity, flags = excluded.flags";
    run(kSql, [&](sqlite3_stmt* statement) {
        bind_i64(statement, 1, static_cast<std::int64_t>(steam_id));
        bind_i64(statement, 2, static_cast<std::int64_t>(item.item_id));
        bind_i64(statement, 3, item.definition);
        bind_i64(statement, 4, item.quantity);
        bind_i64(statement, 5, item.flags);
    });
}

}  // namespace

std::unique_ptr<Store> Store::open(const std::string& path, std::string& error) {
    // Not `sqlite3_open_v2` with a mode a caller chose: this store is either reading
    // and writing or it is not open, and the one decision left - whether to make the
    // file - is the only one a state file has. The flags say both because a run given
    // a path is a run that wants it used.
    sqlite3* db = nullptr;
    const int status =
        sqlite3_open_v2(path.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (status != SQLITE_OK) {
        error = "cannot open the state file '" + path +
                "': " + (db != nullptr ? sqlite3_errmsg(db) : "out of memory");
        if (db != nullptr) {
            sqlite3_close_v2(db);
        }
        return nullptr;
    }

    auto store = std::make_unique<SqliteStore>(db);
    if (!store->error().empty()) {
        // A file that is not this harness's, or one from a build that kept something
        // else. Either way nothing may be written to it, and a run that carried on
        // would be quietly not keeping anything - which is the failure this whole
        // class exists to make visible.
        error = "the state file '" + path + "' cannot be used: " + store->error();
        return nullptr;
    }
    return store;
}

}  // namespace steammock
