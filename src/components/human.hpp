#pragma once

#include "engine/engine.hpp"

namespace component {

class Human : public engine::Component {
public:
    Human(engine::ComponentID id): engine::Component(id) { Reset(); }

    void Reset() override {
        right.texture = nullptr;
        down.texture = nullptr;
        up.texture = nullptr;
        pickupRange = 20;
    }

    engine::Image right;
    engine::Image down;
    engine::Image up;
    float pickupRange;
};

}