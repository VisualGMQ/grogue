#include "object_reader.hpp"

std::optional<ObjectConfig> ObjectConfigReader::Read(const std::filesystem::path& filepath) {
    ObjectConfig config;
    auto tbl = toml::parse_file(filepath.c_str());
    config.id = tbl["id"].value<int>().value();
    config.name = tbl["name"].value<std::string>().value_or("[No Name]");
    if (tbl["img"].is_array()) {
        auto imgArr = tbl["img"].as_array();
        // config.imageName = imgArr[0].value<std::string>().value();
        config.orientatedArchImages.resize(imgArr->size());
        for (int i = 0; i < imgArr->size(); i++) {
            std::string imgName = imgArr->get(i)->value<std::string>().value();
            if (!engine::ImageFactory::Find(imgName, config.orientatedArchImages[i])) {
                Loge("object image {} not found", imgName);
            }   
        }
        config.image = config.orientatedArchImages[0];
    } else if (tbl["img"].is_string()) {
        config.imageName = tbl["img"].value<std::string>().value();
        if (!engine::ImageFactory::Find(config.imageName, config.image)) {
            Loge("object image {} not found", config.imageName);
        }
    } else {
        Loge("object field `img` invalid");
        return std::nullopt;
    }
    config.description = tbl["description"].value<std::string>().value_or("");
    config.type = getObjectTypeFromStr(tbl["type"].value<std::string>().value());
    return config;
}

ObjectConfig::Type ObjectConfigReader::getObjectTypeFromStr(const std::string& str) {
    if (str == "architecture") {
        return ObjectConfig::Type::Architecture;
    }
    if (str == "pickup") {
        return ObjectConfig::Type::Pickable;
    }
    return ObjectConfig::Type::Unknown;
}

std::unordered_map<std::string, ObjectID> ObjectConfigStorage::idNameMap;
std::unordered_map<ObjectID, ObjectConfig> ObjectConfigStorage::configs;

void ObjectConfigStorage::LoadAllConfig(const std::filesystem::path& root) {
    if (!std::filesystem::exists(root)) {
        Loge("load config failed: root dir not exists - {}", root);
        return;
    }
    
    PathWalker pathWalker({".toml"}, [&](const std::filesystem::path& path) {
        ObjectConfigReader reader;
        auto config = reader.Read(path);

        if (config) {
            configs[config->id] = config.value();
            auto name = RemoveRootFromPath(root.string(), GetFilenameNoExt(path.string()));
            CvtWindowsDelim2Unix(name);
            idNameMap[name] = config->id;
        }
    });

    if (pathWalker(root) == PathWalker::Error::PathNotExists) {
        Loge("object config path {} not exists", root);
    }
}

ObjectConfig* ObjectConfigStorage::Find(ObjectID id) {
    auto it = configs.find(id);
    if (it == configs.end()) {
        return nullptr;
    }
    return &it->second;
}

ObjectConfig* ObjectConfigStorage::Find(const std::string& name) {
    auto it = idNameMap.find(name);
    if (it == idNameMap.end()) return nullptr;
    return Find(it->second);
}


engine::Entity* CreateObject(ObjectID id) {
    auto config = ObjectConfigStorage::Find(id);
    if (!config) {
        Loge("object {} config not found", id);
        return nullptr;
    }
    switch (config->type) {
        case ObjectConfig::Type::Architecture:
            return CreateArchitecture(*config);
        case ObjectConfig::Type::Pickable:
            return CreatePickupable(*config);
        default:
            return nullptr;
    }

}

std::optional<ObjectID> ObjectConfigStorage::ObjectName2ID(const std::string& name) {
    auto it = idNameMap.find(name);
    if (it != idNameMap.end()) {
        return it->second;
    }
    return std::nullopt;
}

engine::Entity* CreateArchitecture(const ObjectConfig& config) {
    auto entity = engine::World::Instance().CreateEntity<component::Sprite, component::Architecture, component::BoxCollider>("architecture-" + config.name);
    auto sprite = entity->GetComponent<component::Sprite>().Unwrap();
    auto architecture = entity->GetComponent<component::Architecture>().Unwrap();
    auto box = entity->GetComponent<component::BoxCollider>().Unwrap();
    architecture->id = config.id;

    sprite->image = config.image;
    sprite->size.Set(TileSize, TileSize);

    box->rect.Set(0, 0, sprite->size.w, sprite->size.h);

    return entity;
}

engine::Entity* CreatePickupable(const ObjectConfig& config) {
    auto entity = engine::World::Instance().CreateEntity<component::Sprite, component::Pickupable>("architecture-" + config.name);
    auto sprite = entity->GetComponent<component::Sprite>().Unwrap();
    auto pickupable = entity->GetComponent<component::Pickupable>().Unwrap();
    pickupable->num = 1;
    pickupable->id = config.id;

    sprite->image = config.image;
    sprite->size.Set(TileSize, TileSize);

    return entity;   
}

engine::Entity* CreateObject(const std::string& name) {
    auto id = ObjectConfigStorage::ObjectName2ID(name);
    if (!id) return nullptr;
    return CreateObject(id.value());
}
