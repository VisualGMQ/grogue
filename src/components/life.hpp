#pragma once

#include "engine/engine.hpp"

namespace component {

class Life: public engine::Component {
public:
    Life(engine::ComponentID id): engine::Component(id) {}
    void Reset() override {
        engine::Component::Reset();
        hp = 100;
        energy = 1000;
    }

    int hp;
    int energy;
};
    
}
