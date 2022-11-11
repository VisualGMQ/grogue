#include "physical.hpp"

void PhysicalSystem::Update(engine::Entity* entity) {
    if (!entity) { return; }

    engine::Node2DComponent* node;
    component::RigidBody* rigid;
    MATCH_INTO_VAR_OR_RETURN_VOID(entity->GetComponent<engine::Node2DComponent>(), node);
    MATCH_INTO_VAR_OR_RETURN_VOID(entity->GetComponent<component::RigidBody>(), rigid);

    node->position += engine::Timer::GetElapse() * rigid->velocity;
}