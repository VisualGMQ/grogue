#include "playerinfo_render.hpp"

void PlayerInfoRenderSystem::Update() {
    if (!GameData::Instance()->GetPlayer()) return;

    auto life = GameData::Instance()->GetPlayer()->GetComponent<component::Life>();

    engine::Rect barRect(10, 10, 200, 10);
    drawBar(barRect, life->hp / life->maxHp, engine::Color{150, 0, 0}, engine::Color{255, 0, 0}, engine::Color{0, 0, 0});
    barRect.position.y += 15;
    drawBar(barRect, life->energy / life->maxEnergy, engine::Color{130, 120, 0}, engine::Color{255, 255, 0}, engine::Color{0, 0, 0});
}

void PlayerInfoRenderSystem::drawBar(const engine::Rect& bar, float ratio,
                                     const engine::Color& bgColor, const engine::Color& foreColor, const engine::Color& borderColor) {
    engine::Renderer::SetDrawColor(bgColor);
    engine::Renderer::FillRect(bar);
    auto ratioRect = bar;
    ratioRect.size.w *= ratio;
    engine::Renderer::SetDrawColor(foreColor);
    engine::Renderer::FillRect(ratioRect);
    engine::Renderer::SetDrawColor(borderColor);
    engine::Renderer::DrawRect(bar);
}
