#pragma once

#include "map/object_config.hpp"

class ObjectConfigReader final {
public:
    ObjectConfig Read(const std::filesystem::path& filepath);
};

class ObjectConfigStorage final {
public:
    static void LoadAllConfig(const std::filesystem::path& root);
    static bool Find(ObjectID id, ObjectConfig&);
    static bool Find(const std::string& name, ObjectConfig&);

private:
    static std::unordered_map<std::string, ObjectID> idNameMap;
    static std::unordered_map<ObjectID, ObjectConfig> configs;

    static void loadConfigRecursive(const std::filesystem::path& root, const std::filesystem::path& path);
};

engine::Entity* CreateObject(ObjectID id);
engine::Entity* CreateObject(const std::string& name);