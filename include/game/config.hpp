#pragma once

#include "core/pch.hpp"
#include "app/config_parse.hpp"

// some config data

struct RaceProfDefPOD final {
    std::vector<std::string> races;
    std::vector<std::string> professions;
};

struct MonsterPropertyRange final {
    std::array<int, 2> hp;
    std::array<int, 2> mp;
    std::array<int, 2> strength;
    std::array<int, 2> intelligence;
    std::array<int, 2> outsight;
    std::array<int, 2> constitution;
    std::array<int, 2> agility;
    std::array<int, 2> nutrition;
};

struct RacePOD final {
    std::string name;
    MonsterProperty basic;
    MonsterProperty max;
    std::vector<MonsterPropertyRange> professions;
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

class RaceProfDef final {
public:
    RaceProfDef(LuaManager&, const std::string& filename);
    const std::vector<std::string>& GetRaces() const { return data_.races; }
    const std::vector<std::string>& GetProfessions() const { return data_.professions; }
    bool Valid() const { return valid_; }

private:
    RaceProfDefPOD data_;
    bool valid_;
};

class RaceProfConfig final {
public:
    RaceProfConfig(LuaManager&, const RaceProfDef& defs, const std::string& filename);
    bool Valid() const { return valid_; }

private:
    RacePOD data_;
    bool valid_;
};

class GameConfig final {
public:
    explicit GameConfig(const std::string& configDir);

    auto& GetRaceProfDef() const { return *raceProfDef_; }
    auto& GetRaceProfConfig() const { return raceProfConfig_; }
    bool Valid() const { return valid_; }

private:
    std::unique_ptr<RaceProfDef> raceProfDef_;
    std::vector<std::unique_ptr<RaceProfConfig>> raceProfConfig_;
    bool valid_;
};