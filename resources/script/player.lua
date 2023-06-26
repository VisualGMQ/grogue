require("defs")
require("math")

---@param entity Entity
---@param cmds Commands
---@param res Resources
function Startup(entity, cmds, res)
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
    ---@type Input
    local input = res:GetInput()

    local entities = querier:QueryPlayer()
    for _, entity in pairs(entities) do
        if not querier:HasMonster(entity) then
            goto NEXT_LOOP
        end
        local monster = querier:GetMonster(entity)
        local particle = querier:GetParticle(entity)

        PlayerMove(input, monster, particle)

        if input:GetActionState("pickup"):IsPressed() and
            querier:HasBackpack(entity) then
           local backpack = querier:GetBackpack(entity) 
           PickupTileOneItem(backpack, cmds:Raw(), querier:Raw(), res:Raw(), events:Raw())
        end

        if input:GetActionState("toggle_backpack_ui"):IsPressed() then
            ToggleBackpackUIPanel(querier, res)
        end

        :: NEXT_LOOP ::
    end
end