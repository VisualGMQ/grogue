#include "physical_clear.hpp"

void PhysicalClearSystem::Update(engine::Entity* entity) {
    if (!entity) { return; }

    auto rigid = entity->GetComponent<component::RigidBody>();

    if (!rigid) { return; }

    rigid->velocity.Set(0, 0);
}