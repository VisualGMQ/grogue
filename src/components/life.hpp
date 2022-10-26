#pragma once

#include "engine/engine.hpp"

namespace component {

class Life: public engine::Component {
public:
    Life(engine::ComponentID id): engine::Component(id) { Reset(); }
    void Reset() override {
        engine::Component::Reset();
        maxHp = 100;
        hp = maxHp;
        maxEnergy = 100;
        energy = maxEnergy;
    }

    float hp;
    float maxHp;
    float energy;
    float maxEnergy;
};
    
}
