#include "composite_panel_render.hpp"

void CompositePanelRenderSystem::Update() {
    auto player = GameData::Instance()->GetPlayer();
    if (!player) return;

    auto backpack = player->GetComponent<component::Backpack>();


}