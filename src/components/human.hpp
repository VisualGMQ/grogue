#pragma once

#include "engine/engine.hpp"

namespace component {

class Human : public engine::Component {
public:
    enum Orientation: unsigned int {
        None = 0x00,
        Left = 0x01,
        Right = 0x02,
        Up = 0x04,
        Down = 0x08,
        UpRight = Up|Right,
        UpLeft = Up|Left,
        DownRigth = Down|Right,
        DownLeft = Down|Left,
    };

    Human(engine::ComponentID id): engine::Component(id) { Reset(); }

    void Reset() override {
        engine::Component::Reset();

        right.texture = nullptr;
        down.texture = nullptr;
        up.texture = nullptr;
        pickupRange = 20;
        orientation = Down;
    }

    engine::Image right;
    engine::Image down;
    engine::Image up;
    Orientation orientation;
    float pickupRange;
};

}