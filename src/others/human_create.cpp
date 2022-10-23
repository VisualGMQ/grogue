#include "human_create.hpp"

engine::Entity* CreateHuman(const std::string& down, const std::string& up, const std::string& right, float pickupRange) {
    auto entity = engine::World::Instance()->CreateEntity<component::Human, component::Sprite, component::Backpack,engine::Node2DComponent>("human");
    auto human = entity->GetComponent<component::Human>();
    engine::ImageFactory::Find(down, human->down);
    engine::ImageFactory::Find(up, human->up);
    engine::ImageFactory::Find(right, human->right);
    human->pickupRange = pickupRange;

    auto sprite = entity->GetComponent<component::Sprite>();
    sprite->image = human->down;
    sprite->size = sprite->image.region.size * 2;

    auto node = entity->GetComponent<engine::Node2DComponent>();

    return entity;
}