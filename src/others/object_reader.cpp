#include "object_reader.hpp"

ObjectConfig ObjectConfigReader::Read(const std::filesystem::path& filepath) {
    ObjectConfig config;
    auto tbl = toml::parse_file(filepath.c_str());
    config.id = tbl["id"].value<int>().value();
    config.name = tbl["name"].value<std::string>().value_or("[No Name]");
    config.imageName = tbl["img"].value<std::string>().value();
    if (!engine::ImageFactory::Find(config.imageName, config.image)) {
        Loge("object image {} not found", config.imageName);
    }
    config.description = tbl["description"].value<std::string>().value_or("");
    config.type = getObjectTypeFromStr(tbl["type"].value<std::string>().value());
    auto features = tbl["features"].as_array();
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
    
    loadConfigRecursive(root, root);
}

void ObjectConfigStorage::loadConfigRecursive(const std::filesystem::path& root, const std::filesystem::path& path) {
    if (std::filesystem::is_directory(path)) {
        std::filesystem::directory_iterator dirIter(path);
        while (dirIter != std::filesystem::directory_iterator()) {
            loadConfigRecursive(root, *dirIter);
            dirIter++;
        }
    } else {
        ObjectConfigReader reader;
        auto config = reader.Read(path);
        configs[config.id] = config;
        auto name = RemoveRootFromPath(root.string(), GetFilenameNoExt(path.string()));
        CvtWindowsDelim2Unix(name);
        idNameMap[name] = config.id;
    }
}

bool ObjectConfigStorage::Find(ObjectID id, ObjectConfig& config) {
    auto it = configs.find(id);
    if (it == configs.end()) {
        return false;
    }
    config = it->second;
    return true;
}

bool ObjectConfigStorage::Find(const std::string& name, ObjectConfig& config) {
    auto it = idNameMap.find(name);
    if (it == idNameMap.end()) return false;
    return Find(it->second, config);
}


engine::Entity* CreateObject(ObjectID id) {
    ObjectConfig config;
    if (!ObjectConfigStorage::Find(id, config)) {
        Loge("object {} config not found", config.name);
        return nullptr;
    }
    switch (config.type) {
        case ObjectConfig::Type::Architecture:
            return CreateArchitecture(config);
        case ObjectConfig::Type::Pickable:
            return CreatePickupable(config);
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
    auto entity = engine::World::Instance()->CreateEntity<component::Sprite, component::Architecture>("architecture-" + config.name);
    auto sprite = entity->GetComponent<component::Sprite>();
    auto architecture = entity->GetComponent<component::Architecture>();
    architecture->id = config.id;

    sprite->image = config.image;
    sprite->size.Set(TileSize, TileSize);

    return entity;
}

engine::Entity* CreatePickupable(const ObjectConfig& config) {
    auto entity = engine::World::Instance()->CreateEntity<component::Sprite, component::Pickupable>("architecture-" + config.name);
    auto sprite = entity->GetComponent<component::Sprite>();
    auto pickupable = entity->GetComponent<component::Pickupable>();
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