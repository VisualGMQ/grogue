#include "sword_behavior.hpp"

void SwordBehavior::OnInit() {
    transform = Parent()->GetComponent<engine::Node2DComponent>();
    auto tile = engine::TileSheetFactory::Find("tilesheet")->Get(0, 8);

    sprite = Parent()->GetComponent<engine::SpriteComponent>();
    sprite->image = tile;
    sprite->offset.Set(0, -8);

    transform->position = engine::Vec2(0, 0);
}

void SwordBehavior::OnUpdate() {
}

void SwordBehavior::OnQuit() {

}
