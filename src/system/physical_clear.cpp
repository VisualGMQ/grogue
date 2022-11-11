#include "physical_clear.hpp"

void PhysicalClearSystem::Update(engine::Entity* entity) {
    if (!entity) { return; }

    component::RigidBody* rigid;
    MATCH_INTO_VAR_OR_RETURN_VOID(entity->GetComponent<component::RigidBody>(), rigid);

    rigid->velocity.Set(0, 0);
}