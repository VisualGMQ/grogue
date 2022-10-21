#include "transform_update.hpp"

void TransformUpdateSystem::Update(engine::Entity* entity) {
    if (!entity) return;

    if (auto node = entity->GetComponent<engine::Node2DComponent>(); node) {
        node->globalPosition = node->position;
        node->globalScale = node->scale;
        node->rotation = node->rotation;
        auto parentNode = node->GetParentNode()->GetComponent<engine::Node2DComponent>();
        if (parentNode) {
            node->globalPosition += parentNode->globalPosition;
            node->globalScale *= parentNode->globalScale;
            node->rotation += parentNode->rotation;
        }
    }
}