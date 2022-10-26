#include "map_generate.hpp"

std::unique_ptr<Map> GroundGenerator::Generate(int w, int h) {
    size_.Set(w, h);
    std::vector<TerrianType> terrians(w * h, TerrianType::Grass);
    std::vector<ObjectType> objects(w * h, ObjectType::None);

    return std::move(logicalTypes2Map(terrians, objects));
}

std::unique_ptr<Map> GroundGenerator::logicalTypes2Map(const std::vector<TerrianType>& terrians, const std::vector<ObjectType>& objects) {
    std::unique_ptr<Map> map = std::make_unique<Map>(size_.w, size_.h);
    for (int x = 0; x < size_.w; x++) {
        for (int y = 0; y < size_.h; y++) {
            auto terrian = getElem(x, y, terrians);
            auto object = getElem(x, y, objects);
            map->Get(x, y).terrian = createTerrian(terrian);
            map->Get(x, y).object = createObject(x, y, map.get(), object);
        }
    }
    return std::move(map);
}

Terrian GroundGenerator::createTerrian(TerrianType type) {
    Terrian terrian;

    if (type == TerrianType::Void) {
        terrian.type = Terrian::Type::Void;
        return terrian;
    }

    if (type == TerrianType::Grass) {
        std::string grassName = "tilesheet#grass";
        float rand = engine::Random::Instance()->Random();
        if (rand > 0.5) {
            grassName += "1";
        } else {
            grassName += "2";
        }
        if (!engine::ImageFactory::Find(grassName, terrian.image)) {
            Loge("image `{}` is not exists", grassName);
        }
        terrian.type = Terrian::Type::Solid;
    } else if (type == TerrianType::Water) {
        if (!engine::ImageFactory::Find("tilesheet#water1", terrian.image)) {
            Loge("image `tilesheet#water1` is not exists");
        }
        terrian.type = Terrian::Type::Liquid;
    }
    return terrian;
}

engine::Entity* GroundGenerator::createObject(int x, int y, Map* map, ObjectType type) {
    auto& tile = map->Get(x, y);
    if (tile.terrian.type == Terrian::Type::Solid && !tile.object) {
        auto rng = engine::Random::Instance()->Random();
        if (rng >= 0 && rng <= 0.1) {
            auto config = ObjectConfigStorage::Find("tree");
            if (config) {
                return CreateArchitecture(*config);
            }
        } else if (rng <= 0.2) {
            auto config = ObjectConfigStorage::Find("bigstone");
            if (config) {
                return CreateArchitecture(*config);
            }
        } else if (rng <= 0.3) {
            auto config = ObjectConfigStorage::Find("wood");
            if (config) {
                return CreatePickupable(*config);
            }
        } else if (rng <= 0.4) {
            auto config = ObjectConfigStorage::Find("round_stone");
            if (config) {
                return CreatePickupable(*config);
            }
        } else if (rng <= 0.5) {
            auto config = ObjectConfigStorage::Find("sharp_stone");
            if (config) {
                return CreatePickupable(*config);
            }
        }
    }
    return nullptr;
}