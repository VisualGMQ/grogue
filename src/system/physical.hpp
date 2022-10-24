#pragma once
#include "engine/engine.hpp"
#include "others/config.hpp"
#include "components/rigidbody.hpp"
#include "others/data.hpp"
#include "collision/collision.hpp"

class PhysicalSystem: public engine::EntityUpdateSystem {
public:
    PhysicalSystem(engine::World* world): engine::EntityUpdateSystem(world) {}

    void Update(engine::Entity*) override;
};