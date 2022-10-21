#pragma once

#include "engine/engine.hpp"

class TransformUpdateSystem: public engine::EntityUpdateSystem {
public:
    TransformUpdateSystem(engine::World* world): engine::EntityUpdateSystem(world) {}

    void Update(engine::Entity*);
};