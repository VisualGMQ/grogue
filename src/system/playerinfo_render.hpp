#pragma once
#include "engine/engine.hpp"
#include "others/config.hpp"
#include "components/sprite.hpp"
#include "components/life.hpp"
#include "others/data.hpp"

class PlayerInfoRenderSystem: public engine::PerFrameSystem {
public:
    PlayerInfoRenderSystem(engine::World* world): engine::PerFrameSystem(world) {}

    void Update() override;

private:
    void drawBar(const engine::Rect& bar, float ratio, const engine::Color& bgColor, const engine::Color& foreColor, const engine::Color& borderColor);
};