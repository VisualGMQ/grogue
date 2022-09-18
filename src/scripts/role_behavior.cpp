#include "scripts/role_behavior.hpp"

void RoleBehavior::OnInit() {
    sprite_ = Parent()->GetComponent<engine::SpriteComponent>();
    transform_ = Parent()->GetComponent<engine::Node2DComponent>();
    transform_->SetPosition(engine::Vec2(100, 100));
    auto tile = engine::TileSheetFactory::Find("role")->Get(0, 0);
    sprite_->texture = tile.texture;
    sprite_->region = tile.region;
    sprite_->size.Set(16 * 3, 16 * 3);

    swordTransform_ = engine::SceneMgr::CurrentScene()->FindEntity("sword")->GetComponent<engine::Node2DComponent>();
}

void RoleBehavior::OnUpdate() {
    if (engine::Input::IsKeyPressed(SDL_SCANCODE_J)) {
        swordTransform_->SetRotation(engine::Radians(90));
    }
    if (engine::Input::IsKeyReleased(SDL_SCANCODE_J)) {
        swordTransform_->SetRotation(engine::Radians(0));
    }
}

void RoleBehavior::OnQuit() {}