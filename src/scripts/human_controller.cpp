#pragma once

#include "human_controller.hpp"

HumanController::HumanController(engine::Entity* entity): entity_(entity) {
    node2d_ = entity->GetComponent<engine::Node2DComponent>();
    human_ = entity->GetComponent<component::Human>();
    sprite_ = entity->GetComponent<component::Sprite>();
}

void HumanController::Walk(const engine::Vec2& pos) {
    node2d_->position = pos;
}

void HumanController::Update() {

    constexpr int speed = 1;
    if (engine::Input::IsKeyPressing(SDL_SCANCODE_A)) {
        Walk(node2d_->position + engine::Vec2(-speed, 0));
        sprite_->image = human_->right;
        node2d_->scale.Set(-1, 1);
        sprite_->offset.Set(8, 0);
    }
    if (engine::Input::IsKeyPressing(SDL_SCANCODE_D)) {
        Walk(node2d_->position + engine::Vec2(speed, 0));
        sprite_->image = human_->right;
        node2d_->scale.Set(1, 1);
        sprite_->offset.Set(0, 0);
    }
    if (engine::Input::IsKeyPressing(SDL_SCANCODE_W)) {
        Walk(node2d_->position + engine::Vec2(0, -speed));
        sprite_->image = human_->up;
        node2d_->scale.Set(1, 1);
        sprite_->offset.Set(0, 0);
    }
    if (engine::Input::IsKeyPressing(SDL_SCANCODE_S)) {
        Walk(node2d_->position + engine::Vec2(0, speed));
        sprite_->image = human_->down;
        node2d_->scale.Set(1, 1);
        sprite_->offset.Set(0, 0);
    }
}