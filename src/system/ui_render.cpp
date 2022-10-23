#include "ui_render.hpp"

void UIRenderSystem::Update(engine::Entity* entity) {
    if (!entity) return;

    auto player = GameData::Instance()->GetPlayer();
    if (!player) return;

    if (auto backpackPanel = entity->GetComponent<component::BackpackPanel>(); backpackPanel) {
        int row = backpackPanel->capacity / backpackPanel->gridNumInCol;
        int width = backpackPanel->gridNumInCol * backpackPanel->gridSize + (backpackPanel->gridNumInCol + 1) * backpackPanel->gridPadding;
        int height = row * backpackPanel->gridSize + (row + 1) * backpackPanel->gridPadding;

        // draw background
        engine::Renderer::SetDrawColor(engine::Color{100, 100, 100});
        auto rect = engine::Rect((engine::Video::GetInitSize().w - width) / 2,
                                 (engine::Video::GetInitSize().h - height) / 2,
                                 width, height);
        engine::Renderer::FillRect(rect);
        engine::Renderer::SetDrawColor(engine::Color{0, 0, 0});
        engine::Renderer::DrawRect(rect);

        auto backpack = player->GetComponent<component::Backpack>();

        // draw grid
        for (int y = 0; y < row; y++) {
            for (int x = 0; x < backpackPanel->gridNumInCol; x++) {
                engine::Renderer::SetDrawColor(engine::Color{50, 50, 50});
                engine::Rect gridRect(rect.position.x + (x + 1) * backpackPanel->gridPadding + x * backpackPanel->gridSize,
                                      rect.position.y + (y + 1) * backpackPanel->gridPadding + y * backpackPanel->gridSize,
                                      backpackPanel->gridSize, backpackPanel->gridSize);
                engine::Renderer::FillRect(gridRect);

                if (engine::Vec2(x, y) == backpackPanel->GetHoverGridPos()) {
                    engine::Renderer::SetDrawColor(engine::Color{0, 0, 255});
                } else {
                    engine::Renderer::SetDrawColor(engine::Color{0, 0, 0});
                }
                engine::Renderer::DrawRect(gridRect);

                int objectIdx = x + y * backpackPanel->gridNumInCol;
                if (backpack && objectIdx < backpack->objects.size()) {
                    auto object = backpack->objects[objectIdx];
                    auto sprite = object ->GetComponent<component::Sprite>();
                    engine::Transform transform;
                    transform.position = gridRect.position;
                    transform.position += engine::Vec2((backpackPanel->gridSize - TileSize) / 2.0,
                                                       (backpackPanel->gridSize - TileSize) / 2.0);
                    engine::Renderer::DrawTexture(*sprite->image.texture,
                                                  &sprite->image.region,
                                                  engine::Size(TileSize, TileSize),
                                                  transform);

                    auto pickup = object ->GetComponent<component::Pickupable>();
                    auto font = engine::FontFactory::Find("simhei");
                    auto numStr = std::to_string(pickup->num);
                    auto fontSize = font->SizeUTF8(numStr);
                    engine::Renderer::DrawText(font, numStr,
                                               transform.position + engine::Vec2(backpackPanel->gridSize, backpackPanel->gridSize) - fontSize);
                }
            }
        }
    }
}