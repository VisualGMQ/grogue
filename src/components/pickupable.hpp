#pragma once

#include "engine/engine.hpp"

namespace component {

class Pickupable: public engine::Component {
public:
    Pickupable(engine::ComponentID id): engine::Component(id) { Reset(); }

    void Reset() override {}
};

}