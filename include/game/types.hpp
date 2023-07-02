#pragma once

#include "core/pch.hpp"
#include "app/sprite.hpp"

// entity's material
struct LUA_BIND Material final {
    float opaque;   // in [0, 1]. 0 means transparent, 1 means opaque
    float solid;    // in [0, 1]. [0, 0.3) means gass, [0.3, 0.6) means liquid, [0.6, 1.0] mean solid
    float lumen;    // in [0, +inf). 0 means no emission.
};

struct LUA_BIND MonsterProperty final {
    int strength;       // affect attack damage, bearing amount
    int intelligence;   // affect mp recover speed
    int outsight;       // affect find trap speed
    int constitution;   // affect hp upbound, hp recover speed
    int agility;        // affect move speed and possibility of trigging trap
    int nutrition;
    int hp;
    int mp;
};

struct LUA_BIND MonsterMetaProperty final {
    Material material;
    MonsterProperty basic; // basic property on level 0
    MonsterProperty max;   // max property, monster property can't beyond this property
};

struct LUA_BIND MonsterInfo final {
    std::string_view race;
    Material material;
    MonsterProperty property;
    int level;
};

struct LUA_BIND RaceAddition final {
    MonsterProperty addition;
};

struct LUA_BIND EntityProperty final {
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

//! @brief the operation you can apply on item
using ItemOpFunc = std::function<bool(ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&)>;

//! @brief game item
struct LUA_BIND Item final {
    std::string nameID; //!< raw nameID in GameConfig
    int amount = 1;
    // std::string name; // new name for item

    bool operator==(const Item& o) const {
        if (&o == this) {
            return true;
        }

        return o.nameID == nameID;
    }
};

struct LUA_BIND ItemOperation {
    std::string operate;
    bool valid;
};

struct LUA_BIND ItemInfo final {
    std::string name;
    int weight;
    bool architecture;
    std::vector<ItemOperation> operations;
    SpriteBundle sprite;
};

//! @brief backpack component for monster
struct LUA_BIND_COMPONENT Backpack final {
    std::vector<Item> items;    //<! the item in backpack

    Item leftHand;  //<! the item on the left hand
    Item rightHand; //<! the item on the right hand
};

//! @brief a cache to save which object mouse grabing(like item from backpack)
struct LUA_BIND_RESOURCE MouseGrabCache {
    std::optional<Item> item;   //<! @brief the item
    std::optional<ecs::Entity> entity; //<! @brief the item entity
    bool isGrabJustnow = false; //<! @brief is grab at same frame
};

struct LUA_BIND BackpackUIInfo final {
    int width;
    int height;
    int gridSize;
    int padding;
    int margin;
    math::Vector2 left_hand_position;
    math::Vector2 right_hand_position;

    int row; //!< calculate, not load from config
    int col; //!< calculate, not load from config
};

//! @brief a tag component to point out ui root node
struct LUA_BIND_COMPONENT UIRoot {};

enum class TerrianType {
    DryLand,
    Water,
};

struct Terrian final {
    TerrianType type;
    EntityProperty property;
    SpriteBundle sprite;
};

//! @brief tag component for backpack panel UI
struct LUA_BIND_COMPONENT BackpackUIPanel {};
//! @brief tag component for left hand ui panel
struct LUA_BIND_COMPONENT LeftHandUIPanel {};
//! @brief tag component for right hand ui panel
struct LUA_BIND_COMPONENT RightHandUIPanel {};

/// @brief a tag component to point out player entity
struct LUA_BIND_COMPONENT Player {};

constexpr float SCALE = 2.0;

//! @brief parameter pack for BackpackUIPanelUpdate signal
struct BackpackUIPanelUpdateParam {
    enum class Type {
        Pile,
        NewItem,
    } type;
    int value;
};

// reflect

/*
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
*/