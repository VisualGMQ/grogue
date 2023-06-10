require("defs")

local SignalFunc = {
    UpdateBackpackItem = 0,
}

---@param cmds Commands
---@param querier Querier
---@param res Resources
---@param events Events
local function UpdateBackpackItem(cmds, querier, res, events)
    local entities = querier.query_backpack_ui_panel()
    if entities:empty() then
        return
    end

    local backpack_ui_entity = entities[0]
    entities = querier.query_player()
    if entities:empty() then
        return
    end

    local player_entity = entities[0]
end

---@param entity Entity
---@param cmds Commands
---@param res Resources
function Startup(entity, cmds, res)
    local signalMgr = res:get_signal_manager();
    signalMgr.regist(SignalFunc.UpdateBackpackItem, UpdateBackpackItem)
end

---@param entity Entity
---@param cmds Commands
---@param querier Querier
---@param res Resources
function Run(entity, cmds, querier, res)
end