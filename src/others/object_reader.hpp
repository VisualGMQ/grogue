#pragma once

#include "engine/engine.hpp"
#include "object_feature.hpp"
#include "path_helper.hpp"

struct ObjectConfig {
    enum Type {
        Unknown = 0,
        Block,
        Pickable,

        TypeNum,
    };

    ObjectFeature feature; 
    ObjectID id;
    std::string name;
    std::string description;
    std::string image;
    Type type;

    static Type GetTypeFromStr(std::string_view str);

    void TurnOnFeature(std::string_view feature);

private:
    bool isFeatureHasProperty(std::string_view feature);
    std::pair<std::string_view, int> parseFeature(std::string_view feature);
};

class ObjectConfigReader final {
public:
    ObjectConfig Read(const std::filesystem::path& filepath);
};

class ObjectConfigStorage final {
public:
    static void LoadAllConfig(const std::filesystem::path& root);
    static std::optional<std::reference_wrapper<ObjectConfig>> Find(ObjectID id);
    static std::optional<std::reference_wrapper<ObjectConfig>> Find(const std::string& name);

private:
    static std::unordered_map<std::string, ObjectID> idNameMap;
    static std::unordered_map<ObjectID, ObjectConfig> configs;

    static void loadConfigRecursive(const std::filesystem::path& root, const std::filesystem::path& path);
};

engine::Entity* CreateObject(ObjectID id);
engine::Entity* CreateObject(const std::string& name);