#pragma once
#include "core/pch.hpp"

// entity's material
struct Material final {
    float opaque;   // in [0, 1]. 0 means transparent, 1 means opaque
    float solid;    // in [0, 1]. [0, 0.3) means gass, [0.3, 0.6) means liquid, [0.6, 1.0] mean solid
    float lumen;    // in [0, +inf). 0 means no emission.
};

// monster race
enum Race {
    Human,
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
    Race race;
    Material material;
    MonsterProperty property;
    int level;
};

struct RaceAddition final {
    MonsterProperty addition;
};

// reflection MonsterProperty
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