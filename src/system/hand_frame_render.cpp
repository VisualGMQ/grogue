#include "hand_frame_render.hpp"

void HandFrameRenderSystem::Update(engine::Entity* entity) {
    auto handFrame = entity->GetComponent<component::HandFrame>();
    if (!handFrame) return;

    auto rect = engine::Rect(handFrame->position,
                             handFrame->frameImage_.region.size);

    engine::Renderer::DrawTexture(*handFrame->frameImage_.texture,
                                  &handFrame->frameImage_.region,
                                  engine::Size(TileSize, TileSize),
                                  engine::Transform{rect.position});

    if (handFrame && handFrame->image) {
        engine::Renderer::DrawTexture(*handFrame->image.texture,
                                      &handFrame->image.region,
                                      engine::Size(TileSize, TileSize),
                                      engine::Transform{AlignCenter(handFrame->image.region.size, rect)});
    }
}