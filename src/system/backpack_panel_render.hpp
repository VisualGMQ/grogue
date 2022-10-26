#pragma once

#include "engine/engine.hpp"
#include "ui/backpack_panel.hpp"
#include "others/data.hpp"
#include "components/backpack.hpp"

class BackpackPanelRenderSystem: public engine::EntityUpdateSystem {
public:
    BackpackPanelRenderSystem(engine::World* world): engine::EntityUpdateSystem(world) {}

    void Update(engine::Entity*);

    void drawBackground(component::BackpackPanel*, const engine::Rect&, const engine::Color& bgColor, const engine::Color& borderColor);
    void drawOneGrid(component::BackpackPanel*, const engine::Rect&, const engine::Color& bgColor, const engine::Color& borderColor);
    void drawOneObject(component::Backpack*, component::BackpackPanel*, int objectIdx, const engine::Vec2& gridPos);
    void drawOneObjectNum(engine::Font* font, int number, const engine::Vec2& pos, const engine::Color&);
    void drawSelectImage(const engine::Image&, const engine::Rect&);
};