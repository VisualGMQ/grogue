#include "scenes/start_scene.hpp"

void StartScene::OnInit() {
    loadResources();
    engine::debug::PropertyWatcher::Attach2Scene();
    initNodes();
    engine::debug::DebugAttachSceneTree();
}

void StartScene::OnQuit() {

}

void StartScene::loadResources() {
    auto tilesheet = engine::TextureFactory::Create("resources/img/tilesheet.png", "tilesheet");
    auto role = engine::TextureFactory::Create("resources/img/role.png", "role");
    engine::TileSheetFactory::Create("tilesheet", tilesheet, 12, 12);
    engine::TileSheetFactory::Create("role", role, 9, 2);
}

void StartScene::initNodes() {
    auto world = engine::World::Instance();
    auto role = world->CreateEntity("role");
    role->SetBehavior(std::make_unique<RoleBehavior>());
    auto node = world->CreateComponent<engine::Node2DComponent>();
    role->SetComponent(node);
    Attach2D(role);

    engine::debug::PropertyWatcher::SetWatchEntity(role);

    auto roleHead = world->CreateEntity<engine::SpriteComponent, engine::Node2DComponent>("role head");
    node->Attach(roleHead);

    auto roleBody = world->CreateEntity<engine::SpriteComponent, engine::Node2DComponent>("role body");
    node->Attach(roleBody);

    auto roleHair = world->CreateEntity<engine::SpriteComponent, engine::Node2DComponent>("role hair");
    node->Attach(roleHair);

    auto roleFeet = world->CreateEntity<engine::SpriteComponent, engine::Node2DComponent>("role feet");
    node->Attach(roleFeet);

    auto sword = world->CreateEntity<engine::Node2DComponent, engine::SpriteComponent>("sword");
    sword->SetBehavior(std::make_unique<SwordBehavior>());
    node->Attach(sword);
}
