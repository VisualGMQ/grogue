#pragma once

#include "engine/engine.hpp"
#include "ui/hand_frame.hpp"
#include "others/config.hpp"
#include "others/common.hpp"
#include "components/sprite.hpp"
#include "components/pickupable.hpp"

class HandFrameRenderSystem: public engine::EntityUpdateSystem {
public:
    HandFrameRenderSystem(engine::World* world): engine::EntityUpdateSystem(world) {}

    void Update(engine::Entity* entity) override;
};