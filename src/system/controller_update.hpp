#pragma once
#include "engine/engine.hpp"
#include "others/config.hpp"
#include "components/sprite.hpp"
#include "others/data.hpp"

class ControllerUpdateSystem: public engine::PerFrameSystem {
public:
    ControllerUpdateSystem(engine::World* world): engine::PerFrameSystem(world) {}

    void Update() override;
};