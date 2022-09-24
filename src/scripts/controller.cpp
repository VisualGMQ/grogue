#include "controller.hpp"

WalkCommand::WalkCommand(engine::Vec2& ownerVel,
                         engine::Entity* hair, engine::Entity* head, engine::Entity* body, engine::Entity* feet,
                         engine::Animation<engine::ImageInfo, false>* anim, engine::Node2DComponent* transform,
                         int tilesheetRow)
: hair_(hair), head_(head), body_(body), feet_(feet), ownerVel_(ownerVel), anim_(anim), transform_(transform), tilesheetRow_(tilesheetRow) {
    tilesheet_ = engine::TileSheetFactory::Find("role");
}

void WalkCommand::SetVelocity(const engine::Vec2& velocity) {
    velocity_ = velocity;
}

void WalkCommand::Execute() {
    if (velocity_.x == 0 && velocity_.y == 0) return;
    if (!anim_->IsPlay()) {
        anim_->Play();
    }
    anim_->Update();

    if (velocity_.x < 0) {
        head_->GetComponent<engine::SpriteComponent>()->image = tilesheet_->Get(0, 0);
        hair_->GetComponent<engine::SpriteComponent>()->image = tilesheet_->Get(0, tilesheetRow_);
        body_->GetComponent<engine::SpriteComponent>()->image = tilesheet_->Get(3, tilesheetRow_);
        transform_->scale.x = -abs(transform_->scale.x);
    }
    if (velocity_.x > 0) {
        head_->GetComponent<engine::SpriteComponent>()->image = tilesheet_->Get(0, 0);
        hair_->GetComponent<engine::SpriteComponent>()->image = tilesheet_->Get(0, tilesheetRow_);
        body_->GetComponent<engine::SpriteComponent>()->image = tilesheet_->Get(3, tilesheetRow_);
        transform_->scale.x = abs(transform_->scale.x);
    }
    if (velocity_.y > 0) {
        head_->GetComponent<engine::SpriteComponent>()->image = tilesheet_->Get(1, 0);
        hair_->GetComponent<engine::SpriteComponent>()->image = tilesheet_->Get(1, tilesheetRow_);
        body_->GetComponent<engine::SpriteComponent>()->image = tilesheet_->Get(4, tilesheetRow_);
    }
    if (velocity_.y < 0) {
        head_->GetComponent<engine::SpriteComponent>()->image = tilesheet_->Get(2, 0);
        hair_->GetComponent<engine::SpriteComponent>()->image = tilesheet_->Get(2, tilesheetRow_);
        body_->GetComponent<engine::SpriteComponent>()->image = tilesheet_->Get(5, tilesheetRow_);
    }

}

PlayerController::PlayerController(engine::Entity* entity,
                                   engine::Entity* hair, engine::Entity* head, engine::Entity* body, engine::Entity* feet,
                                   float speed, int tilesheetRow)
    : entity_(entity), hair_(hair), head_(head), body_(body), feet_(feet), speed_(speed), tilesheetRow_(tilesheetRow) {
    auto sprite = feet_->GetComponent<engine::SpriteComponent>();
    auto tilesheet = engine::TileSheetFactory::Find("role");
    walkAnim_.reset(new engine::Animation<engine::ImageInfo, false>({
                engine::KeyFrame<engine::ImageInfo>(engine::ImageInfo(tilesheet->Get(7, tilesheetRow_)), 0),
                engine::KeyFrame<engine::ImageInfo>(engine::ImageInfo(tilesheet->Get(8, tilesheetRow_)), 100),
                engine::KeyFrame<engine::ImageInfo>(engine::ImageInfo(tilesheet->Get(7, tilesheetRow_)), 200),
            }, sprite->image));
    walk_ = std::make_unique<WalkCommand>(vel_,
                                          hair_, head_, body_, feet_,
                                          walkAnim_.get(), entity->GetComponent<engine::Node2DComponent>(),
                                          tilesheetRow);

    tilesheet_ = engine::TileSheetFactory::Find("role");
}

void PlayerController::Update() {
    vel_.Set(0, 0);
    feet_->GetComponent<engine::SpriteComponent>()->image = tilesheet_->Get(6, tilesheetRow_);
    float diagFactor = std::sqrt(2) / 2.0;
    bool isVerticalKeyPressed = false;
    if (engine::Input::IsKeyPressing(SDL_SCANCODE_A)) {
        vel_.x -= speed_;
        isVerticalKeyPressed = true;
    }
    if (engine::Input::IsKeyPressing(SDL_SCANCODE_D)) {
        vel_.x += speed_;
        isVerticalKeyPressed = true;
    }
    if (engine::Input::IsKeyPressing(SDL_SCANCODE_W)) {
        vel_.y -= speed_;
        if (isVerticalKeyPressed) {
            vel_ *= diagFactor;
        }
    }
    if (engine::Input::IsKeyPressing(SDL_SCANCODE_S)) {
        vel_.y += speed_;
        if (isVerticalKeyPressed) {
            vel_ *= diagFactor;
        }
    }
    
    walk_->SetVelocity(vel_);
    walk_->Execute();
    entity_->GetComponent<engine::Node2DComponent>()->position += vel_ * engine::Timer::GetElapse() / 1000.0f;
}
