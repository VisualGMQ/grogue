#include "grid_panel_render.hpp"

void GridPanelRenderSystem::Update(engine::Entity* entity) {
    if (!entity) return;

    auto gridPanel = entity->GetComponent<component::GridPanel>();
    if (!gridPanel) return;

    int row = std::ceil(gridPanel->capacity / static_cast<float>(gridPanel->gridNumInCol));
    int width = gridPanel->gridNumInCol * gridPanel->gridSize + (gridPanel->gridNumInCol + 1) * gridPanel->gridPadding;
    int height = row * gridPanel->gridSize + (row + 1) * gridPanel->gridPadding;
    auto rect = engine::Rect((engine::Video::GetInitSize().w - width) / 2,
                            (engine::Video::GetInitSize().h - height) / 2,
                            width, height);
    if (gridPanel->position.x != -1 || gridPanel->position.y != -1) {
        rect.position = gridPanel->position;
    }

    drawBackground(gridPanel, rect, engine::Color{100, 100, 100}, engine::Color::Black);

    for (int y = 0; y < row; y++) {
        for (int x = 0; x < gridPanel->gridNumInCol; x++) {
            if (x + y * gridPanel->gridNumInCol >= gridPanel->capacity) {
                return;
            }

            engine::Rect gridRect(rect.position.x + (x + 1) * gridPanel->gridPadding + x * gridPanel->gridSize,
                                  rect.position.y + (y + 1) * gridPanel->gridPadding + y * gridPanel->gridSize,
                                  gridPanel->gridSize, gridPanel->gridSize);

            drawOneGrid(gridPanel, gridRect, engine::Color{200, 200, 200}, engine::Color::Black);

            if (gridPanel->drawImageFunc) {
                gridPanel->drawImageFunc(gridPanel, gridRect, x, y);
            }

            if (gridPanel->showCursor && engine::Vec2(x, y) == gridPanel->GetHoverGridPos()) {
                drawSelectImage(gridPanel->selectOutline, gridRect);
            }
        }
    }
}

void GridPanelRenderSystem::drawBackground(component::GridPanel* gridPanel, const engine::Rect& rect, const engine::Color& bgColor, const engine::Color& borderColor) {
    engine::Renderer::SetDrawColor(bgColor);
    engine::Renderer::FillRect(rect);
    engine::Renderer::SetDrawColor(borderColor);
    engine::Renderer::DrawRect(rect);
}

void GridPanelRenderSystem::drawOneGrid(component::GridPanel* gridPanel, const engine::Rect& rect, const engine::Color& bgColor, const engine::Color& borderColor) {
    engine::Renderer::SetDrawColor(engine::Color{200, 200, 200});
    engine::Renderer::FillRect(rect);

    engine::Renderer::SetDrawColor(engine::Color{0, 0, 0});
    engine::Renderer::DrawRect(rect);
}

void GridPanelRenderSystem::drawSelectImage(const engine::Image& image, const engine::Rect& rect) {
    if (!image) return;
    engine::Renderer::DrawTexture(*image.texture,
                                  &image.region,
                                  rect.size,
                                  {rect.position});

}
