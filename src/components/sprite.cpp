#include "sprite.hpp"

namespace component {

void Sprite::Reset() {
    engine::Component::Reset();
    offset.Set(0, 0);
    size.Set(0, 0);
    anchor.Set(0, 0);
    rotation = 0;
    image.texture = nullptr;
}

void DrawSprite(Sprite* sprite, const engine::Transform& transform) {
    if (!sprite || !sprite->image) return;
    engine::Renderer::DrawTexture(*sprite->image.texture,
                                  &sprite->image.region,
                                  sprite->size,
                                  transform);
}

}