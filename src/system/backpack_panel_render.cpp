#include "backpack_panel_render.hpp"

void BackpackPanelRenderSystem::Update(engine::Entity* entity) {
    if (!entity) return;

    auto player = GameData::Instance()->GetPlayer();
    if (!player) return;

    if (auto backpackPanel = entity->GetComponent<component::BackpackPanel>(); backpackPanel) {

        int row = backpackPanel->capacity / backpackPanel->gridNumInCol;
        int width = backpackPanel->gridNumInCol * backpackPanel->gridSize + (backpackPanel->gridNumInCol + 1) * backpackPanel->gridPadding;
        int height = row * backpackPanel->gridSize + (row + 1) * backpackPanel->gridPadding;
        auto rect = engine::Rect((engine::Video::GetInitSize().w - width) / 2,
                                (engine::Video::GetInitSize().h - height) / 2,
                                width, height);

        drawBackground(backpackPanel, rect, engine::Color{100, 100, 100}, engine::Color::Black);

        // draw grid
        for (int y = 0; y < row; y++) {
            for (int x = 0; x < backpackPanel->gridNumInCol; x++) {
                engine::Rect gridRect(rect.position.x + (x + 1) * backpackPanel->gridPadding + x * backpackPanel->gridSize,
                                      rect.position.y + (y + 1) * backpackPanel->gridPadding + y * backpackPanel->gridSize,
                                      backpackPanel->gridSize, backpackPanel->gridSize);

                drawOneGrid(backpackPanel, gridRect, engine::Color{200, 200, 200}, engine::Color::Black);

                if (engine::Vec2(x, y) == backpackPanel->GetHoverGridPos()) {
                    drawSelectImage(backpackPanel->selectOutline, gridRect);
                }

                int objectIdx = x + y * backpackPanel->gridNumInCol;
                auto backpack = player->GetComponent<component::Backpack>();
                if (backpack && objectIdx < backpack->objects.size()) {
                    drawOneObject(backpack, backpackPanel, objectIdx, gridRect.position);

                    auto pickup = backpack->objects[objectIdx]->GetComponent<component::Pickupable>();
                    auto font = engine::FontFactory::Find("simhei");
                    engine::Vec2 position = gridRect.position + engine::Vec2(backpackPanel->gridSize, backpackPanel->gridSize) - font->SizeUTF8(std::to_string(pickup->num));
                    drawOneObjectNum(font, pickup->num, position, engine::Color::White);
                }
            }
        }
    }
}

void BackpackPanelRenderSystem::drawBackground(component::BackpackPanel* backpackPanel, const engine::Rect& rect, const engine::Color& bgColor, const engine::Color& borderColor) {
    engine::Renderer::SetDrawColor(bgColor);
    engine::Renderer::FillRect(rect);
    engine::Renderer::SetDrawColor(borderColor);
    engine::Renderer::DrawRect(rect);
}

void BackpackPanelRenderSystem::drawOneGrid(component::BackpackPanel* backpackPanel, const engine::Rect& rect, const engine::Color& bgColor, const engine::Color& borderColor) {
    engine::Renderer::SetDrawColor(engine::Color{200, 200, 200});
    engine::Renderer::FillRect(rect);

    engine::Renderer::SetDrawColor(engine::Color{0, 0, 0});
    engine::Renderer::DrawRect(rect);
}

void BackpackPanelRenderSystem::drawOneObject(component::Backpack* backpack, component::BackpackPanel* panel, int objectIdx, const engine::Vec2& gridPos) {
    auto object = backpack->objects[objectIdx];
    auto sprite = object ->GetComponent<component::Sprite>();
    engine::Transform transform{gridPos};
    transform.position += engine::Vec2((panel->gridSize - TileSize) * 0.5,
                                       (panel->gridSize - TileSize) * 0.5);
    component::DrawSprite(sprite, transform);
}

void BackpackPanelRenderSystem::drawOneObjectNum(engine::Font* font, int number, const engine::Vec2& pos, const engine::Color& color) {
    auto numStr = std::to_string(number);
    engine::Renderer::DrawText(font, numStr, pos, color);
}

void BackpackPanelRenderSystem::drawSelectImage(const engine::Image& image, const engine::Rect& rect) {
    engine::Renderer::DrawTexture(*image.texture,
                                  &image.region,
                                  rect.size,
                                  {rect.position});

}