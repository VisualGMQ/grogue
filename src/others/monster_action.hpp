#pragma once

#include "engine/engine.hpp"
#include "components/human.hpp"
#include "components/sprite.hpp"
#include "components/rigidbody.hpp"
#include "components/backpack.hpp"
#include "others/data.hpp"
#include "map/map.hpp"

class MonsterAction final {
public:
    MonsterAction() = delete;

    static void Move(component::Sprite* sprite, engine::Node2DComponent* node2d, component::Human* human, component::RigidBody* rigid, const engine::Vec2& v);
    static void Pickup(component::Backpack* backpack);
};