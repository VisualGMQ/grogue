#include "scenes/start_scene.hpp"

void StartScene::OnInit() {
    ObjectConfigStorage::LoadAllConfig("./data/object");
    LoadAllImageResources("./resources/img");

    GroundGenerator generator;
    auto map = generator.Generate(100, 100);

    auto mapNode = engine::World::Instance()->CreateEntity("map");
    auto node2d = engine::World::Instance()->CreateComponent<engine::Node2DComponent>();
    mapNode->SetComponent(node2d);
    for (int x = 0; x < map->GetSize().w; x++) {
        for (int y = 0; y < map->GetSize().h; y++) {
            node2d->Attach(map->Get(x, y).terrian);
        }
    }
    Attach2D(mapNode);
}

void StartScene::OnQuit() {

}