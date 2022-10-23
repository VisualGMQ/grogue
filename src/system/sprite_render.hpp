#pragma once
#include "engine/engine.hpp"
#include "others/config.hpp"
#include "components/sprite.hpp"

class SpriteRenderSystem: public engine::EntityUpdateSystem {
public:
    SpriteRenderSystem(engine::World* world): engine::EntityUpdateSystem(world) {}

    void Update(engine::Entity*) override;
};