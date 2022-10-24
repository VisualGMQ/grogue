#pragma once
#include "engine/engine.hpp"
#include "others/config.hpp"
#include "components/life.hpp"
#include "others/data.hpp"
#include "map/map.hpp"

class LifeUIRenderSystem: public engine::PerFrameSystem {
public:
    LifeUIRenderSystem(engine::World* world): engine::PerFrameSystem(world) {}

    void Update() override;
};