#include "collision_outline.hpp"

void CollisionOutlineSystem::Update(engine::Entity* entity) {
    if (!entity) return;

    auto box = entity->GetComponent<component::BoxCollider>();
    auto node = entity->GetComponent<engine::Node2DComponent>();
    if (box.IsErr() || node.IsErr()) return;

    engine::Renderer::SetDrawColor(engine::Color{0, 255, 0});
    engine::Renderer::DrawRect(engine::Rect(box.Unwrap()->rect.position + node.Unwrap()->globalPosition, box.Unwrap()->rect.size));
}