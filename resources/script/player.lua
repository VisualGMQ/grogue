require("defs")
require("math")

---@param entity Entity
---@param cmds Commands
---@param res Resources
function Startup(entity, cmds, res)
    print("startup")
end



---@param monster Monster
---@param backack Backpack
---@param cmd Commands
---@param querier Querier
---@param res Resources
---@param events Events
local function PickupItemOnTile(monster, backpack, cmd, querier, res, events)
    local PickupHalfRange = 100
    local map = res:GetMapManager():GetCurrentMap()
    local hover = res:GetNearestItemHover()

    local x, y = math.floor(hover.position.x), math.floor(hover.position.y)

    if map.tiles:IsInRange(x, y) then
        local tile = map.tiles:Get(x, y)
        local items = querier:GetMapTile(tile).items
        if items:empty() then
            return;
        end

        backpack.items:add(items[items:size()])
        items:erase(items:size())

        if items:empty() then
            cmd:RemoveCollideShape(tile) 
        end

        for _, item in pairs(items) do
            print(item)
            local tile = querier:GetMapTile(item)
            print(tile)
        end

        local signalMgr = res:GetSignalManager()
        signalMgr:Raise(0, cmd, querier, res, events, {})
    end
end

---@param querier Querier
---@param res Resources
local function ToggleBackpackUIPanel(querier, res)
    local entities = querier:QueryBackpackUIPanel()

    if not entities:empty() then
        local window = res:GetWindow()
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
function Run(entity, cmds, querier, res, events)
    ---@type Keyboard
    local keyboard = res:GetKeyboard()

    local entities = querier:QueryPlayer()
    for _, entity in pairs(entities) do
        if not querier:HasMonster(entity) then
            goto NEXT_LOOP
        end
        local monster = querier:GetMonster(entity)
        local particle = querier:GetParticle(entity)

        PlayerMove(keyboard, monster, particle)

        if keyboard:Key(Key.KEY_SPACE):IsPressed() and
            querier:HasBackpack(entity) then
           local backpack = querier:GetBackpack(entity) 
           -- PickupItemOnTile(monster, backpack, cmds:Raw(), querier:Raw(), res:Raw(), events:Raw())
           PickupItemOnTile(monster, backpack, cmds, querier, res, events)
        end

        if keyboard:Key(Key.KEY_TAB):IsPressed() then
            ToggleBackpackUIPanel(querier, res)
        end

        :: NEXT_LOOP ::
    end
end