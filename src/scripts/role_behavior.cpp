#include "scripts/role_behavior.hpp"

void RoleBehavior::OnInit() {
    transform_ = Parent()->GetComponent<engine::Node2DComponent>();
    transform_->zIndex = 0;
    transform_->position = engine::Vec2(100, 100);
    transform_->scale.Set(3, 3);

    initRoleBody();

    swordTransform_ = engine::SceneMgr::CurrentScene()->FindEntity("sword")->GetComponent<engine::Node2DComponent>();
    swordTransform_->zIndex = 1;
    swordAnim_.reset(new engine::Animation<float, true>({
                     engine::KeyFrame(engine::Radians(0.0f), 0),
                     engine::KeyFrame(engine::Radians(160.0f), 100),
                     engine::KeyFrame(engine::Radians(0.0f), 130),
                }, swordTransform_->rotation));
    auto tilesheet = engine::TileSheetFactory::Find("role");
    runAnim_.reset(new engine::Animation<engine::ImageInfo, false>({
                   engine::KeyFrame(engine::ImageInfo(tilesheet->Get(2, 3)), 0),
                   engine::KeyFrame(engine::ImageInfo(tilesheet->Get(1, 3)), 100),
                   engine::KeyFrame(engine::ImageInfo(tilesheet->Get(2, 3)), 200),
                }, feet_->GetComponent<engine::SpriteComponent>()->image));

    controller_ = std::make_unique<PlayerController>(Parent(),
                                                     hair_, head_, body_, feet_,
                                                     engine::SceneMgr::CurrentScene()->FindEntity("sword"),
                                                     200, 1);
}

void RoleBehavior::OnUpdate() {
    if (engine::Input::IsKeyPressed(SDL_SCANCODE_J)) {
        if (!swordAnim_->IsPlay()) {
            swordAnim_->Play();
        }
    }
    auto tilesheet = engine::TileSheetFactory::Find("role");
    swordAnim_->Update();
    controller_->Update();
}

void RoleBehavior::OnQuit() {}

void RoleBehavior::initRoleBody() {
    hair_ = engine::SceneMgr::CurrentScene()->FindEntity("role hair");
    head_ = engine::SceneMgr::CurrentScene()->FindEntity("role head");
    body_ = engine::SceneMgr::CurrentScene()->FindEntity("role body");
    feet_ = engine::SceneMgr::CurrentScene()->FindEntity("role feet");

    auto tilesheet = engine::TileSheetFactory::Find("role");

    auto sprite = body_->GetComponent<engine::SpriteComponent>();
    sprite->image = tilesheet->Get(3, 1);

    sprite = feet_->GetComponent<engine::SpriteComponent>();
    sprite->image = tilesheet->Get(6, 1);

    sprite = head_->GetComponent<engine::SpriteComponent>();
    sprite->image = tilesheet->Get(2, 0);
    head_->GetComponent<engine::Node2DComponent>()->zIndex = 0;

    sprite = hair_->GetComponent<engine::SpriteComponent>();
    sprite->image = tilesheet->Get(1, 1);
    hair_->GetComponent<engine::Node2DComponent>()->zIndex = 0.01;
}
