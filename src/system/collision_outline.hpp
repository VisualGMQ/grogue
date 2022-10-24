#pragma once
#include "engine/engine.hpp"
#include "others/config.hpp"
#include "components/sprite.hpp"
#include "components/box_collider.hpp"
#include "others/data.hpp"
#include "collision/collision.hpp"

class CollisionOutlineSystem: public engine::EntityUpdateSystem {
public:
    CollisionOutlineSystem(engine::World* world): engine::EntityUpdateSystem(world) {}

    void Update(engine::Entity*) override;
};