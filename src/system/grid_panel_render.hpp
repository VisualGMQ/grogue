#pragma once

#include "engine/engine.hpp"
#include "others/data.hpp"
#include "ui/grid_panel.hpp"

class GridPanelRenderSystem: public engine::EntityUpdateSystem {
public:
    GridPanelRenderSystem(engine::World* world): engine::EntityUpdateSystem(world) {}

    void Update(engine::Entity*);

    void drawBackground(component::GridPanel*, const engine::Rect&, const engine::Color& bgColor, const engine::Color& borderColor);
    void drawOneGrid(component::GridPanel*, const engine::Rect&, const engine::Color& bgColor, const engine::Color& borderColor);
    void drawSelectImage(const engine::Image&, const engine::Rect&);
};