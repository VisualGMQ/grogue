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
            if (!tile.object) {
                drawMonster(y);
            } else {
                if (tile.object->GetComponent<component::Pickupable>()) {
                    drawTile(tile, node, visiableArea, x, y);
                    drawMonster(y);
                } else if (tile.object->GetComponent<component::Architecture>()) {
                    drawMonster(y);
                    drawTile(tile, node, visiableArea, x, y);
                }
            }
        }
    }
}

void MapObjectRenderSystem::drawMonster(int y) {
    while (monsterIt_ != MonsterManager::cend()) {
        auto monsterNode = (*monsterIt_)->GetComponent<engine::Node2DComponent>();
        auto monsterSprite = (*monsterIt_)->GetComponent<component::Sprite>();
        auto monsterY = monsterNode->position.y + monsterSprite->size.h;
        if (monsterY <= (y + 1) * TileSize) {
            engine::Renderer::DrawTexture(*monsterSprite->image.texture,
                                            &monsterSprite->image.region,
                                            monsterSprite->size,
                                            engine::Transform{monsterNode->globalPosition,
                                                            monsterNode->globalScale,
                                                            monsterNode->globalRotation,
                                                            engine::Vec2{0, 0}});
        } else {
            break;
        }
        monsterIt_ ++;
    }
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