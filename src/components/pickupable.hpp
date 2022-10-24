#pragma once

#include "engine/engine.hpp"
#include "others/object_config.hpp"

namespace component {

class Pickupable: public engine::Component {
public:
    Pickupable(engine::ComponentID id): engine::Component(id) { Reset(); }

    void Reset() override {
        engine::Component::Reset();
        num = 1;
        id = -1;
    }

    int num;
    ObjectID id;
};

}