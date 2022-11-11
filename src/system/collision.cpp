#include "collision.hpp"

void CollisionSystem::Update() {
    auto map = MapManager::GetGroundMap();

    // map collide monsters
    if (map) {
        collideMonsterAndMap(map);    
    }

    // monsters collide monsters
}

void CollisionSystem::collideMonsterAndMap(Map* map) {
    auto visiableArea = CalcVisiableTileArea(map, GameData::Instance().GetCamera().GetPosition());
        for (int y = visiableArea.initRows; y < visiableArea.endRows; y++) {
            for (int x = visiableArea.initCols; x < visiableArea.endCols; x++) {
                auto& tile = map->Get(x, y);
                if (tile.object && tile.object->GetComponent<component::Architecture>().IsOk()) {
                    component::BoxCollider* archBox = nullptr;
                    MATCH_INTO_VAR_OR(tile.object->GetComponent<component::BoxCollider>(), archBox, continue;);

                    engine::Rect archRect = archBox->rect;
                    archRect.position += engine::Vec2(x, y) * TileSize;

                    auto monsterIt = MonsterManager::begin();
                    while (monsterIt != MonsterManager::end()) {
                        engine::Node2DComponent* monsterNode = nullptr;
                        component::BoxCollider* monsterBox = nullptr;
                        component::RigidBody* monsterRigid = nullptr;
                        component::Sprite* monsterSprite = nullptr;

                        MATCH_INTO_VAR_OR((*monsterIt)->GetComponent<engine::Node2DComponent>(), monsterNode,
                                          monsterIt ++; continue;);
                        MATCH_INTO_VAR_OR((*monsterIt)->GetComponent<component::BoxCollider>(), monsterBox,
                                          monsterIt ++; continue;);
                        MATCH_INTO_VAR_OR((*monsterIt)->GetComponent<component::RigidBody>(), monsterRigid,
                                          monsterIt ++; continue;);
                        MATCH_INTO_VAR_OR((*monsterIt)->GetComponent<component::Sprite>(), monsterSprite,
                                          monsterIt ++; continue;);

                        engine::Rect box(monsterBox->rect.position + monsterNode->globalPosition, monsterBox->rect.size);
                        
                        collideMonsterWithMapTile(archRect, monsterNode, monsterRigid, box);
                        moveMonsterIntoMap(map, monsterNode, box, monsterBox->rect, monsterSprite->image.region.size);
                        monsterIt ++;
                    }
                }
            }
        }
}

void CollisionSystem::collideMonsterWithMapTile(const engine::Rect& archRect, engine::Node2DComponent* monsterNode, component::RigidBody* monsterRigid, engine::Rect& box) {
    CollisionResult collideResult;
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
}

void CollisionSystem::moveMonsterIntoMap(Map* map, engine::Node2DComponent* node2d, const engine::Rect& globalColliBox, const engine::Rect& localColliBox, const engine::Size& imgSize) {
    if (globalColliBox.position.x < 0) {
        node2d->globalPosition.x = -localColliBox.position.x;
    }
    if (globalColliBox.position.y < 0) {
        node2d->globalPosition.y = -localColliBox.position.y;
    }
    if (globalColliBox.position.x + globalColliBox.size.w > map->GetSize().w * TileSize) {
        float offset = globalColliBox.position.x + globalColliBox.size.w - node2d->globalPosition.x;
        node2d->globalPosition.x = map->GetSize().w * TileSize - offset;
    }
    if (globalColliBox.position.y + globalColliBox.size.h > map->GetSize().h * TileSize) {
        float offset = globalColliBox.position.y + globalColliBox.size.h - node2d->globalPosition.y;
        node2d->globalPosition.y = map->GetSize().h * TileSize - offset;
    }
}