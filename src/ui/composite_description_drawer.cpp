#include "composite_description_drawer.hpp"

void CompositeDescriptionDrawer::operator()(component::GridPanel* panel,
                                            const engine::Rect& rect,
                                            int x, int y) {
    // FIXME refactory it with CompositeController::changeDescriptionPanelCapacity
    auto& hoverPos = GameData::Instance()->GetCompositePanel()->GetComponent<component::GridPanel>()->GetHoverGridPos();
    int objectIdx = hoverPos.x + hoverPos.y * panel->gridNumInCol;
    auto it = ComposeConfigStorage::begin();
    std::advance(it, objectIdx);
    auto& materials = it->second.materials;

    int materialIdx = x + y * panel->gridNumInCol;
    auto& material = materials[materialIdx];

    auto materialConfig = ObjectConfigStorage::Find(material.id);
    engine::Renderer::DrawTexture(*materialConfig->image.texture,
                                  &materialConfig->image.region,
                                  engine::Size(TileSize, TileSize),
                                  {rect.position});

    auto font = engine::FontFactory::Find("simhei");
    if (!font) return;

    auto numStr = std::to_string(material.num);
    auto strSize = font->SizeUTF8(numStr);

    engine::Renderer::DrawText(font, numStr, rect.position + rect.size - strSize, engine::Color::Blue);
}
