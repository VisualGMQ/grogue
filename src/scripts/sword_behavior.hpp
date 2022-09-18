#pragma once

#include "engine/engine.hpp"

class SwordBehavior: public engine::Behavior {
public:
    void OnInit() override;
    void OnUpdate() override;
    void OnQuit() override;

    engine::SpriteComponent* sprite;
    engine::Node2DComponent* transform;
};