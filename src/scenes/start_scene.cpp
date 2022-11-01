#include "scenes/start_scene.hpp"

void StartScene::OnInit() {
    engine::World::Instance()->AddSystem<engine::VideoSystem>();

    Localization::Init("data/localization/chinese.toml");
    LoadAllImageResources("./resources/img");
    ObjectConfigStorage::LoadAllConfig("./data/object");
    ComposeConfigStorage::LoadAllConfig("./data/compose.toml");
    engine::FontFactory::Create("C:/windows/fonts/simhei.ttf", "simhei", 20);

    attachSystems();
    initMap();

    auto player = CreateHuman("role#down", "role#up", "role#right", 50);
    Attach2D(player);

    attachBackpackPanel();
    attachCompositePanel();
    attachCompositeDescriptionPanel();

    GameData::Instance()->SetPlayer(player);
    MonsterManager::Add(player);

    GameData::Instance()->InitControllers(player);
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
    engine::World::Instance()->AddSystem<GridPanelRenderSystem>();
    engine::World::Instance()->AddSystem<PlayerInfoRenderSystem>();
    // engine::World::Instance()->AddSystem<CollisionOutlineSystem>();
}

void StartScene::OnQuit() {
    Localization::Quit();
    MapManager::ClearGroundMap();
    MapManager::ClearDungeonMap();
}

void StartScene::attachBackpackPanel() {
    auto backpackEntity = engine::World::Instance()->CreateEntity<engine::NodeComponent, component::GridPanel>("backpack");
    backpackEntity->SetActive(false);
    auto gridPanel = backpackEntity->GetComponent<component::GridPanel>();
    BackpackObjectDrawer backpackObjDrawer;
    engine::ImageFactory::Find("tilesheet#select_outline", gridPanel->selectOutline);
    gridPanel->drawImageFunc = backpackObjDrawer;
    Attach2D(backpackEntity);
    GameData::Instance()->SetBackpackPanel(backpackEntity);
}

void StartScene::attachCompositePanel() {
    auto compositeEntity = engine::World::Instance()->CreateEntity<engine::NodeComponent, component::GridPanel>("composite-panel");
    compositeEntity->SetActive(false);
    auto gridPanel = compositeEntity->GetComponent<component::GridPanel>();
    CompositeObjectDrawer drawer;
    engine::ImageFactory::Find("tilesheet#select_outline", gridPanel->selectOutline);
    gridPanel->drawImageFunc = drawer;
    gridPanel->gridNumInCol = 3;
    gridPanel->position.Set(100, 100);
    gridPanel->capacity = ComposeConfigStorage::Size();
    Attach2D(compositeEntity);
    GameData::Instance()->SetCompositePanel(compositeEntity);
}

void StartScene::attachCompositeDescriptionPanel() {
    auto compositeDescriptionPanel = engine::World::Instance()->CreateEntity<engine::NodeComponent, component::GridPanel>("composite-description-panel");
    compositeDescriptionPanel->SetActive(false);
    auto gridPanel = compositeDescriptionPanel->GetComponent<component::GridPanel>();
    CompositeDescriptionDrawer drawer;
    engine::ImageFactory::Find("tilesheet#select_outline", gridPanel->selectOutline);
    gridPanel->drawImageFunc = drawer;
    gridPanel->gridNumInCol = 3;
    gridPanel->position.Set(600, 100);
    gridPanel->capacity = 0;
    gridPanel->showCursor = false;
    Attach2D(compositeDescriptionPanel);
    GameData::Instance()->SetCompositeDescriptionPanel(compositeDescriptionPanel);
}
