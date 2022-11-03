#pragma once
#include "engine/engine.hpp"
#include "components/rigidbody.hpp"
#include "components/human.hpp"
#include "components/sprite.hpp"

class OrientationUpdateSystem: public engine::EntityUpdateSystem {
public:
    OrientationUpdateSystem(engine::World* world): engine::EntityUpdateSystem(world) {}

    void Update(engine::Entity*) override;
};