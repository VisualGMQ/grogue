#include "orientation.hpp"

void OrientationUpdateSystem::Update(engine::Entity* entity) {
    if (!entity) { return; }

    auto rigid = entity->GetComponent<component::RigidBody>();
    auto human = entity->GetComponent<component::Human>();

    if (!human || !rigid) { return; }

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

    auto sprite = entity->GetComponent<component::Sprite>();
    auto node2d = entity->GetComponent<engine::Node2DComponent>();

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