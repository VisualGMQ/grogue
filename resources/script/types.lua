local class = require("class")

types = {}

---@class Material
types.Material = {
    ---@type number in [0, 1000]. 0 means transparent, 1000 means opaque
    opaque = 0,

    ---@type number in [0, 1]. [0, 0.3) means gass, [0.3, 0.6) means liquid, [0.6, 1.0] mean solid
    solid = 0,

    ---@type number in [0, +inf). 0 means no emission
    lumen = 0,

    ---@type number in [0, +inf), 0 means absolutly zero degree
    heat = 0,

    ---@type number in [0, 1000], 0 means a ball
    sharp = 0,

    ---@param opaque number
    ---@param solid number
    ---@param lumen number
    ---@param heat number
    ---@param sharp number
    ---@return Material
    new = function(opaque, solid, lumen, heat, sharp)
        ---@type Material
        local o = class.class(types.Material)
        o.lumen = lumen
        o.opaque = opaque
        o.solid = solid
        o.heat = heat
        o.sharp = sharp
        return o
    end
}

---@class MonsterProperty
types.MonsterProperty = {
    ---@type number
    strength = 0,

    ---@type number
    outsight = 0,

    ---@type number
    intelligence = 0,

    ---@type number
    hp = 0,

    ---@type number
    mp = 0,

    ---@param strength number
    ---@param intelligence number
    ---@param outsight number
    ---@param constitution number
    ---@param agility number
    ---@param nutrition number
    ---@param hp number
    ---@param mp number
    new = function(strength, intelligence, outsight, constitution, agility, nutrition, hp, mp)
        ---@type MonsterProperty
        local o = class.class(types.MonsterProperty)
        o.strength = strength
        o.outsight = outsight
        o.intelligence = intelligence
        o.hp = hp
        o.mp = mp
        return o
    end
}

---@class MonsterMetaProperty
types.MonsterMetaProperty = {
    ---@type Material|nil
    material = nil,

    ---@type MonsterProperty|nil
    basicProp = nil,

    ---@type MonsterProperty|nil
    maxProp = nil,


    ---@type function
    ---@param material Material
    ---@param basicProp MonsterProperty
    ---@param maxProp MonsterProperty
    ---@return MonsterMetaProperty
    new = function(material, basicProp, maxProp)
        local o = class.class(types.MonsterMetaProperty)
        o.material = material
        o.basicProp = basicProp
        o.maxProp = maxProp
        return o
    end
}

---@class MonsterInfo
types.MonsterInfo = {
    ---@type string|nil
    race = nil,
    ---@type Material|nil
    material = nil,
    ---@type MonsterProperty|nil
    property = nil,
    ---@type number
    level = 0,

    ---@param race string
    ---@param material Material
    ---@param property MonsterProperty
    ---@param level number
    new = function(race, material, property, level)
        ---@type MonsterInfo
        local o = class.class(types.MonsterInfo)
        o.race = race
        o.material = material
        o.property = property
        o.level = level
        return o
    end
}

---@enum BuffType
types.BuffType = {
    Poision = 0,
    Burn = 1,
    Bleed = 2,
    Stun = 3,
    Cold = 4,
}

---@enum TerrianType
types.TerrianType = {
    DryLand = 0,
    Water = 1,
}

---@class Terrian
types.Terrian {
    ---@type TerrianType
    type = types.TerrianType.DryLand,
    ---@type Material|nil
    material = nil,
    ---@type SpriteBundle|nil
    sprite = nil,

    ---@param type TerrianType
    ---@param material Material
    ---@param sprite SpriteBundle
    new = function(type, material, sprite)
        ---@type Terrian
        local o = class.class(types.Terrian)
        o.type = type
        o.material = material
        o.sprite = sprite
        return o
    end
}

---@class Item
types.Item = {
    ---@type string
    id = '',
    ---@type number
    amount = 1,

    ---@param id string
    ---@param amount number
    new = function(id, amount)
        ---@type Item
        local o = class.class(types.Item)
        o.id = id
        o.amount = amount
        return o
    end
}