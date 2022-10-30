#include "human_controller.hpp"
#include "others/data.hpp"

HumanController::HumanController(engine::Entity* entity): entity_(entity) {
    node2d_ = entity->GetComponent<engine::Node2DComponent>();
    rigid_ = entity->GetComponent<component::RigidBody>();
    human_ = entity->GetComponent<component::Human>();
    sprite_ = entity->GetComponent<component::Sprite>();
    backpack_ = entity->GetComponent<component::Backpack>();
}

void HumanController::Walk(const engine::Vec2& velocity) {
    rigid_->velocity = velocity;
}

void HumanController::Pickup() {
    auto& obj = GameData::Instance()->GetPickableObjGridPos();
    if (obj) {
        auto& mapObj = MapManager::GetGroundMap()->Get(obj->x, obj->y);
        if (mapObj.object) {
            if (backpack_->AddObject(mapObj.object)) {
                mapObj.object = nullptr;
            }
        }
    }
    GameData::Instance()->ClearPickableObjGridPos();
}

void HumanController::Update() {
    constexpr float speed = 0.2;
    engine::Vec2 velocity;
    if (engine::Input::IsKeyPressing(SDL_SCANCODE_A)) {
        sprite_->image = human_->right;
        node2d_->scale.Set(-1, 1);
        sprite_->offset.Set(8, 0);
        velocity += engine::Vec2(-speed, 0);
    }
    if (engine::Input::IsKeyPressing(SDL_SCANCODE_D)) {
        sprite_->image = human_->right;
        node2d_->scale.Set(1, 1);
        sprite_->offset.Set(0, 0);
        velocity += engine::Vec2(speed, 0);
    }
    if (engine::Input::IsKeyPressing(SDL_SCANCODE_W)) {
        sprite_->image = human_->up;
        node2d_->scale.Set(1, 1);
        sprite_->offset.Set(0, 0);
        velocity += engine::Vec2(0, -speed);
    }
    if (engine::Input::IsKeyPressing(SDL_SCANCODE_S)) {
        sprite_->image = human_->down;
        node2d_->scale.Set(1, 1);
        sprite_->offset.Set(0, 0);
        velocity += engine::Vec2(0, speed);
    }
    if (velocity.x != 0 && velocity.y != 0) {
        constexpr float sqrt2 = 1.4142135623730951 / 2.0;
        velocity.x *= sqrt2;
        velocity.y *= sqrt2;
    }
    Walk(velocity);
    if (engine::Input::IsKeyPressed(SDL_SCANCODE_J)) {
        Pickup();
    }
    if (engine::Input::IsKeyPressed(SDL_SCANCODE_E)) {
        GameData::Instance()->ChangeController(GameData::Instance()->GetBackpackController());
        GameData::Instance()->GetBackpackPanel()->SetActive(true);
    }
    if (engine::Input::IsKeyPressed(SDL_SCANCODE_TAB)) {
        GameData::Instance()->ChangeController(GameData::Instance()->GetCompositeController());
        GameData::Instance()->GetCompositePanel()->SetActive(true);
        GameData::Instance()->GetCompositeDescriptionPanel()->SetActive(true);
    }
}
