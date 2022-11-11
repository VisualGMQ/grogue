#include "orientation.hpp"

void OrientationUpdateSystem::Update(engine::Entity* entity) {
    if (!entity) { return; }

    component::RigidBody* rigid;
    component::Human* human;
    MATCH_INTO_VAR_OR_RETURN_VOID(entity->GetComponent<component::RigidBody>(), rigid);
    MATCH_INTO_VAR_OR_RETURN_VOID(entity->GetComponent<component::Human>(), human);

    if (rigid->velocity == engine::Vec2(0, 0)) {
        return;
    }

    if (rigid->velocity.x > 0) {
        human->orientation = component::Human::Orientation::Right;
    } else if (rigid->velocity.x < 0) {
        human->orientation = component::Human::Orientation::Left;
    } else {
        human->orientation = component::Human::Orientation::None;
    }
    if (rigid->velocity.y > 0) {
        human->orientation = static_cast<component::Human::Orientation>(human->orientation | component::Human::Orientation::Down);
    } else if (rigid->velocity.y < 0) {
        human->orientation = static_cast<component::Human::Orientation>(human->orientation | component::Human::Orientation::Up);
    }

    component::Sprite* sprite;
    engine::Node2DComponent* node2d;
    MATCH_INTO_VAR_OR_RETURN_VOID(entity->GetComponent<component::Sprite>(), sprite);
    MATCH_INTO_VAR_OR_RETURN_VOID(entity->GetComponent<engine::Node2DComponent>(), node2d);

    if (human->orientation & component::Human::Orientation::Up) {
        node2d->scale.x = 1;
        sprite->image = human->up;
        sprite->offset.Set(0, 0);
    } else if (human->orientation & component::Human::Orientation::Down) {
        sprite->image = human->down;
        node2d->scale.x = 1;
        sprite->offset.Set(0, 0);
    } else {
        if (human->orientation & component::Human::Orientation::Left) {
            sprite->image = human->right;
            node2d->scale.x = -1;
            sprite->offset.Set(8, 0);
        } else if (human->orientation & component::Human::Orientation::Right) {
            sprite->image = human->right;
            node2d->scale.x = 1;
            sprite->offset.Set(0, 0);
        }
    }
}