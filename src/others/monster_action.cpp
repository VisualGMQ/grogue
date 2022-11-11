#include "monster_action.hpp"

void MonsterAction::Move(component::Sprite* sprite, engine::Node2DComponent* node2d, component::Human* human, component::RigidBody* rigid, const engine::Vec2& v) {
    if (v == engine::Vec2{0, 0}) return;
    rigid->velocity += v;
}

void MonsterAction::Pickup(component::Backpack* backpack) {
    auto& obj = GameData::Instance().GetPickableObjGridPos();
    if (obj.has_value()) {
        auto& mapObj = MapManager::GetGroundMap()->Get(obj->x, obj->y);
        if (mapObj.object) {
            if (backpack->AddObject(mapObj.object).IsOk()) {
                mapObj.object = nullptr;
            }
        }
    }
    GameData::Instance().ClearPickableObjGridPos();
}