#include "backpack_object_drawer.hpp"


void BackpackObjectDrawer::operator()(component::GridPanel* panel, const engine::Rect& rect, int x, int y) {
    auto player = GameData::Instance()->GetPlayer();
    if (!player) return;

    auto backpack = player->GetComponent<component::Backpack>();
    if (!backpack) return;

    int objIdx = x + y * panel->gridNumInCol;
    if (backpack->objects.size() <= objIdx) { return; }

    auto object = backpack->objects[objIdx];

    auto sprite = object->GetComponent<component::Sprite>();

    if (!sprite || !sprite->image) return;

    drawObject(backpack, panel, sprite->image, rect.position + (rect.size - sprite->size) * 0.5);

    auto pickupable = object->GetComponent<component::Pickupable>();
    if (!pickupable || pickupable->num == 1) return;

    auto font = engine::FontFactory::Find("simhei");
    if (!font) return;

    auto strSize = font->SizeUTF8(std::to_string(pickupable->num));
    drawObjectNum(font, pickupable->num, rect.position + rect.size - strSize, engine::Color::Blue);
}

void BackpackObjectDrawer::drawObject(component::Backpack* backpack, component::GridPanel* panel, const engine::Image& image, const engine::Vec2& pos) {
    engine::Transform transform{pos};
    engine::Renderer::DrawTexture(*image.texture,
                                  &image.region,
                                  engine::Size(TileSize, TileSize),
                                  transform);
}

void BackpackObjectDrawer::drawObjectNum(engine::Font* font, int number, const engine::Vec2& pos, const engine::Color& color) {
    auto numStr = std::to_string(number);
    engine::Renderer::DrawText(font, numStr, pos, color);
}

