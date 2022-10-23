#include "hint_arrow.hpp"

void HintArrowSystem::Update() {
    auto map = MapManager::GetGroundMap();
    const auto& playerPos = GameData::Instance()->GetPlayer()->GetComponent<engine::Node2DComponent>()->globalPosition;
    float pickupRange = GameData::Instance()->GetPlayer()->GetComponent<component::Human>()->pickupRange;
    float pickupRangeSquare = pickupRange * pickupRange;
    if (!map) return ;

    auto visiableArea = CalcVisiableTileArea(map, engine::Vec2(0, 0));

    engine::Vec2 arrowPosition;
    GameData::Instance()->ClearPickableObjGridPos();
    float distance = std::numeric_limits<float>::max();

    for (int x = visiableArea.initCols; x < visiableArea.endCols; x++) {
        for (int y = visiableArea.initRows; y < visiableArea.endRows; y++) {
            auto& tile = map->Get(x, y);
            if (tile.object && tile.object->GetComponent<component::Pickupable>()) {
                engine::Vec2 objGlobalPos(x * TileSize, y * TileSize);
                engine::Vec2 dir = objGlobalPos - playerPos;
                float tileDistance = engine::Length2(dir);
                if (tileDistance <= pickupRangeSquare && tileDistance < distance) {
                    distance = tileDistance;
                    arrowPosition.Set(x * TileSize, y * TileSize - TileSize / 2.0);
                    GameData::Instance()->SetPickableObjGridPos(engine::Vec2(x, y));
                }
            }
        }
    }

    if (image_.texture && GameData::Instance()->GetPickableObjGridPos()) {
        engine::Transform transform;
        transform.position = arrowPosition;
        engine::Renderer::DrawTexture(*image_.texture, &image_.region,
                                        engine::Size(TileSize, TileSize),
                                        transform);
    }
}