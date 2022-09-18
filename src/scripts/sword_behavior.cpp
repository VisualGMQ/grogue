#include "sword_behavior.hpp"

void SwordBehavior::OnInit() {
    transform = Parent()->GetComponent<engine::Node2DComponent>();
    auto tile = engine::TileSheetFactory::Find("tilesheet")->Get(0, 8);

    sprite = Parent()->GetComponent<engine::SpriteComponent>();
    sprite->texture = tile.texture;
    sprite->region = tile.region;
    sprite->offset.Set(0, -16);
    sprite->size.Set(2 * 16, 2 * 16);

    transform->SetPosition(engine::Vec2(0, 8));
}

void SwordBehavior::OnUpdate() {
}

void SwordBehavior::OnQuit() {

}