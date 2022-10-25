#pragma once

#include "object_config.hpp"
#include "components/sprite.hpp"
#include "components/architecture.hpp"
#include "components/pickupable.hpp"
#include "components/box_collider.hpp"
#include "components/compose.hpp"
#include "config.hpp"

class ObjectConfigReader final {
public:
    std::optional<ObjectConfig> Read(const std::filesystem::path& filepath);
    
private:
    static ObjectConfig::Type getObjectTypeFromStr(const std::string&);
};

class ObjectConfigStorage final {
public:
    static void LoadAllConfig(const std::filesystem::path& root);
    static bool Find(ObjectID id, ObjectConfig&);
    static bool Find(const std::string& name, ObjectConfig&);
    static std::optional<ObjectID> ObjectName2ID(const std::string& name);

private:
    static std::unordered_map<std::string, ObjectID> idNameMap;
    static std::unordered_map<ObjectID, ObjectConfig> configs;

    static void loadConfigRecursive(const std::filesystem::path& root, const std::filesystem::path& path);
};

engine::Entity* CreateObject(ObjectID id);
engine::Entity* CreateArchitecture(const ObjectConfig& config);
engine::Entity* CreatePickupable(const ObjectConfig& config);
engine::Entity* CreateObject(const std::string& name);