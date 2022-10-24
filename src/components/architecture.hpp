#pragma once

#include "engine/engine.hpp"

namespace component {

class Architecture : public engine::Component {
public:
    Architecture(engine::ComponentID id): engine::Component(id) { Reset(); }
    void Reset() override {
        engine::Component::Reset();
        id = -1;
    }

    ObjectID id;
};
    
}