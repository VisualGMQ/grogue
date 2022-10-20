#include "map_generate.hpp"

std::shared_ptr<Map> GroundGenerator::Generate(int w, int h) {
    size_.Set(w, h);
    std::vector<TerrianType> terrians(w * h, TerrianType::Grass);
    std::vector<ObjectType> objects(w * h, ObjectType::None);

    return logicalTypes2Map(terrians, objects);
}

std::shared_ptr<Map> GroundGenerator::logicalTypes2Map(const std::vector<TerrianType>& terrians, const std::vector<ObjectType>& objects) {
    std::shared_ptr<Map> map = std::make_shared<Map>(size_.w, size_.h);
    for (int x = 0; x < size_.w; x++) {
        for (int y = 0; y < size_.h; y++) {
            auto terrian = getElem(x, y, terrians);
            auto object = getElem(x, y, objects);
            map->Get(x, y).terrian = createTerrian(x, y, terrian);
            map->Get(x, y).object = createObject(x, y, object);
        }
    }
    return map;
}

engine::Entity* GroundGenerator::createTerrian(int x, int y, TerrianType type) {
    auto world = engine::World::Instance();
    engine::Entity* entity = nullptr;
    if (type == TerrianType::Void) {
        return nullptr;
    }

    entity = world->CreateEntity("tile");
    auto terrian = world->CreateComponent<component::Terrian>();
    auto sprite = world->CreateComponent<engine::SpriteComponent>();
    auto node2d = world->CreateComponent<engine::Node2DComponent>();
    entity->SetComponent(terrian);
    entity->SetComponent(node2d);
    entity->SetComponent(sprite);

    node2d->position.Set(x * TileSize, y * TileSize);
    sprite->size.Set(TileSize, TileSize);

    if (type == TerrianType::Grass) {
        std::string grassName = "tilesheet#grass";
        float rand = engine::Random::Instance()->Random();
        if (rand > 0.5) {
            grassName += "1";
        } else {
            grassName += "2";
        }
        if (!engine::ImageFactory::Find(grassName, sprite->image)) {
            Loge("image `{}` is not exists", grassName);
        }
        terrian->type = component::Terrian::Type::Solid;
    } else if (type == TerrianType::Water) {
        if (!engine::ImageFactory::Find("tilesheet#water1", sprite->image)) {
            Loge("image `tilesheet#water1` is not exists");
        }
        terrian->type = component::Terrian::Type::Liquid;
    }
    return entity;
}

engine::Entity* GroundGenerator::createObject(int x, int y, ObjectType type) {
    // TODO not finish
    return nullptr;
}