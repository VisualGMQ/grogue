#pragma once

#include "engine/engine.hpp"

class Localization final {
public:
    static void Init(const std::string& filename);
    static void Quit();

    static std::string Get(const std::string& key);

private:
    static std::unordered_map<std::string, std::string> datas_;
};