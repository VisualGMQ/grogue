#pragma once

#include "core/pch.hpp"
#include "app/sprite.hpp"

// entity's material
struct Material final {
    float opaque;   // in [0, 1]. 0 means transparent, 1 means opaque
    float solid;    // in [0, 1]. [0, 0.3) means gass, [0.3, 0.6) means liquid, [0.6, 1.0] mean solid
    float lumen;    // in [0, +inf). 0 means no emission.
};

struct MonsterProperty final {
    int strength;       // affect attack damage, bearing amount
    int intelligence;   // affect mp recover speed
    int outsight;       // affect find trap speed
    int constitution;   // affect hp upbound, hp recover speed
    int agility;        // affect move speed and possibility of trigging trap
    int nutrition;
    int hp;
    int mp;
};

struct MonsterMetaProperty final {
    Material material;
    MonsterProperty basic; // basic property on level 0
    MonsterProperty max;   // max property, monster property can't beyond this property
};

struct MonsterInfo final {
    std::string_view race;
    Material material;
    MonsterProperty property;
    int level;
};

struct RaceAddition final {
    MonsterProperty addition;
};

struct EntityProperty final {
    float solid; //!< in [0, 100]
                 //!< - [0.0, 30): gass
                 //!< - [30, 60): liquid
                 //!< - [60, 100]: solid
    float lumen; //!< in [0, +inf)
    float heat;  //!< in [0, +inf)
    float sharp; //!< in [0, 100)
};

enum class BuffType {
    Poison,
    Burn,
    Bleed,
    Stun,
    Cold,
};

struct Buff final {
    BuffType type;
    float time;     //!< the buffer continue time
    float value;    //!< the buffer energy
    std::function<float(float)> change; //!< the buffer change while time pass

    void Update(float time) {
        this->time = std::max(this->time - time, 0.0f);
        if (change) {
            value = change(time);
        }
        if (value < 0.0 || time == 0) {
            value = 0.0;
        }
    }
};

//! @brief backpack component for monster
struct Backpack final {
    std::vector<ecs::Entity> items;
};

struct BackpackUIInfo final {
    int width;
    int height;
    int gridSize;
    int padding;
    int margin;

    int row; //!< calculate, not load from config
    int col; //!< calculate, not load from config
};

//! @brief the operation you can apply on item
using ItemOpFunc = std::function<bool(ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&)>;

//! @brief a component for item
struct Item final {
    std::string nameID;
};

struct ItemInfo final {
    std::string name;
    int weight;
    struct Operation {
        std::string operate;
        bool valid;
    };
    std::vector<Operation> operations;
    SpriteBundle sprite;
};

struct Terrian final {
    enum class Type {
        DryLand,
        Water,
    } type;
    EntityProperty property;
    SpriteBundle sprite;
};

constexpr float SCALE = 2.0;

// reflect

ReflRegist(
    refl::Class<Material>("Material")
        .Member(&Material::opaque, "opaque")
        .Member(&Material::solid, "solid")
        .Member(&Material::lumen, "lumen")
)

ReflRegist(
    refl::Class<MonsterProperty>("MonsterProperty")
    .Member(&MonsterProperty::hp, "hp")
    .Member(&MonsterProperty::mp, "mp")
    .Member(&MonsterProperty::strength, "strength")
    .Member(&MonsterProperty::intelligence, "intelligence")
    .Member(&MonsterProperty::outsight, "outsight")
    .Member(&MonsterProperty::constitution, "constitution")
    .Member(&MonsterProperty::agility, "agility")
    .Member(&MonsterProperty::nutrition, "nutrition")
);