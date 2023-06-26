#include "game/game.hpp"

void PutItemIntoBackpack(Backpack& backpack, const Item& newItem, ecs::Commands& cmds, ecs::Querier querier, ecs::Resources res, ecs::Events& events) {
    bool cantPileup = true;
    int piledIdx = -1;

    for (int i = 0; i < backpack.items.size(); i++) {
        auto& item = backpack.items[i];
        if (item.nameID == newItem.nameID) {
            item.amount += 1;
            piledIdx = i;
            cantPileup = false;
        }
    }

    auto& signalMgr = res.Get<SignalManager>();
    BackpackUIPanelUpdateParam param;
    if (cantPileup) {
        backpack.items.push_back(newItem);
        param.type = BackpackUIPanelUpdateParam::Type::NewItem;
        param.value = 1;    // the new item count
    } else {
        param.type = BackpackUIPanelUpdateParam::Type::Pile;
        param.value = piledIdx;    // the piled item index
    }
    signalMgr.Raise(SignalCallback::UpdateBackpackItem, cmds, querier, res,
                    events, (void*)&param);
}

void PickupTileOneItem(Backpack& backpack, ecs::Commands& cmds,
                       ecs::Querier querier, ecs::Resources res,
                       ecs::Events& events) {
    auto& map = res.Get<MapManager>().GetCurrentMap();
    const auto& hover = res.Get<NearestItemHover>();

    math::Vector2 coord(std::floor(hover.position.x),
                        std::floor(hover.position.y));

    if (!hover.valid || !map->tiles.IsInRange(coord.x, coord.y)) {
        return;
    }

    auto& tile = map->tiles.Get(coord.x, coord.y);
    auto& items = querier.Get<MapTile>(tile).items;
    if (items.empty()) {
        return;
    }

    auto& newItem = items.back();
    PutItemIntoBackpack(backpack, newItem, cmds, querier, res, events);
    items.pop_back();
}

void PlayerMove(const Input& input, Monster& monster, physic::Particle& particle) {
    constexpr float SPEED = 50;

    auto axis = input.Axis();
    particle.vel = math::Vector2(axis.x * SPEED, -axis.y * SPEED);

    monster.Move(particle.vel);
}