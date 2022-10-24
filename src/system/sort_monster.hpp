#pragma once

#include "engine/engine.hpp"
#include "others/data.hpp"

class SortMonstersSystem: public engine::PerFrameSystem {
public:
    SortMonstersSystem(engine::World* world): engine::PerFrameSystem(world) { }

    void Update() override;
};