#pragma once

#include "engine/engine.hpp"

namespace component {

class Human : public engine::Component {
public:
    Human(engine::ComponentID id): engine::Component(id) { Reset(); }

    void Reset() override {}

    engine::Image right;
    engine::Image down;
    engine::Image up;
};

}