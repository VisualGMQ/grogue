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
    ComposeConfigStorage() = delete;
    ComposeConfigStorage(const ComposeConfigStorage&) = delete;

    using container = std::map<ObjectID, Composite>;
    using iterator = container::iterator;
    using const_iterator = container::const_iterator;

    static void LoadAllConfig(const std::filesystem::path& path);
    static Composite* Find(ObjectID);
    static Composite* Find(const std::string& objName);
    static iterator begin() { return formulas_.begin(); }
    static iterator end() { return formulas_.end(); }
    static const_iterator cbegin() { return formulas_.cbegin(); }
    static const_iterator cend() { return formulas_.cend(); }
    static size_t Size() { return formulas_.size(); }

    // for debug
    static void OutputAllConfig();

private:
    static container formulas_;
};
