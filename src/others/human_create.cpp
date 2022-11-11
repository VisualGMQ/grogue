#include "human_create.hpp"

engine::Entity* CreateHuman(const std::string& down, const std::string& up, const std::string& right, float pickupRange) {
    auto entity = engine::World::Instance().CreateEntity<component::Human,
                                                         component::Sprite,
                                                         component::Backpack,
                                                         engine::Node2DComponent,
                                                         component::BoxCollider,
                                                         component::RigidBody,
                                                         component::Life>("human");
    auto human = entity->GetComponent<component::Human>().Unwrap();
    engine::ImageFactory::Find(down, human->down);
    engine::ImageFactory::Find(up, human->up);
    engine::ImageFactory::Find(right, human->right);
    human->pickupRange = pickupRange;

    auto sprite = entity->GetComponent<component::Sprite>().Unwrap();
    sprite->image = human->down;
    sprite->size = sprite->image.region.size * 2;

    auto box = entity->GetComponent<component::BoxCollider>().Unwrap();
    box->rect.position.Set(8, 32);
    box->rect.size.Set(16, 12);

    return entity;
}