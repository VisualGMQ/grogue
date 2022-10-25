#pragma once

#include "object_config.hpp"
#include "engine/engine.hpp"
#include "object_config.hpp"
#include <filesystem>

struct Composite {
    ObjectID target;
    std::vector<ComposeMaterial> materials;
};

class ComposeConfigStorage final {
public:
    static void LoadAllConfig(const std::filesystem::path& path);
    static Composite* Find(ObjectID);
    static Composite* Find(const std::string& objName);

    // for debug
    static void OutputAllConfig();

private:
    static std::unordered_map<ObjectID, Composite> formulas_;
};