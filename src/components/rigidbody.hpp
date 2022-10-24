#pragma once

#include "engine/engine.hpp"

namespace component {

class RigidBody: public engine::Component {
public:
    RigidBody(engine::ComponentID id): engine::Component(id) { Reset(); }

    void Reset() override {
        engine::Component::Reset();

        velocity.Set(0, 0);
    }

    engine::Vec2 velocity;
};

}
