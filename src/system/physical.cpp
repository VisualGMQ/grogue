#include "physical.hpp"

void PhysicalSystem::Update(engine::Entity* entity) {
    if (!entity) { return; }

    auto node = entity->GetComponent<engine::Node2DComponent>();
    auto rigid = entity->GetComponent<component::RigidBody>();

    if (!node || !rigid) { return; }

    node->position += engine::Timer::GetElapse() * rigid->velocity;
}