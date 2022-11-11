#include "human_controller.hpp"
#include "others/data.hpp"

HumanController::HumanController(engine::Entity* entity) {
    SetEntity(entity);
}

void HumanController::Update() {
    velocity_.Set(0, 0);

    if (up_) up_->Update();
    
    if (down_) down_->Update();
    if (left_) left_->Update();
    if (right_) right_->Update();

    if (velocity_.x != 0 && velocity_.y != 0) {
        constexpr float sqrt2 = 1.4142135623730951 / 2.0;
        velocity_.x *= sqrt2;
        velocity_.y *= sqrt2;
    }

    component::Sprite* sprite;
    MATCH_INTO_VAR_OR_RETURN_VOID(GetEntity()->GetComponent<component::Sprite>(), sprite);

    component::Human* human;
    MATCH_INTO_VAR_OR_RETURN_VOID(GetEntity()->GetComponent<component::Human>(), human);

    component::RigidBody* rigid;
    MATCH_INTO_VAR_OR_RETURN_VOID(GetEntity()->GetComponent<component::RigidBody>(), rigid);

    engine::Node2DComponent* node2d;
    MATCH_INTO_VAR_OR_RETURN_VOID(GetEntity()->GetComponent<engine::Node2DComponent>(), node2d);

    MonsterAction::Move(sprite, node2d, human, rigid, velocity_);

    if (pickup_) pickup_->Update();
    if (openBackpackPanel_) openBackpackPanel_->Update();
    if (openCompositePanel_) openCompositePanel_->Update();
    if (putLeft_) putLeft_->Update();
    if (putRight_) putRight_->Update();
}