#pragma once

#include "engine/engine.hpp"
#include "map/object_feature.hpp"
#include "others/path_helper.hpp"

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
    engine::Image image;
    Type type;

    static Type GetTypeFromStr(std::string_view str);

    void TurnOnFeature(std::string_view feature);

private:
    bool isFeatureHasProperty(std::string_view feature);
    std::pair<std::string_view, int> parseFeature(std::string_view feature);
};
