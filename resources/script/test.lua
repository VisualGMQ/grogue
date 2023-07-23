require "gamelib"

local Script = {}

---@param cmds Commands
---@param res  Resources
function Script.Startup(_, cmds, res)
    print("here")
    local tilesheet_mgr = res:Get(gamelib.Resource.TilesheetManager)
    ---@type Tilesheet
    local tilesheet = tilesheet_mgr:LoadFromConfig("./resources/img/role.lua")
    local entity = cmds:Spawn()
    ---@type SpriteBundle
    local sprite_bundle = gamelib.SpriteBundle.new()
    print(sprite_bundle.flip)
    local tile = tilesheet:Get(0, 0)
    sprite_bundle.image = tile.handle
    sprite_bundle.sprite = gamelib.Sprite.FromRegion(tile.region)
    cmds:AddComponent(entity, sprite_bundle)
    local transform = gamelib.Transform.FromPosition(Vector2.new(100, 100))
    cmds:AddComponent(entity, transform)
end

function Script.Run(entity, cmds, querier, res, events)
    local renderer = res:Get(gamelib.Resource.Renderer)
    renderer:SetDrawColor(gamelib.Color.new(0, 255, 0, 255))
    renderer:Clear();
end

return Script