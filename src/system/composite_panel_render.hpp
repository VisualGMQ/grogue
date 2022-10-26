#pragma once

#include "engine/engine.hpp"
#include "ui/backpack_panel.hpp"
#include "others/data.hpp"
#include "components/backpack.hpp"

class CompositePanelRenderSystem: public engine::PerFrameSystem{
public:
    CompositePanelRenderSystem(engine::World* world): engine::PerFrameSystem(world) {}

    void Update();
};