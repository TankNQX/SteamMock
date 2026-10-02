// include/bridge/inventory.hpp for what an inventory is here, and where it splits from an
// item's definition.
//
// The shape is the lobby world's and the board world's, deliberately: a world answers only what
// it has grounds for, says which calls those are, and hands a game the payload that completes
// the call it just made. A call it declines is reported as unanswered, which is how the
// scenario and the session keep their say.

#include "bridge/inventory.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "bridge/store.hpp"

namespace steammock {
namespace {

// The calls this world answers, by the names the SDK's own surface sends. Every one of them is
// an ISteamInventory call, and they are the ones Spacewar reaches on its way to any screen that
// shows an inventory - plus the three that ask for an item to be made or moved, which this
// world answers *without* making one.
constexpr const char* kLoadItemDefinitions = "SteamAPI_ISteamInventory_LoadItemDefinitions";
constexpr const char* kSendItemDropHeartbeat = "SteamAPI_ISteamInventory_SendItemDropHeartbeat";
constexpr const char* kGrantPromoItems = "SteamAPI_ISteamInventory_GrantPromoItems";
constexpr const char* kGetAllItems = "SteamAPI_ISteamInventory_GetAllItems";
constexpr const char* kTriggerItemDrop = "SteamAPI_ISteamInventory_TriggerItemDrop";
constexpr const char* kExchangeItems = "SteamAPI_ISteamInventory_ExchangeItems";
constexpr const char* kGenerateItems = "SteamAPI_ISteamInventory_GenerateItems";
constexpr const char* kGetResultItems = "SteamAPI_ISteamInventory_GetResultItems";
constexpr const char* kGetItemDefinitionProperty =
    "SteamAPI_ISteamInventory_GetItemDefinitionProperty";
constexpr const char* kCheckResultSteamID = "SteamAPI_ISteamInventory_CheckResultSteamID";
constexpr const char* kDestroyResult = "SteamAPI_ISteamInventory_DestroyResult";

// The SDK's own enumerations written out, because this tree carries no Valve enumeration and a
// value typed as a number is what the wire has anyway.
constexpr std::int32_t kResultOK = 1;  // k_EResultOK

// The two payloads a result arrives as, in the SDK's own order: a full update first, which says
// "this is everything you hold" rather than "this changed", and the result-ready that ends it.
// Spacewar's own comment on its handler says the first one "triggers immediately before" the
// second, which is the order the SDK documents and the order these are queued in.
constexpr const char* kFullUpdate = "SteamInventoryFullUpdate_t";
constexpr const char* kResultReady = "SteamInventoryResultReady_t";

// The name of a property, as `GetItemDefinitionProperty` asks for one. The three Spacewar reads
// are the three the catalogue carries, and a property a definition does not have is answered
// for rather than left unanswered - see the answer below.
constexpr const char* kNameProperty = "name";
constexpr const char* kDescriptionProperty = "description";
constexpr const char* kIconProperty = "icon_url";

// Everything this world answers is labelled "inventory", so a transcript says which calls came
// out of a player's items rather than out of a scenario, a session, a room or a board.
constexpr const char* kInventoryVia = "inventory";

// --- reading what a call was given -----------------------------------------

std::int64_t int_member(const Json& object, const char* key, std::int64_t fallback) {
    const Json* value = json_member(object, key);
    if (value == nullptr || !value->is_number()) {
        return fallback;
    }
    return as_int64(*value);
}

std::uint64_t id_member(const Json& object, const char* key) {
    const Json* value = json_member(object, key);
    if (value == nullptr || !value->is_number()) {
        return 0;
    }
    return as_uint64(*value);
}

std::string text_member(const Json& object, const char* key) {
    const Json* value = json_member(object, key);
    if (value == nullptr || !value->is_string()) {
        return std::string();
    }
    return std::string(as_string(*value));
}

// A result handle, as a game passes one: the SDK's own type is 32-bit, and -1 is the invalid
// one - which is what Spacewar starts every one of its handles at.
std::int32_t handle_member(const Json& args) {
    return static_cast<std::int32_t>(int_member(args, "resultHandle", -1));
}

// --- answers ---------------------------------------------------------------

Answer answered(Json ret) {
    Answer answer;
    answer.answered = true;
    answer.ret = std::move(ret);
    return answer;
}

Answer from_inventory(Json ret) {
    Answer answer = answered(std::move(ret));
    answer.via = kInventoryVia;
    return answer;
}

Json payload_of(const char* name, Json fields) {
    Json event = Json::object();
    event["event"] = Json(name);
    event["in"] = std::move(fields);
    return event;
}

Json ready_fields(std::int32_t handle, std::int32_t result) {
    Json fields = Json::object();
    fields["m_handle"] = Json(static_cast<std::int64_t>(handle));
    fields["m_result"] = Json(static_cast<std::int64_t>(result));
    return fields;
}

Json handle_fields(std::int32_t handle) {
    Json fields = Json::object();
    fields["m_handle"] = Json(static_cast<std::int64_t>(handle));
    return fields;
}

// One item, as the fields of `SteamItemDetails_t` - the structure a game's own array is made
// of, and what the stub writes each element of that array from. The members are the layouts'
// names, because that is what the generated store looks for.
Json item_json(const ItemDetails& item) {
    Json fields = Json::object();
    fields["m_itemId"] = Json(item.item_id);
    fields["m_iDefinition"] = Json(static_cast<std::int64_t>(item.definition));
    fields["m_unQuantity"] = Json(static_cast<std::int64_t>(item.quantity));
    fields["m_unFlags"] = Json(static_cast<std::int64_t>(item.flags));
    return fields;
}

// What a result handle arrives as: the payloads a game is waiting for, and nothing else. A
// full update only for a result that is the whole inventory.
Json result_events(std::int32_t handle, bool full_update) {
    Json events = Json::array();
    if (full_update) {
        events.push_back(payload_of(kFullUpdate, handle_fields(handle)));
    }
    events.push_back(payload_of(kResultReady, ready_fields(handle, kResultOK)));
    return events;
}

// A call whose result is a handle *the game owns the pointer to*: the SDK's `pResultHandle` is
// an out parameter rather than the return value, which is why the handle is in the out block
// and not in the reply's return - a game that passed a null pointer there gets the payloads and
// no handle, which is exactly what Spacewar's `GetAllItems( NULL )` asks for.
Answer from_inventory_holding(std::int32_t handle, bool full_update) {
    Answer answer = from_inventory(Json(true));
    answer.out = Json::object();
    answer.out["pResultHandle"] = Json(static_cast<std::int64_t>(handle));
    answer.events = result_events(handle, full_update);
    return answer;
}

}  // namespace

InventoryWorld::InventoryWorld() {
    // Spacewar's own item ids, from the game's own header (steamworksexample/Inventory.h): the
    // list a timed drop comes out of, the four ship decorations, the two weapons and the two
    // specials. The *names* are this world's, because a real app's come from its item schema
    // and a harness has no way to read one - and they are what a game draws, since
    // `GetItemDefinitionProperty( def, "name", buf, &size )` is how a game asks for one.
    //
    // The icon is a URL that cannot resolve, on purpose: it is not a picture of anything and
    // nothing should ever fetch it. Spacewar does not draw item icons at all - its own
    // DrawInventory says "todo: divide up so can draw image" - so this is only ever text a
    // game may print.
    add_definition(10, "Timed Drop List", "The list a timed drop is chosen from.");
    add_definition(100, "Ship Decoration 1", "A red hull.");
    add_definition(101, "Ship Decoration 2", "A blue hull.");
    add_definition(102, "Ship Decoration 3", "A green hull.");
    add_definition(103, "Ship Decoration 4", "A black hull.");
    add_definition(110, "Ship Weapon 1", "A faster gun.");
    add_definition(111, "Ship Weapon 2", "A heavier gun.");
    add_definition(120, "Ship Special 1", "A first ship special.");
    add_definition(121, "Ship Special 2", "A second ship special.");
}

void InventoryWorld::add_definition(std::int32_t id, const char* name, const char* description) {
    ItemDefinition definition;
    definition.id = id;
    definition.name = name;
    definition.description = description;
    definition.icon_url = "http://example.invalid/steammock/items/" + std::to_string(id) + ".png";
    _definitions.push_back(std::move(definition));
}

void InventoryWorld::attach(Store* store) {
    _store = store;
    if (store == nullptr) {
        return;
    }

    // The catalogue is not read back, because it is not a game's to change: the definitions
    // above are this app's own item schema as far as a harness can know it, and the same
    // every run. What a store holds is what *players* hold.
    store->load_inventories(_inventories);
    for (const auto& entry : _inventories) {
        for (const ItemDetails& item : entry.second) {
            if (item.item_id >= _next_item) {
                _next_item = item.item_id + 1;
            }
        }
    }
}

std::vector<std::string> InventoryWorld::handled_calls() {
    return {
        kLoadItemDefinitions,       kSendItemDropHeartbeat, kGrantPromoItems, kGetAllItems,
        kTriggerItemDrop,           kExchangeItems,         kGenerateItems,   kGetResultItems,
        kGetItemDefinitionProperty, kCheckResultSteamID,    kDestroyResult,
    };
}

const std::vector<ItemDetails>&
InventoryWorld::inventory_of(std::uint64_t steam_id) const noexcept {
    // A player nobody has granted anything to holds nothing, which is a real answer and not a
    // missing one - so this is an empty list rather than a pointer a caller has to test.
    static const std::vector<ItemDetails> kNothing;
    const auto found = _inventories.find(steam_id);
    return found != _inventories.end() ? found->second : kNothing;
}

std::vector<ItemDetails> InventoryWorld::grant_promotions(std::uint64_t steam_id) {
    // Which promotions an app has and who is entitled to them is the app's own business, and
    // this world has no way to read it - so the policy is written down here instead, and it is
    // the two items Spacewar's own test path names (`GrantTestItems` calls `GenerateItems` with
    // 100 and 101). A player who already holds one is not given a second: an inventory is a set
    // of instances, and granting a definition twice is how a game ends up drawing two of the
    // same hull.
    static constexpr std::int32_t kPromotions[] = {100, 101};

    std::vector<ItemDetails>& inventory = _inventories[steam_id];
    std::vector<ItemDetails> granted;
    for (const std::int32_t definition : kPromotions) {
        bool held = false;
        for (const ItemDetails& item : inventory) {
            if (item.definition == definition) {
                held = true;
                break;
            }
        }
        if (held) {
            continue;
        }
        ItemDetails item;
        item.item_id = _next_item++;
        item.definition = definition;
        item.quantity = 1;
        granted.push_back(item);
        inventory.push_back(item);
        if (_store != nullptr) {
            // The one place an item ever appears, and so the one place one is ever kept.
            _store->save_item(steam_id, item);
        }
    }
    return granted;
}

std::int32_t InventoryWorld::remember(std::uint64_t owner, std::vector<ItemDetails> items) {
    const std::int32_t handle = _next_result++;
    Result result;
    result.owner = owner;
    result.items = std::move(items);
    _results.emplace_back(handle, std::move(result));
    // The window, oldest first: a game that never destroys a result would otherwise hold this
    // for the whole run - and Spacewar destroys every one it is given, which is what makes the
    // window a bound rather than a leak.
    while (_results.size() > kMaxResults) {
        _results.erase(_results.begin());
    }
    return handle;
}

const InventoryWorld::Result* InventoryWorld::find_result(std::int32_t handle) const noexcept {
    for (const auto& [stored_handle, result] : _results) {
        if (stored_handle == handle) {
            return &result;
        }
    }
    return nullptr;
}

bool InventoryWorld::answer(const Session& session, const std::string& call, const Json& args,
                            Answer& out) {
    const std::uint64_t me = session.profile().steam_id;

    if (call == kLoadItemDefinitions) {
        // The catalogue is this world's own, so there is nothing to load and nothing to fail:
        // a game told no here asks no further questions, and its items have no names.
        out = from_inventory(Json(true));
        return true;
    }

    if (call == kSendItemDropHeartbeat) {
        // "This game is still being played", which a real Steam uses to decide when a timed
        // drop is due. There is no clock here and nothing is awarded for playing, so this is
        // answered and nothing happens.
        out = from_inventory(Json(true));
        return true;
    }

    if (call == kGrantPromoItems) {
        // What this player qualifies for and does not hold yet. The result of this call is the
        // items it granted - the SDK's own words - so it is the delta and not the inventory.
        const std::int32_t handle = remember(me, grant_promotions(me));
        out = from_inventory_holding(handle, false);
        return true;
    }

    if (call == kGetAllItems) {
        // Everything held, as a full update, which is the form a game replaces its own list
        // from rather than one it adds to.
        const std::int32_t handle = remember(me, inventory_of(me));
        out = from_inventory_holding(handle, true);
        return true;
    }

    // The three calls that ask this world to make or move items. Each is answered with a result
    // that holds nothing, and no inventory changes: an item appears here by being granted with
    // the promotions above, which is what keeps a run's inventory a record of what a game was
    // given rather than of what it asked for. Answering at all is what tells a game its request
    // was heard - including the `TriggerItemDrop` a real Steam would eventually have a drop for
    // and this world never does.
    if (call == kTriggerItemDrop || call == kExchangeItems || call == kGenerateItems) {
        const std::int32_t handle = remember(me, std::vector<ItemDetails>());
        out = from_inventory_holding(handle, false);
        return true;
    }

    if (call == kGetResultItems) {
        const Result* result = find_result(handle_member(args));
        if (result == nullptr || result->owner != me) {
            // A handle this run never handed out, or another player's: there is no list to
            // read. Reported as unanswered, so a scenario keeps its say and a game that
            // invented a handle gets its own buffer back untouched.
            return false;
        }
        Json items = Json::array();
        for (const ItemDetails& item : result->items) {
            items.push_back(item_json(item));
        }
        out = from_inventory(Json(true));
        out.out = Json::object();
        // No count here on purpose: the buffer that is written owns it, because the caller's
        // own pointer is where the number belongs - see ArrayOut in bridge/synth.hpp.
        out.out["pOutItemsArray"] = std::move(items);
        return true;
    }

    if (call == kGetItemDefinitionProperty) {
        const std::int32_t id = static_cast<std::int32_t>(int_member(args, "iDefinition", 0));
        const std::string wanted = text_member(args, "pchPropertyName");
        const ItemDefinition* definition = nullptr;
        for (const ItemDefinition& candidate : _definitions) {
            if (candidate.id == id) {
                definition = &candidate;
                break;
            }
        }
        std::string value;
        bool found = false;
        if (definition != nullptr) {
            if (wanted == kNameProperty) {
                value = definition->name;
                found = true;
            } else if (wanted == kDescriptionProperty) {
                value = definition->description;
                found = true;
            } else if (wanted == kIconProperty) {
                value = definition->icon_url;
                found = true;
            }
        }
        if (!found) {
            // A definition this app does not have, or a property it does not carry. Answered
            // *no* rather than left unanswered: the catalogue is this world's, so "no" is this
            // world's answer, and a game that shows "(unknown)" for it is showing something
            // true about the run.
            out = from_inventory(Json(false));
            return true;
        }
        out = from_inventory(Json(true));
        out.out = Json::object();
        // And again no length: what the value needs, terminator included, goes back through
        // the caller's own pointer - see TextOut in bridge/synth.hpp.
        out.out["pchValueBuffer"] = Json(value);
        return true;
    }

    if (call == kCheckResultSteamID) {
        const Result* result = find_result(handle_member(args));
        out = from_inventory(
            Json(result != nullptr && result->owner == id_member(args, "steamIDExpected")));
        return true;
    }

    if (call == kDestroyResult) {
        const std::int32_t handle = handle_member(args);
        for (auto entry = _results.begin(); entry != _results.end(); ++entry) {
            if (entry->first == handle) {
                _results.erase(entry);
                break;
            }
        }
        out = from_inventory(Json(true));
        return true;
    }

    return false;
}

}  // namespace steammock
