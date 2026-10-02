#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "bridge/json_read.hpp"
#include "bridge/session.hpp"

namespace steammock {

// Defined in bridge/store.hpp, which includes this header: the store speaks about an
// ItemDetails, so it cannot be included from here without a cycle.
class Store;

// ---------------------------------------------------------------------------
//  The catalogue, and what each player holds.
// ---------------------------------------------------------------------------
//  Inventory is the third thing here that one call cannot answer, and it is split in two the
//  way Steam splits it: *definitions* are the app's - one catalogue of what a kind of item is,
//  with a name and a description the game draws - and an *inventory* is one player's, made of
//  instances of those definitions. So the catalogue lives here beside the rooms and the boards,
//  and an inventory is per session.
//
//  Nothing here mints an item on request. A game can ask (`GenerateItems`, `ExchangeItems`,
//  `TriggerItemDrop`) and what it gets back is a result that holds nothing, because an item
//  appears in an inventory by being *granted*: this world grants the promotions its own policy
//  names, once, when the game asks for them - which is what Spacewar does on its way to any
//  screen that shows an inventory. That is the same rule the boards keep about scores: the
//  harness answers about what games have actually done, and does not invent an entitlement.

// One instance a player holds: which instance, of what definition, how many, and the SDK's own
// flags. Sixteen bytes, which is what the game's own array is made of - see SteamItemDetails_t
// in the layouts, which is what `GetResultItems` fills in.
struct ItemDetails {
    std::uint64_t item_id = 0;
    std::int32_t definition = 0;
    std::uint16_t quantity = 0;
    std::uint16_t flags = 0;
};

// What the app says one *kind* of item is. Every player's inventory is made of these, which is
// why the catalogue is not per session.
struct ItemDefinition {
    std::int32_t id = 0;
    std::string name;
    std::string description;
    std::string icon_url;
};

class InventoryWorld {
public:
    // An item instance id is the game's to hand back and nothing more: Steam's own is opaque,
    // so this is a counter with a shape that is not a SteamID - a game that printed one would
    // see that it is not a player.
    static constexpr std::uint64_t kFirstItemId = 0x0C00000000000001ull;

    // Result handles are ours as well, and they are 32-bit because the SDK's own
    // `SteamInventoryResult_t` is: -1 is the invalid one, which is what Spacewar starts its
    // handles at.
    static constexpr std::int32_t kFirstResultHandle = 1;

    // How many results stay readable. A real handle is invalid the moment its callback
    // returns, and the window is here for the same reason the board world keeps one: a game
    // that never destroys a result would otherwise grow this for the length of a run.
    static constexpr std::size_t kMaxResults = 64;

    // The calls this world answers, so a test can prove every one of them is a name the stub
    // can actually send.
    static std::vector<std::string> handled_calls();

    InventoryWorld();

    // Where what the players hold is kept, or null for a run that keeps nothing - the
    // default, and what every run without a state file passes. Called once, before any
    // call is answered.
    //
    // Reading the inventories back is also what tells the id counter where to resume.
    // An item id is handed to a game, so the next one this run mints has to clear every
    // id a previous run already used - otherwise a promotion granted here is an item the
    // same player was given last time, and a game holding two items with one id draws one.
    void attach(Store* store);

    // What one inventory call resolved to. False means this world has nothing to say about it -
    // not an inventory call at all, or one naming a result handle no player was given - which
    // leaves the scenario and the session their say, in that order.
    bool answer(const Session& session, const std::string& call, const Json& args, Answer& out);

    // The catalogue, and what one player holds: neither is visible from the answer to one call.
    const std::vector<ItemDefinition>& definitions() const noexcept { return _definitions; }
    const std::vector<ItemDetails>& inventory_of(std::uint64_t steam_id) const noexcept;

private:
    // One result handle, and what a game reading it gets: the items it was handed and which
    // player's inventory they are.
    struct Result {
        std::uint64_t owner = 0;
        std::vector<ItemDetails> items;
    };

    void add_definition(std::int32_t id, const char* name, const char* description);

    // The promotions this player is entitled to and does not hold yet, added to the inventory
    // they were entitled to. This is the one place an item ever appears.
    std::vector<ItemDetails> grant_promotions(std::uint64_t steam_id);

    std::int32_t remember(std::uint64_t owner, std::vector<ItemDetails> items);
    const Result* find_result(std::int32_t handle) const noexcept;

    std::vector<ItemDefinition> _definitions;
    std::map<std::uint64_t, std::vector<ItemDetails>> _inventories;
    std::vector<std::pair<std::int32_t, Result>> _results;
    std::int32_t _next_result = kFirstResultHandle;
    std::uint64_t _next_item = kFirstItemId;

    // Where an item is also kept, or null when this run keeps nothing. Not owned; the
    // server that attached it outlives this world.
    Store* _store = nullptr;
};

}  // namespace steammock
