#include "composite_object_drawer.hpp"

void CompositeObjectDrawer::operator()(component::GridPanel* panel,
                                       const engine::Rect& rect,
                                       int x, int y) {
    int objectIdx = x + y * panel->gridNumInCol;
    if (objectIdx >= panel->capacity || objectIdx >= ComposeConfigStorage::Size()) {
        return;
    }
    auto it = ComposeConfigStorage::begin();
    std::advance(it, objectIdx);
    auto config = ObjectConfigStorage::Find(it->first);
    if (!config) {
        Loge("composite object {} not found", it->first);
        return;
    }

    if (config->image) {
        engine::Renderer::DrawTexture(*config->image.texture,
                                      &config->image.region,
                                      engine::Size(TileSize, TileSize),
                                      {rect.position});
    }
}
