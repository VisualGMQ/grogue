#pragma once
#include "engine/engine.hpp"

namespace component {

class Terrian: public engine::Component {
public:
    Terrian(engine::ComponentID id): engine::Component(id) { Reset(); }

    void Reset() override {
        engine::Component::Reset();
        type = Solid;
    }

    enum Type {
        Solid = 0,
        Liquid = 1,
    } type;
};

}