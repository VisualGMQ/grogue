#include "monster_action.hpp"

void MonsterAction::Move(component::Sprite* sprite, engine::Node2DComponent* node2d, component::Human* human, component::RigidBody* rigid, const engine::Vec2& v) {
    if (v == engine::Vec2{0, 0}) return;

    if (v.x < v.y) {
        if (v.y < 0) {
            sprite->image = human->up;
            node2d->scale.Set(1, 1);
            sprite->offset.Set(0, 0);
        } else {
            sprite->image = human->down;
            node2d->scale.Set(1, 1);
            sprite->offset.Set(0, 0);
        }
    } else {
        if (v.x < 0) {
            sprite->image = human->right;
            node2d->scale.Set(-1, 1);
            sprite->offset.Set(8, 0);
        } else {
            sprite->image = human->right;
            node2d->scale.Set(1, 1);
            sprite->offset.Set(0, 0);
        }
    }
    rigid->velocity += v;
}

void MonsterAction::Pickup(component::Backpack* backpack) {
    auto& obj = GameData::Instance()->GetPickableObjGridPos();
    if (obj) {
        auto& mapObj = MapManager::GetGroundMap()->Get(obj->x, obj->y);
        if (mapObj.object) {
            if (backpack->AddObject(mapObj.object)) {
                mapObj.object = nullptr;
            }
        }
    }
    GameData::Instance()->ClearPickableObjGridPos();
}