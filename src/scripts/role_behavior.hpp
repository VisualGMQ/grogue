#pragma once

#include "engine/engine.hpp"
#include "sword_behavior.hpp"

class RoleBehavior: public engine::Behavior {
public:
    void OnInit() override;
    void OnUpdate() override;
    void OnQuit() override;

private:
    engine::SpriteComponent* sprite_;
    engine::Node2DComponent* transform_; 
    engine::Node2DComponent* swordTransform_;
};