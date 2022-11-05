#include "put_hint.hpp"

void PutHintSystem::Update() {
    auto player = GameData::Instance()->GetPlayer();
    auto playerPos = player->GetComponent<engine::Node2DComponent>()->globalPosition;
    auto box = player->GetComponent<component::BoxCollider>();
    auto gridPos = (playerPos + box->rect.position + box->rect.size  * 0.5) / engine::Size(TileSize, TileSize);
    gridPos.x = std::floor(gridPos.x);
    gridPos.y = std::floor(gridPos.y);

    auto orientation = player->GetComponent<component::Human>()->orientation;
    if (orientation & component::Human::Left) {
        gridPos.x -= 1;
    }
    if (orientation & component::Human::Right) {
        gridPos.x += 1;
    }
    if (orientation & component::Human::Up) {
        gridPos.y -= 1;
    }
    if (orientation & component::Human::Down) {
        gridPos.y += 1;
    }

    GameData::Instance()->SetPutTargetGridPos(gridPos);

    engine::Renderer::SetDrawColor(engine::Color::Red);
    auto& cameraPos = GameData::Instance()->GetCamera().GetPosition();
    engine::Renderer::DrawRect(engine::Rect(gridPos.x * TileSize - cameraPos.x, gridPos.y * TileSize - cameraPos.y, TileSize, TileSize));
}
