#include "hint_arrow.hpp"

void HintArrowSystem::Update() {
    auto map = MapManager::GetGroundMap();
    auto& player = GameData::Instance().GetPlayer().Except("no player");
    const auto& playerPos = player->GetComponent<engine::Node2DComponent>().Except("player don't has Node2DComponent")->globalPosition;
    float pickupRange = player->GetComponent<component::Human>().Except("player don' has Human component")->pickupRange;
    float pickupRangeSquare = pickupRange * pickupRange;
    if (!map) return ;

    auto visiableArea = CalcVisiableTileArea(map, GameData::Instance().GetCamera().GetPosition());

    engine::Vec2 arrowPosition;
    GameData::Instance().ClearPickableObjGridPos();
    float distance = std::numeric_limits<float>::max();

    auto sprite = player->GetComponent<component::Sprite>().Except("player don't has Sprite component");

    for (int x = visiableArea.initCols; x < visiableArea.endCols; x++) {
        for (int y = visiableArea.initRows; y < visiableArea.endRows; y++) {
            auto& tile = map->Get(x, y);
            if (tile.object && tile.object->GetComponent<component::Pickupable>().IsOk()) {
                engine::Vec2 objGlobalPos(x * TileSize, y * TileSize);
                engine::Vec2 dir = objGlobalPos - (playerPos + sprite->image.region.size * 0.5);
                float tileDistance = engine::Length2(dir);
                if (tileDistance <= pickupRangeSquare && tileDistance < distance) {
                    distance = tileDistance;
                    arrowPosition.Set(x * TileSize, y * TileSize - TileSize / 2.0);
                    GameData::Instance().SetPickableObjGridPos(engine::Vec2(x, y));
                }
            }
        }
    }

    if (image_.texture && GameData::Instance().GetPickableObjGridPos()) {
        engine::Transform transform;
        transform.position = arrowPosition - GameData::Instance().GetCamera().GetPosition();
        engine::Renderer::DrawTexture(*image_.texture, &image_.region,
                                        engine::Size(TileSize, TileSize),
                                        transform);
    }
}
