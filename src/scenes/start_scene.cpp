#include "scenes/start_scene.hpp"

void StartScene::OnInit() {
    Localization::Init("data/localization/chinese.toml");
    LoadAllImageResources("./resources/img");
    ObjectConfigStorage::LoadAllConfig("./data/object");
    ComposeConfigStorage::LoadAllConfig("./data/compose.toml");
    engine::FontFactory::Create("C:/windows/fonts/simhei.ttf", "simhei", 20);

    attachSystems();
    initMap();

    auto player = CreateHuman("role#down", "role#up", "role#right", 50);
    Attach2D(player);

    GameData::Instance()->InitControllers(player);
    GameData::Instance()->SetPlayer(player);

    auto backpackEntity = engine::World::Instance()->CreateEntity<engine::NodeComponent, component::BackpackPanel>("backpack");
    backpackEntity->SetActive(false);
    Attach2D(backpackEntity);

    GameData::Instance()->SetBackpackPanel(backpackEntity);
    MonsterManager::Add(player);
}

void StartScene::initMap() {
    GroundGenerator generator;
    auto map = generator.Generate(50, 50);

    MapManager::SetGroundMap(std::move(map));

    auto entity = engine::World::Instance()->CreateEntity<component::MapComponent, engine::Node2DComponent>("ground-map");
    auto mapComponent = entity->GetComponent<component::MapComponent>();
    mapComponent->map = MapManager::GetGroundMap();

    Attach2D(entity);
}

void StartScene::attachSystems() {
    engine::World::Instance()->AddSystem<ControllerUpdateSystem>();
    engine::World::Instance()->AddSystem<TransformUpdateSystem>();
    engine::World::Instance()->AddSystem<CollisionSystem>();
    engine::World::Instance()->AddSystem<PhysicalSystem>();
    engine::World::Instance()->AddSystem<PhysicalClearSystem>();
    engine::World::Instance()->AddSystem<SortMonstersSystem>();
    engine::World::Instance()->AddSystem<MapRenderSystem>();
    // engine::World::Instance()->AddSystem<SpriteRenderSystem>();
    engine::World::Instance()->AddSystem<HintArrowSystem>();
    engine::World::Instance()->AddSystem<BackpackPanelRenderSystem>();
    engine::World::Instance()->AddSystem<PlayerInfoRenderSystem>();
    // engine::World::Instance()->AddSystem<CollisionOutlineSystem>();
}

void StartScene::OnQuit() {
    Localization::Quit();
    MapManager::ClearGroundMap();
    MapManager::ClearDungeonMap();
}