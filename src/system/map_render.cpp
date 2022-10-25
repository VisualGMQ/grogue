#include "map_render.hpp"

void MapRenderSystem::Update(engine::Entity* entity) {
    if (!entity) return;

    auto node = entity->GetComponent<engine::Node2DComponent>();
    auto map = entity->GetComponent<component::MapComponent>();

    if (!node || !map) return;

    auto visiableArea = CalcVisiableTileArea(map->map, engine::Vec2(0, 0));
    drawTileAndPickupableObj(visiableArea, map->map);
    drawMonsterAndArch(visiableArea, map->map);
}

void MapRenderSystem::drawOneTile(const engine::Image& image,  const engine::Transform& transform) {
    engine::Renderer::DrawTexture(*image.texture, &image.region, engine::Size(TileSize, TileSize), transform);
}

void MapRenderSystem::drawOneSprite(const component::Sprite* sprite, const engine::Transform& transform) {
    engine::Renderer::DrawTexture(*sprite->image.texture, &sprite->image.region, sprite->size, transform);
}

void MapRenderSystem::drawTileAndPickupableObj(const VisiableTileArea& visiableArea, Map* map) {
    for (int y = visiableArea.initRows; y < visiableArea.endRows; y++) {
        for (int x = visiableArea.initCols; x < visiableArea.endCols; x++) {
            auto& tile = map->Get(x, y);
            if (tile.terrian.image) {
                drawOneTile(tile.terrian.image, {engine::Vec2(x, y) * TileSize});
            }
            if (tile.object && tile.object->GetComponent<component::Pickupable>()) {
                auto sprite = tile.object->GetComponent<component::Sprite>();
                if (sprite) {
                    drawOneSprite(sprite, {engine::Vec2(x, y) * TileSize});
                }
            }
        }
    }
}

void MapRenderSystem::drawMonsterAndArch(const VisiableTileArea& visiableArea, Map* map) {
    auto monsterIt = MonsterManager::begin();

    engine::Node2DComponent* monsterNode = nullptr;
    component::Sprite* monsterSprite = nullptr;

    getNextMonsterInfo(monsterIt, monsterNode, monsterSprite);

    for (int y = visiableArea.initRows; y < visiableArea.endRows; y++) {
        std::optional<float> monsterBottom;
        if (monsterNode && monsterSprite) {
            monsterBottom = monsterNode->globalPosition.y + monsterSprite->size.h;
        }
        while (monsterIt != MonsterManager::end() && monsterBottom &&
               monsterBottom.value() >= y * TileSize && monsterBottom.value() <= (y + 1) * TileSize) {
            drawOneSprite(monsterSprite, {monsterNode->globalPosition, monsterNode->globalScale, monsterNode->globalRotation});

            monsterIt ++;
            getNextMonsterInfo(monsterIt, monsterNode, monsterSprite);
        }
        for (int x = visiableArea.initCols; x < visiableArea.endCols; x++) {
            auto& tile = map->Get(x, y);
            if (tile.object && tile.object->GetComponent<component::Architecture>()) {
                auto sprite = tile.object->GetComponent<component::Sprite>();
                if (sprite) {
                    drawOneSprite(sprite, {engine::Vec2(x, y) * TileSize});
                }
            }
        }
    }
}

void MapRenderSystem::getNextMonsterInfo(MonsterManager::iterator& it, Node2DPtr& node, SpritePtr& sprite) {
    if (it != MonsterManager::end()) {
        node = (*it)->GetComponent<engine::Node2DComponent>();
        sprite = (*it)->GetComponent<component::Sprite>();
    }
}