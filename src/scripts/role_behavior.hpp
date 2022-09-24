#pragma once

#include "engine/engine.hpp"
#include "sword_behavior.hpp"
#include "controller.hpp"

class RoleBehavior: public engine::Behavior {
public:
    void OnInit() override;
    void OnUpdate() override;
    void OnQuit() override;

private:
    engine::Node2DComponent* transform_; 
    engine::Entity* hair_;
    engine::Entity* head_;
    engine::Entity* body_;
    engine::Entity* feet_;
    engine::Node2DComponent* swordTransform_;
    std::unique_ptr<engine::Animation<float, true>> swordAnim_;
    std::unique_ptr<engine::Animation<engine::ImageInfo, false>> runAnim_;
    std::unique_ptr<PlayerController> controller_;

    void initRoleBody();
    void initMoveMachine();
};
