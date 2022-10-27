#pragma once

#include "engine/engine.hpp"
#include "ui/grid_panel.hpp"
#include "others/data.hpp"
#include "components/backpack.hpp"

class CompositePanelRenderSystem: public engine::PerFrameSystem{
public:
    CompositePanelRenderSystem(engine::World* world): engine::PerFrameSystem(world) {}

    void Update();
};