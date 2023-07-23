require("math")
require("gamelib")
require("input")
require("monster")

local Script = {}

---@param tilesheet_mgr TilesheetManager
local function loadResource(tilesheet_mgr)
    tilesheet_mgr:LoadFromConfig("./resources/img/tilesheet.lua")
    tilesheet_mgr:LoadFromConfig("./resources/img/role.lua")
end

---@param physicWorld PhysicWorld
local function setFriction(physicWorld)
    physicWorld.forceGenerators:add(
        ---@param p Particle
        ---@param t number
        function(p, t)
            if p.vel:LengthSqrd() > 0.000001 then
                local friction_factor = 6.0
                p.force = p.force - gamelib.Normalize(p.vel) * 1.0 / p.massInv * friction_factor
            end
        end)
end

---@param keyboard Keyboard
---@param cmds Commands
local function initInput(keyboard, cmds)
    local config = require("config.game_conf")
    local actions = config.actions
    for k, _ in pairs(actions) do
        actions[k] = gamelib.GetKeyFromName(actions[k])
    end
    local input = input.KeyboardInput.new(keyboard, actions)
    cmds:SetRes(input)
end

---@param cmds Commands
---@param res Resources
local function initMonster(cmds, res)
    ---@type TilesheetManager
    local tilesheetMgr = res:Get(gamelib.Resource.TilesheetManager)
    local tilesheet = tilesheetMgr:Find("role")
    local downSprite = gamelib.Sprite.FromRegion(tilesheet:Get(0, 0).region)
    local rightSprite = gamelib.Sprite.FromRegion(tilesheet:Get(1, 0).region)
    local upSprite = gamelib.Sprite.FromRegion(tilesheet:Get(2, 0).region)

    local m = monster.Monster.CreateMonster(rightSprite, upSprite, downSprite)

    local entity = cmds:Spawn()
    cmds:AddComponent(entity, m)
    local spriteBundle = gamelib.SpriteBundle.new()
    spriteBundle.image = tilesheet:Handle()
    spriteBundle.sprite = downSprite
    cmds:AddComponent(entity, spriteBundle)
    local transform = gamelib.Transform.FromPosition(gamelib.Vector2.new(100, 100))
    cmds:AddComponent(entity, transform)
    local rigidBody = gamelib.physics.RigidBody.new()
    cmds:AddComponent(entity, rigidBody)
end

---@param entity Entity
---@param cmds Commands
---@param res Resources
function Script.Startup(entity, cmds, res)
    local tilesheet_mgr = res:Get(gamelib.Resource.TilesheetManager)
    loadResource(tilesheet_mgr)
    -- setFriction(res:Get(gamelib.Resource.PhysicWorld))
    initInput(res:Get(gamelib.Resource.Keyboard), cmds)
    initMonster(cmds, res)
end

---@param querier Querier
---@param res Resources
local function ToggleBackpackUIPanel(querier, res)
    local entities = querier:Query(gamelib.Component.BackpackUIPanel)

    if not entities:empty() then
        local window = res:Get(gamelib.Resource.Window)
        ---@type RectTransform
        local transform = querier:GetRectTransform(entities:at(1))
        local y = transform.transform.localTransform.position.y

        if y >= 0 and y < window:GetSize().y then
            transform.transform.localTransform.position.y = window:GetSize().y
        else
            local info = res:GetGameConfig():GetBackpackUIConfig():Info()
            transform.transform.localTransform.position.y = window:GetSize().y - info.height
        end
    end
end


---@param entity Entity
---@param cmds Commands
---@param querier Querier
---@param res Resources
---@param events Events
function Script.Run(entity, cmds, querier, res, events)
    ---@type KeyboardInput
    local input = res:Get(input.KeyboardInput.GetComponentID())

    monster.MonsterUpdate(querier)
    monster.MonsterMove(querier, input)
end

return Script