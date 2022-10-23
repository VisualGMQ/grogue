#pragma once

#include "engine/engine.hpp"
#include "others/path_helper.hpp"
#include "others/localization.hpp"

using ObjectID = unsigned int;

struct ArchitectureInfo {
    enum DestroyType {
        Undestroyable,
        Hammer,
        Cut,
    }destroyType = Undestroyable;
    int destroyLevel = 0;
};

struct ObjectInfo {
    int hammerLevel = 0;
    int cutLevel = 0;

    int attack = 0;
    int defence = 0;

    std::string GetReadableInfo() const;
};

inline std::ostream& operator<<(std::ostream& o, const ObjectInfo& info) {
    o << "ObjectInfo: " << info.GetReadableInfo();
    return o;
}

struct ObjectConfig {
    enum Type {
        Unknown = 0,
        Architecture,
        Pickable,

        TypeNum,
    };

    ObjectID id;
    Type type;
    std::string name;
    std::string description;
    std::optional<ArchitectureInfo> architectureConfig;
    std::optional<ObjectInfo> objectInfo;
    engine::Image image;
    std::string imageName;

    std::string GetReadableInfo() const;
    std::string GetDebugInfo() const;
};

std::string ObjectType2ReadableStr(ObjectConfig::Type);