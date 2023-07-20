require("math")

local MyComponentID = 100

---@param entity Entity
---@param cmds Commands
---@param res Resources
function Startup(entity, cmds, res)
    local entity = cmds:Spawn()
    cmds:AddComponent(entity, MyComponentID, { name = "MyComponent" })
end

---@param querier Querier
---@param res Resources
local function ToggleBackpackUIPanel(querier, res)

    local entities = querier:Query(Component.BackpackUIPanel)

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
    local test_entities = querier:Query(MyComponentID)
    for _, entity in pairs(test_entities) do
        local comp = querier:GetSolObject(entity)
        -- print(comp.name)
    end

    ---@type Input
    local input = res:GetInput()

    local entities = querier:Query(Component.Player)
    for _, entity in pairs(entities) do
        if not querier:Has(entity, Component.Monster) then
            goto NEXT_LOOP
        end
        local monster = querier:GetMonster(entity)
        local particle = querier:GetParticle(entity)

        PlayerMove(input, monster, particle)

        if input:GetActionState("pickup"):IsPressed() and
            querier:Has(entity, Component.Backpack) then
           local backpack = querier:GetBackpack(entity) 
           PickupTileOneItem(backpack, cmds:Raw(), querier:Raw(), res:Raw(), events:Raw())
        end

        if input:GetActionState("toggle_backpack_ui"):IsPressed() then
            ToggleBackpackUIPanel(querier, res)
        end

        :: NEXT_LOOP ::
    end
end