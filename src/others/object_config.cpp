#include "object_config.hpp"

std::string ObjectInfo::GetReadableInfo() const {
    std::string result;
    if (hammerLevel > 0) {
        result += std::string(Localization::Get("objfeature_hammer")) + std::to_string(hammerLevel) + "\n";
    }
    if (cutLevel > 0) {
        result += std::string(Localization::Get("objfeature_cut")) + std::to_string(cutLevel) + "\n";
    }
    if (attack > 0) {
        result += std::string(Localization::Get("obj_attack")) + std::to_string(attack) + "\n";
    }
    if (defence > 0) {
        result += std::string(Localization::Get("obj_defence")) + std::to_string(defence);
    }
    return result;
}

std::string ObjectConfig::GetReadableInfo() const {
    std::string result; 
    result += Localization::Get("objconfig_name") + name + "\n"; 
    result += Localization::Get("objconfig_type") + ObjectType2ReadableStr(type) + "\n";
    result += Localization::Get("objconfig_description") + description;
    return result;
}

std::string ObjectConfig::GetDebugInfo() const {
    std::string result; 
    result += "name:" + name + "\n"; 
    result += "type" + ObjectType2ReadableStr(type) + "\n";
    result += "description:" + description + "\n";
    result += "ID:" + std::to_string(id) + "\n";
    result += "image name:" + imageName;
    return result;
}

std::string ObjectType2ReadableStr(ObjectConfig::Type type) {
    switch (type) {
        case ObjectConfig::Type::Architecture:
            return Localization::Get("objtype_architecture");
        case ObjectConfig::Type::Pickable:
            return Localization::Get("objtype_pickupable");
        case ObjectConfig::Type::Unknown:
            return Localization::Get("objtype_unknown");
        default:
            return "";
    }
}