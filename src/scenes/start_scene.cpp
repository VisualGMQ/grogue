#include "scenes/start_scene.hpp"

void StartScene::OnInit() {
    loadResources();
    engine::debug::DebugAttachSceneTree();
    engine::debug::PropertyWatcher::Attach2Scene();
    initNodes();
    
    // auto window = engine::CreateUIWindow("sprite controller", "sprite controller",
    //                                       NK_WINDOW_MINIMIZABLE|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE,
    //                                       engine::Rect(200, 200, 200, 400));
    // AttachUI(window);
    // auto layout = engine::CreateUIDynamicRowLayout("layout", 20, 1, window);
    // engine::CreateUIProperty("pos_x", "x", nullptr, engine::UIProperty::Type::Float, 0, 1024, 0, 1, 1, layout);
    // engine::CreateUIProperty("pos_y", "y", nullptr, engine::UIProperty::Type::Float, 0, 1024, 0, 1, 1, layout);
    // engine::CreateUIProperty("offset_x", "offset_x", nullptr, engine::UIProperty::Type::Float, 0, 100, 0, 1, 1, layout);
    // engine::CreateUIProperty("offset_y", "offset_y", nullptr, engine::UIProperty::Type::Float, 0, 100, 0, 1, 1, layout);
    // engine::CreateUIProperty("rotation", "rotation", nullptr, engine::UIProperty::Type::Float, 0, 360, 0, 1, 1, layout);
    // engine::CreateUICheckbox("flip_vertical", "flip_vertical", nullptr, layout);
    // engine::CreateUICheckbox("flip_horizontal", "flip_horizontal", nullptr, layout);
    // engine::CreateUIProperty("sword_rotation", "sword_rotation", nullptr, engine::UIProperty::Type::Float, 0, 360, 0, 1, 1, layout);
}

void StartScene::OnQuit() {

}

void StartScene::loadResources() {
    auto tilesheet = engine::TextureFactory::Create("resources/img/tilesheet.png", "tilesheet");
    auto role = engine::TextureFactory::Create("resources/img/role.png", "role");
    engine::TileSheetFactory::Create("tilesheet", tilesheet, 12, 12);
    engine::TileSheetFactory::Create("role", role, 9, 1);
}

void StartScene::initNodes() {
    auto world = engine::World::Instance();
    auto entity = world->CreateEntity("role");
    entity->SetBehavior(std::make_unique<RoleBehavior>());
    auto node = world->CreateComponent<engine::Node2DComponent>();
    entity->SetComponent(node);
    entity->SetComponent(world->CreateComponent<engine::SpriteComponent>());
    Attach2D(entity);

    engine::debug::PropertyWatcher::SetWatchEntity(entity);

    entity = world->CreateEntity("sword");
    entity->SetBehavior(std::make_unique<SwordBehavior>());
    entity->SetComponent(world->CreateComponent<engine::Node2DComponent>());
    entity->SetComponent(world->CreateComponent<engine::SpriteComponent>());
    node->Attach(entity);
}