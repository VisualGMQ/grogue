#include "mapobject_render.hpp"

void MapObjectRenderSystem::Update(engine::Entity* entity) {
    if (!entity) return;

    monsterIt_ = MonsterManager::begin();

    auto node = entity->GetComponent<engine::Node2DComponent>();
    auto map = entity->GetComponent<component::MapComponent>();
    if (!node || !map) return;

    auto visiableArea = CalcVisiableTileArea(map->map, engine::Vec2(0, 0));
    for (int y = visiableArea.initRows; y < visiableArea.endRows; y++) {
        for (int x = visiableArea.initCols; x < visiableArea.endCols; x++) {
            auto& tile = map->map->Get(x, y);
            auto monsterNode = (*monsterIt_)->GetComponent<engine::Node2DComponent>();
            auto monsterSprite = (*monsterIt_)->GetComponent<component::Sprite>();
            float monsterY = monsterNode->position.y + monsterSprite->size.h;
            if (tile.object) {
                drawTile(tile, node, visiableArea, x, y);
            }
        }
    }
    while (monsterIt_ != MonsterManager::end()) {
        drawMonster((*monsterIt_)->GetComponent<component::Sprite>(), (*monsterIt_)->GetComponent<engine::Node2DComponent>());
        monsterIt_++;
    }
}

void MapObjectRenderSystem::drawMonster(component::Sprite* sprite, engine::Node2DComponent* node) {
    engine::Renderer::DrawTexture(*sprite->image.texture,
                                   &sprite->image.region,
                                   sprite->size,
                                   engine::Transform{node->globalPosition,
                                                     node->globalScale,
                                                     node->globalRotation,
                                                     engine::Vec2{0, 0}});

}

void MapObjectRenderSystem::drawTile(MapTile& tile, engine::Node2DComponent* node,  const VisiableTileArea& visiableArea, int x, int y) {
    if (tile.object && tile.object->GetComponent<component::Sprite>()) {
        auto sprite = tile.object->GetComponent<component::Sprite>();
        if (!sprite->image.texture) return;
        engine::Renderer::DrawTexture(*sprite->image.texture,
                                        &sprite->image.region,
                                        sprite->size,
                                        engine::Transform{node->globalPosition + engine::Vec2(x * TileSize, y * TileSize) + visiableArea.offset,
                                                          node->globalScale,
                                                          node->globalRotation,
                                                          engine::Vec2{0, 0}});
    }
}