#pragma once

#include "engine/engine.hpp"

class Controller {
public:
    virtual void Walk(const engine::Vec2& pos) = 0;
    virtual void Update() = 0;
};