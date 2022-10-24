#include "life_ui_render.hpp"

void LifeUIRenderSystem::Update() {
    auto player = GameData::Instance()->GetPlayer();
    if (!player) return;

    auto life = player->GetComponent<component::Life>();
    if (!life) return;

    // TODO render life UI on screen
}