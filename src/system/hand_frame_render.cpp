#include "hand_frame_render.hpp"

void HandFrameRenderSystem::Update(engine::Entity* entity) {
    component::HandFrame* handFrame;
    MATCH_INTO_VAR_OR_RETURN_VOID(entity->GetComponent<component::HandFrame>(), handFrame);

    auto rect = engine::Rect(handFrame->position,
                             handFrame->frameImage_.region.size);

    engine::Renderer::DrawTexture(*handFrame->frameImage_.texture,
                                  &handFrame->frameImage_.region,
                                  engine::Size(TileSize, TileSize),
                                  engine::Transform{rect.position});

    if (!handFrame->entity) return ;

    component::Sprite* sprite;
    MATCH_INTO_VAR_OR_RETURN_VOID(handFrame->entity->GetComponent<component::Sprite>(), sprite);

    engine::Renderer::DrawTexture(*sprite->image.texture,
                                   &sprite->image.region,
                                   engine::Size(TileSize, TileSize),
                                   engine::Transform{AlignCenter(sprite->image.region.size, rect)});

    component::Pickupable* pickupable;
    MATCH_INTO_VAR_OR_RETURN_VOID(handFrame->entity->GetComponent<component::Pickupable>(), pickupable);

    if (pickupable && pickupable->num > 0) {
        auto font = engine::FontFactory::Find("simhei");
        if (!font) return ;

        auto numStr = std::to_string(pickupable->num);
        auto strSize = font->SizeUTF8(numStr);
        // FIXME: text at top-left
        engine::Renderer::DrawText(font, numStr, AlignBottomRight(strSize, rect), engine::Color::White);
    }

}