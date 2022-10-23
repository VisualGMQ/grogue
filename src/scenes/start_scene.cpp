#include "scenes/start_scene.hpp"

void StartScene::OnInit() {
    Localization::Init("data/localization/chinese.toml");
    LoadAllImageResources("./resources/img");
    ObjectConfigStorage::LoadAllConfig("./data/object");

    engine::World::Instance()->AddSystem<ControllerUpdateSystem>();
    engine::World::Instance()->AddSystem<TransformUpdateSystem>();
    engine::World::Instance()->AddSystem<MapTileRenderSystem>();
    engine::World::Instance()->AddSystem<MapObjectRenderSystem>();
    engine::World::Instance()->AddSystem<SpriteRenderSystem>();
    engine::World::Instance()->AddSystem<HintArrowSystem>();

    initMap();

    auto entity = CreateHuman("role#down", "role#up", "role#right", 50);

    GameData::Instance()->ChangeController(std::make_unique<HumanController>(entity));
    GameData::Instance()->SetPlayer(entity);

    Attach2D(entity);
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

void StartScene::OnQuit() {
    Localization::Quit();
    MapManager::ClearGroundMap();
    MapManager::ClearDungeonMap();
}