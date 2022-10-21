#include "maptile_render.hpp"

void MapTileRenderSystem::Update(engine::Entity* entity) {
    if (!entity) return;

    if (auto node = entity->GetComponent<engine::Node2DComponent>(); node) {
        if (auto map = entity->GetComponent<component::MapComponent>(); map && map->map) {
            int initX = std::floor(node->globalPosition.x / TileSize);
            int initY = std::floor(node->globalPosition.y / TileSize);
            int col = std::min(std::ceil(engine::Video::GetInitSize().w / TileSize), map->map->GetSize().w);
            int row = std::min(std::ceil(engine::Video::GetInitSize().h / TileSize), map->map->GetSize().h);
            engine::Vec2 offset(node->globalPosition.x - initX * TileSize, node->globalPosition.y - initY * TileSize);
            for (int x = 0; x < col; x++) {
                for (int y = 0; y < row; y++) {
                    auto& tile = map->map->Get(initX + x, initY + y);
                    if (tile.terrian.image.texture) {
                        engine::Renderer::DrawTexture(*tile.terrian.image.texture,
                                                      &tile.terrian.image.region,
                                                      engine::Size(TileSize, TileSize),
                                                      engine::Transform{node->globalPosition + engine::Vec2(x * TileSize, y * TileSize) + offset, node->globalScale, node->globalRotation, engine::Vec2{0, 0}});
                    }
                    // TODO extract this to another render system
                    if (tile.object && tile.object->image) {
                        engine::Renderer::DrawTexture(*tile.object->image.texture,
                                                      &tile.object->image.region,
                                                      engine::Size(TileSize, TileSize),
                                                      engine::Transform{node->globalPosition + engine::Vec2(x * TileSize, y * TileSize) + offset, node->globalScale, node->globalRotation, engine::Vec2{0, 0}});
                    }
                }
            }
        }
    }
}