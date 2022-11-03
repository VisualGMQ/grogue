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

    if (!handFrame || !handFrame->entity) {
        return ;
    }

    auto sprite = handFrame->entity->GetComponent<component::Sprite>();
    engine::Renderer::DrawTexture(*sprite->image.texture,
                                    &sprite->image.region,
                                    engine::Size(TileSize, TileSize),
                                    engine::Transform{AlignCenter(sprite->image.region.size, rect)});

    auto pickupable = handFrame->entity->GetComponent<component::Pickupable>();
    if (pickupable && pickupable->num > 0) {
        auto font = engine::FontFactory::Find("simhei");
        if (!font) return;

        auto numStr = std::to_string(pickupable->num);
        auto strSize = font->SizeUTF8(numStr);
        // FIXME: text at top-left
        engine::Renderer::DrawText(font, numStr, AlignBottomRight(strSize, rect), engine::Color::White);
    }

}