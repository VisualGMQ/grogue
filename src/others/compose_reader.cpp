#pragma once

#include "compose_reader.hpp"
#include "object_reader.hpp"

std::unordered_map<ObjectID, Composite> ComposeConfigStorage::formulas_;

void  ComposeConfigStorage::LoadAllConfig(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        Loge("compose config file {} not exists", path);
        return; 
    }

    auto tbl = toml::parse_file(path.string());
    for (auto& [key, value] : tbl) {
        auto id = ObjectConfigStorage::ObjectName2ID(key.data());
        if (!id) {
            Loge("id of {} not found", key);
            continue;
        }

        Composite composite;
        composite.target = id.value();

        auto arr = value.as_array();
        for (auto& elem : *arr) {
            auto elemTbl = elem.as_table();
            ComposeMaterial material;
            auto materialID = ObjectConfigStorage::ObjectName2ID(elemTbl->at("name").value<std::string>().value());
            if (!materialID) {
                Loge("object {} not exists", key);
                continue;
            }
            material.id = materialID.value();
            material.num = elemTbl->at("num").value<int>().value();

            composite.materials.push_back(material);
        }

        formulas_[id.value()] = composite;
    }
}

Composite* ComposeConfigStorage::Find(ObjectID id) {
    auto it = formulas_.find(id);
    if (it == formulas_.end()) return nullptr;

    return &it->second;
}

Composite* ComposeConfigStorage::Find(const std::string& objName) {
    auto id = ObjectConfigStorage::ObjectName2ID(objName);
    if (!id) {
        return nullptr;
    }
    return Find(id.value());
}

void ComposeConfigStorage::OutputAllConfig() {
    for (auto& [key, value] : formulas_) {
        Logd("formular of {}", key);
        for (auto& material : value.materials) {
            Logd("\tmaterial = {}, num = {}", material.id, material.num);
        }
    }
}