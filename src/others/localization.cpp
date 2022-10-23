#pragma once

#include "localization.hpp"
#include <filesystem>

std::unordered_map<std::string, std::string> Localization::datas_;

void Localization::Init(const std::string& filename) {
    if (!std::filesystem::exists(filename)) {
        Loge("localization file {} not exists", filename);
        return;
    }
    auto tbl = toml::parse_file(filename);
    for (auto& elem : tbl) {
        Logt("elem = {}", elem.first.str());
        datas_[std::string(elem.first.str())] = elem.second.value<std::string>().value();
    }
}

std::string Localization::Get(const std::string& key) {
    auto it = datas_.find(key);
    if (it != datas_.end()) {
        return it->second;
    } else {
        Loge("localization {} field not found", key);
        return "";
    }
}

void Localization::Quit() {}