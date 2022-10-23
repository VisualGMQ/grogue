#include "sprite_render.hpp"


void SpriteRenderSystem::Update(engine::Entity* entity) {
    if (!entity) return;

    if (auto node = entity->GetComponent<engine::Node2DComponent>(); node) {
        if (auto sprite = entity->GetComponent<component::Sprite>(); sprite && sprite->image) {
            engine::Transform transform;
            transform.position = sprite->offset + node->globalPosition;
            transform.rotation = sprite->rotation + node->globalRotation;
            transform.scale = node->globalScale;
            engine::Renderer::DrawTexture(*sprite->image.texture,
                                          &sprite->image.region,
                                          sprite->size,
                                          transform);
        }
    }
}