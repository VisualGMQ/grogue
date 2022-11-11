#include "camera_update.hpp"

void CameraUpdateSystem::Update() {
    auto& camera = GameData::Instance().GetCamera();
    engine::Rect cameraRect;
    cameraRect.size = engine::Video::GetInitSize();
    engine::Entity* player;
    MATCH_INTO_VAR_OR_RETURN_VOID(GameData::Instance().GetPlayer(), player);

    engine::Node2DComponent* node2d;
    MATCH_INTO_VAR_OR_RETURN_VOID(player->GetComponent<engine::Node2DComponent>(), node2d);

    component::BoxCollider* box;
    MATCH_INTO_VAR_OR_RETURN_VOID(player->GetComponent<component::BoxCollider>(), box);

    auto centerPos = node2d->globalPosition + box->rect.position + box->rect.size * 0.5;
    auto& canvaSize = engine::Video::GetInitSize();
    cameraRect.position = centerPos - canvaSize * 0.5;
    
    auto mapSize = MapManager::GetGroundMap()->GetSize() * TileSize;

    if (cameraRect.position.x + cameraRect.size.w > mapSize.w) cameraRect.position.x = mapSize.w - cameraRect.size.w;
    if (cameraRect.position.y + cameraRect.size.h > mapSize.h) cameraRect.position.y = mapSize.h - cameraRect.size.h;
    if (cameraRect.position.x < 0) cameraRect.position.x = 0;
    if (cameraRect.position.y < 0) cameraRect.position.y = 0;

    camera.MoveTo(cameraRect.position);
}
