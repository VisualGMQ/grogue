#include "human_create.hpp"

engine::Entity* CreateHuman(const std::string& down, const std::string& up, const std::string& right) {
    auto entity = engine::World::Instance()->CreateEntity<component::Human, component::Sprite, engine::Node2DComponent>("human");
    auto human = entity->GetComponent<component::Human>();
    engine::ImageFactory::Find(down, human->down);
    engine::ImageFactory::Find(up, human->up);
    engine::ImageFactory::Find(right, human->right);

    auto sprite = entity->GetComponent<component::Sprite>();
    sprite->image = human->down;
    sprite->size = sprite->image.region.size * 2;

    auto node = entity->GetComponent<engine::Node2DComponent>();

    return entity;
}