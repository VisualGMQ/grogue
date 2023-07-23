require("class")
require("gamelib")
require("input")

local _M = {}

---@enum Direction
_M.Direction = {
    None = 0,
    Up = 0x01,
    Down = 0x02,
    Left = 0x04,
    Right = 0x08,
}

---@class Monster
_M.Monster = {
    ---@enum DirImageIdx
    DirImageIdx = {
        Right = 1,
        Up = 2,
        Down = 3,
    },

    ---@type Sprite[] only 3 sprites corresponding to to DirImageIdx
    sprites = {},
    ---@type Direction
    direction = 0,

    ---@param right Sprite
    ---@param up Sprite
    ---@param down Sprite
    CreateMonster = function(right, up, down)
        local m = _M.Monster.new()
        m.sprites = { right, up, down }
        return m
    end,

    ---@return Monster
    new = function()
        local o = class.component(_M.Monster)
        return o
    end
}

---@param querier Querier
function _M.MonsterUpdate(querier)
    local entities = querier:Query(monster.Monster.GetComponentID())
    for _, v in ipairs(entities) do
        ---@type Monster
        local monster = querier:Get(v, monster.Monster.GetComponentID())
        ---@type RigidBody
        local rigid = querier:Get(v, gamelib.Component.RigidBody)
        local vel = rigid.particle.vel
        if vel.x < 0 then
            monster.direction = _M.Direction.Left
        elseif vel.x > 0 then
            monster.direction = _M.Direction.Right
        elseif vel.y > 0 then
            monster.direction = _M.Direction.Down
        elseif vel.y < 0 then
            monster.direction = _M.Direction.Up
        end

        ---@type SpriteBundle
        local spriteBundle = querier:Get(v, gamelib.Component.SpriteBundle)
        if monster.direction == _M.Direction.Left then
            spriteBundle.flip = gamelib.Flip.Horizontal
            spriteBundle.sprite = monster.sprites[_M.Monster.DirImageIdx.Right]
        elseif monster.direction == _M.Direction.Right then
            spriteBundle.flip = gamelib.Flip.None
            spriteBundle.sprite = monster.sprites[_M.Monster.DirImageIdx.Right]
        elseif monster.direction == _M.Direction.Up then
            spriteBundle.flip = gamelib.Flip.None
            spriteBundle.sprite = monster.sprites[_M.Monster.DirImageIdx.Up]
        elseif monster.direction == _M.Direction.Down then
            spriteBundle.flip = gamelib.Flip.None
            spriteBundle.sprite = monster.sprites[_M.Monster.DirImageIdx.Down]
        end
    end
end

---@param querier Querier
---@param input KeyboardInput
function _M.MonsterMove(querier, input)
    local monsters = querier:Query(monster.Monster.GetComponentID())
    for _, m in ipairs(monsters) do
        ---@type RigidBody
        local rigid = querier:Get(m, gamelib.Component.RigidBody)
        local axis = input:Axis()
        local FORCE = 5.0
        rigid.particle.force.x = rigid.particle.force.x + axis.x * FORCE
        rigid.particle.force.y = rigid.particle.force.y - axis.y * FORCE
    end
end

monster = _M