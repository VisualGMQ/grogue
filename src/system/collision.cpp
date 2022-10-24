#include "collision.hpp"

void CollisionSystem::Update() {
    auto map = MapManager::GetGroundMap();

    // map collide monsters
    if (map) {
        auto visiableArea = CalcVisiableTileArea(map, engine::Vec2(0, 0));
        for (int y = visiableArea.initRows; y < visiableArea.endRows; y++) {
            for (int x = visiableArea.initCols; x < visiableArea.endCols; x++) {
                auto& tile = map->Get(x, y);
                if (tile.object && tile.object->GetComponent<component::Architecture>()) {
                    // FIXME add offset
                    engine::Rect archRect(x * TileSize, y * TileSize, TileSize, TileSize);

                    auto monsterIt = MonsterManager::begin();
                    while (monsterIt != MonsterManager::end()) {
                        CollisionResult collideResult;
                        auto monsterNode = (*monsterIt)->GetComponent<engine::Node2DComponent>();
                        auto monsterBox = (*monsterIt)->GetComponent<component::BoxCollider>();
                        auto monsterRigid = (*monsterIt)->GetComponent<component::RigidBody>();
                        engine::Rect box(monsterBox->rect.position + monsterNode->globalPosition, monsterBox->rect.size);
                        if (IsRectIntersectRect(box, archRect)) {
                            if (MinkowskiCollide(box, archRect, monsterRigid->velocity, engine::Vec2{}, collideResult)) {
                                if (collideResult.depth != 0) {
                                    monsterNode->position += collideResult.normal * collideResult.depth;
                                    monsterRigid->velocity.Set(0, 0);
                                } else {
									monsterRigid->velocity = monsterRigid->velocity * collideResult.s;
                                }
                            }
                        }
                        monsterIt ++;
                    }
                }
            }
        }
    }

    // monsters collide monsters
}