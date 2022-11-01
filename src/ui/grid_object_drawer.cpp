#include "grid_object_drawer.hpp"

void GridObjectDrawer::operator()(component::GridPanel* panel, const engine::Rect& rect, int x, int y) {
    auto object = GetObject(panel, x + y * panel->gridNumInCol);
    if (!object.image) return;

    drawObject(panel, object.image, rect.position + (rect.size - object.image.region.size) * 0.5);

    if (object.number.has_value()) {
        auto font = engine::FontFactory::Find("simhei");
        if (!font) return;

        auto strSize = font->SizeUTF8(std::to_string(object.number.value()));
        drawObjectNum(font, object.number.value(), rect.position + rect.size - strSize, engine::Color::Blue);
    }
}

void GridObjectDrawer::drawObject(component::GridPanel* panel, const engine::Image& image, const engine::Vec2& pos) {
    engine::Transform transform{pos};
    engine::Renderer::DrawTexture(*image.texture,
                                  &image.region,
                                  engine::Size(TileSize, TileSize),
                                  transform);
}

void GridObjectDrawer::drawObjectNum(engine::Font* font, int number, const engine::Vec2& pos, const engine::Color& color) {
    auto numStr = std::to_string(number);
    engine::Renderer::DrawText(font, numStr, pos, color);
}