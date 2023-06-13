#pragma once

#include "core/pch.hpp"
#include "app/config_parse.hpp"
#include "app/tilesheet.hpp"

// some config data for auto-parse tool

struct LUA_BIND RaceProfDefPOD final {
    std::vector<std::string> races;
    std::vector<std::string> professions;
};

struct LUA_BIND MonsterPropertyRange final {
    std::array<int, 2> hp;
    std::array<int, 2> mp;
    std::array<int, 2> strength;
    std::array<int, 2> intelligence;
    std::array<int, 2> outsight;
    std::array<int, 2> constitution;
    std::array<int, 2> agility;
    std::array<int, 2> nutrition;
};

struct LUA_BIND RacePOD final {
    std::string name;
    MonsterProperty basic;
    MonsterProperty max;
    std::vector<MonsterPropertyRange> professions;
};

struct LUA_BIND TilesheetPOD final {
    std::string name;
    int row, col;
};

struct LUA_BIND SpritePOD final {
    std::array<int, 4> color;
    TilesheetPOD tilesheet;
};

struct LUA_BIND ItemPOD final {
    std::string name;
    int weight;
    std::vector<std::string> operations;
    SpritePOD sprite;
    Material material;
};

// reflect datas

ReflRegist(
    refl::Class<RaceProfDefPOD>("RaceProfPOD")
        .Member(&RaceProfDefPOD::races, "races")
        .Member(&RaceProfDefPOD::professions, "professions")
)

ReflRegist(
    refl::Class<MonsterPropertyRange>("MonsterPropertyRange")
        .Member(&MonsterPropertyRange::hp, "hp")
        .Member(&MonsterPropertyRange::mp, "mp")
        .Member(&MonsterPropertyRange::strength, "strength")
        .Member(&MonsterPropertyRange::intelligence, "intelligence")
        .Member(&MonsterPropertyRange::outsight, "outsight")
        .Member(&MonsterPropertyRange::constitution, "constitution")
        .Member(&MonsterPropertyRange::agility, "agility")
        .Member(&MonsterPropertyRange::nutrition, "nutrition")
)

ReflRegist(
    refl::Class<RacePOD>("RacePOD")
        .Member(&RacePOD::name, "name")
        .Member(&RacePOD::basic, "basic")
        .Member(&RacePOD::max, "max")
        .Member(&RacePOD::professions, "professions")
)

ReflRegist(
    refl::Class<TilesheetPOD>("TilesheetPOD")
        .Member(&TilesheetPOD::name, "name")
        .Member(&TilesheetPOD::row, "row")
        .Member(&TilesheetPOD::col, "col")
)

ReflRegist(
    refl::Class<SpritePOD>("SpritePOD")
        .Member(&SpritePOD::color, "color")
        .Member(&SpritePOD::tilesheet, "tilesheet")
)

ReflRegist(
    refl::Class<ItemPOD>("ItemPOD")
        .Member(&ItemPOD::name, "name")
        .Member(&ItemPOD::weight, "weight")
        .Member(&ItemPOD::operations, "operations")
        .Member(&ItemPOD::material, "material")
        .Member(&ItemPOD::sprite, "sprite")
)

ReflRegist(
    refl::Class<BackpackUIInfo>("BakpackUIInfo")
        .Member(&BackpackUIInfo::width, "width")
        .Member(&BackpackUIInfo::height, "height")
        .Member(&BackpackUIInfo::gridSize, "gridSize")
        .Member(&BackpackUIInfo::padding, "padding")
        .Member(&BackpackUIInfo::margin, "margin")
)

class LUA_BIND RaceProfDef final {
public:
    RaceProfDef(LuaManager&, const std::string& filename);
    const std::vector<std::string>& Races() const { return data_.races; }
    const std::vector<std::string>& Professions() const { return data_.professions; }
    bool Valid() const { return valid_; }

private:
    RaceProfDefPOD data_;
    bool valid_;
};

class LUA_BIND RaceProfConfig final {
public:
    RaceProfConfig(LuaManager&, const RaceProfDef& defs, const std::string& filename);
    bool Valid() const { return valid_; }

private:
    RacePOD data_;
    bool valid_;
};

class LUA_BIND ItemConfig final {
public:
    ItemConfig(LuaManager&, TilesheetManager&, const std::string& filename);

    auto& Items() const { return items_; }
    auto& Names() const { return names_; }
    float TotleWeight() const { return totleWeight_; }

    //! @brief whether config init OK
    bool Valid() const { return valid_; }

private:
    std::unordered_map<std::string, ItemInfo> items_;
    std::vector<std::string> names_;
    float totleWeight_ = 0.0;
    bool valid_;
};

class LUA_BIND BackpackUIConfig final {
public:
    BackpackUIConfig(LuaManager&, const std::string& filename);
    const BackpackUIInfo& Info() const { return info_; }

    //! @brief whether config init OK
    bool Valid() const { return valid_; }

private:
    BackpackUIInfo info_;
    bool valid_;
};

//! @brief contains all game config
class LUA_BIND_RESOURCE GameConfig final {
public:
    explicit GameConfig(LuaManager&, TilesheetManager&, const std::string& configDir);

    auto& GetRaceProfDef() const { return *raceProfDef_; }
    auto& GetRaceProfConfig() const { return raceProfConfig_; }
    auto& GetItemConfig() const { return *itemConfig_; }
    auto& GetBackpackUIConfig() const { return *backpackUIConfig_; }

    //! @brief whether config init OK
    operator bool() const { return valid_; }

private:
    std::unique_ptr<RaceProfDef> raceProfDef_;
    std::vector<std::shared_ptr<RaceProfConfig>> raceProfConfig_;
    std::unique_ptr<ItemConfig> itemConfig_;
    std::unique_ptr<BackpackUIConfig> backpackUIConfig_;
    bool valid_;
};