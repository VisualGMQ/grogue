#include "transform_update.hpp"

void TransformUpdateSystem::Update(engine::Entity* entity) {
    if (!entity) return;

    engine::Node2DComponent* node;
    MATCH_INTO_VAR_OR_RETURN_VOID(entity->GetComponent<engine::Node2DComponent>(), node);

    node->globalPosition = node->position;
    node->globalScale = node->scale;
    node->rotation = node->rotation;

    engine::Node2DComponent* parentNode;
    MATCH_INTO_VAR_OR_RETURN_VOID(node->GetParentNode()->GetComponent<engine::Node2DComponent>(), parentNode);

    node->globalPosition += parentNode->globalPosition;
    node->globalScale *= parentNode->globalScale;
    node->rotation += parentNode->rotation;
}