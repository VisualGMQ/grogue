#pragma once

#include "engine/engine.hpp"
#include "components/backpack.hpp"
#include "grid_panel.hpp"
#include "others/config.hpp"
#include "others/data.hpp"
#include "components/sprite.hpp"
#include "ui/grid_panel.hpp"
#include "others/common.hpp"

class GridObjectDrawer {
public:
    virtual ~GridObjectDrawer() = default;
    void operator()(component::GridPanel*, const engine::Rect& rect, int x, int y);

    struct DrawResources {
        engine::Image image;
        std::optional<int> number;
    };

    virtual DrawResources GetObject(component::GridPanel* panel, int index) = 0;

private:
    void drawObject(component::GridPanel*, const engine::Image&, const engine::Vec2& pos);
    void drawObjectNum(engine::Font* font, int number, const engine::Vec2& pos, const engine::Color&);
};