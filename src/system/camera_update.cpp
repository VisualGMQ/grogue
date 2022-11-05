#include "camera_update.hpp"

void CameraUpdateSystem::Update() {
    auto& camera = GameData::Instance()->GetCamera();
    engine::Rect cameraRect;
    cameraRect.size = engine::Video::GetInitSize();
    auto player = GameData::Instance()->GetPlayer();

    if (!player) return;
    auto node2d = player->GetComponent<engine::Node2DComponent>();
    auto box = player->GetComponent<component::BoxCollider>();
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
