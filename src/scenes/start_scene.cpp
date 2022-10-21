#include "scenes/start_scene.hpp"

void StartScene::OnInit() {
    LoadAllImageResources("./resources/img");
    ObjectConfigStorage::LoadAllConfig("./data/object");

    engine::World::Instance()->AddSystem<TransformUpdateSystem>();
    engine::World::Instance()->AddSystem<MapTileRenderSystem>();

    initMap();
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
    MapManager::ClearGroundMap();
    MapManager::ClearDungeonMap();
}