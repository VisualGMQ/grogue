#include "game/game.hpp"

void PlayerMove(Keyboard& keyboard, Monster& monster) {
    constexpr float SPEED = 2;

    math::Vector2 velocity;

    if (keyboard.Key(KEY_D).IsPressing() ||
        keyboard.Key(KEY_A).IsPressing()) {
        if (keyboard.Key(KEY_D).IsPressing()) {
            velocity.Set(1.0, 0.0);
        } else {
            velocity.Set(-1.0, 0.0);
        }
    }
    if (keyboard.Key(KEY_W).IsPressing() ||
        keyboard.Key(KEY_S).IsPressing()) {
        if (keyboard.Key(KEY_S).IsPressing()) {
            velocity += math::Vector2(0, 1);
        } else {
            velocity += math::Vector2(0, -1);
        }
    }

    velocity.Normalize();
    velocity *= SPEED;
    monster.Move(velocity);
}

void ToggleBackpackUIPanel(ecs::Querier querier, ecs::Resources res) {
    auto entities = querier.Query<BackpackUIPanel>();
    if (!entities.empty()) {
        auto& window = res.Get<Window>();
        auto& transform = querier.Get<ui::RectTransform>(entities[0]);
        float& y = transform.transform.localTransform.position.y;
        if (y >= 0 &&
            y < window.GetSize().y) {  // panel in windows, move it out
            y = window.GetSize().y;
        } else {
            auto& info = res.Get<GameConfig>().GetBackpackUIConfig().Info();
            y = window.GetSize().y - info.height;
        }
    }
}

void PickupItemOnTile(Monster& monster, Backpack& backpack, ecs::Commands& cmd,
                      ecs::Querier querier, ecs::Resources resources,
                      ecs::Events& events) {
    constexpr float PickupHalfRange = 100;
    auto& map = resources.Get<MapManager>().GetCurrentMap();
    auto& hover = resources.Get<NearestItemHover>();
    if (map->tiles.IsInRange(hover.position.x, hover.position.y) &&
        !map->tiles.Get(hover.position.x, hover.position.y).items.empty()) {
        auto& items = map->tiles.Get(hover.position.x, hover.position.y).items;
        if (items.empty()) {
            return;
        }
        backpack.items.push_back(items.back());
        items.pop_back();

        uint32_t newItemCount = 1;
        auto& signalMgr = resources.Get<SignalManager>();
        signalMgr.Raise(SignalCallback::UpdateBackpackItem, cmd, querier,
                        resources, events, &newItemCount);
    }
}