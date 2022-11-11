#include "put_hint.hpp"

void PutHintSystem::Update() {
    engine::Entity* player;
    MATCH_INTO_VAR_OR_RETURN_VOID(GameData::Instance().GetPlayer(), player);

    auto playerPos = player->GetComponent<engine::Node2DComponent>().Except("player don't has Node2DComponent")->globalPosition;

    component::BoxCollider* box;
    MATCH_INTO_VAR_OR_RETURN_VOID(player->GetComponent<component::BoxCollider>(), box);

    auto gridPos = (playerPos + box->rect.position + box->rect.size  * 0.5) / engine::Size(TileSize, TileSize);
    gridPos.x = std::floor(gridPos.x);
    gridPos.y = std::floor(gridPos.y);

    component::Human* human;
    MATCH_INTO_VAR_OR_RETURN_VOID(player->GetComponent<component::Human>(), human);
    auto orientation = human->orientation;

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

    GameData::Instance().SetPutTargetGridPos(gridPos);

    engine::Renderer::SetDrawColor(engine::Color::Red);
    auto& cameraPos = GameData::Instance().GetCamera().GetPosition();
    engine::Renderer::DrawRect(engine::Rect(gridPos.x * TileSize - cameraPos.x, gridPos.y * TileSize - cameraPos.y, TileSize, TileSize));
}
