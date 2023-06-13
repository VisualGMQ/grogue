require("defs")

---@param entity Entity
---@param cmds Commands
---@param res Resources
function Startup(entity, cmds, res)
    print("startup")
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

        PlayerMove(keyboard, monster)

        if keyboard:Key(Key.KEY_SPACE):IsPressed() and
            querier:HasBackpack(entity) then
           local backpack = querier:GetBackpack(entity) 
           PickupItemOnTile(monster, backpack, cmds:Raw(), querier:Raw(), res:Raw(), events:Raw())
        end

        if keyboard:Key(Key.KEY_TAB):IsPressed() then
            ToggleBackpackUIPanel(querier:Raw(), res:Raw())
        end

        :: NEXT_LOOP ::
    end
end