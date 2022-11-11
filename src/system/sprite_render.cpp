#include "sprite_render.hpp"


void SpriteRenderSystem::Update(engine::Entity* entity) {
    if (!entity) return;

    engine::Node2DComponent* node;
    MATCH_INTO_VAR_OR_RETURN_VOID(entity->GetComponent<engine::Node2DComponent>(), node);

    component::Sprite* sprite;
    MATCH_INTO_VAR_OR_RETURN_VOID(entity->GetComponent<component::Sprite>(), sprite);

    engine::Transform transform;
    transform.position = sprite->offset + node->globalPosition;
    transform.rotation = sprite->rotation + node->globalRotation;
    transform.scale = node->globalScale;

    component::DrawSprite(sprite, transform);
}