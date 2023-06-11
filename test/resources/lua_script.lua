-- require("defs")


---@param entity Entity
---@param cmds Commands
---@param res Resources
function Startup(entity, cmds, res)
    local signal = res:GetSignalManager();
    signal:Regist(1,
        ---@param c Commands
        ---@param q Querier
        ---@param r Resources
        ---@param e Events
        function (c, q, r, e)
            print("regist function called") 
        end)

    local context = res:GetShareContext()
    context.context = {
        msg = "hello",
    }
end

---@param entity Entity
---@param cmds Commands
---@param querier Querier
---@param res Resources
---@param events Events
function Run(entity, cmds, querier, res, events)
    local renderer = res:GetRenderer();
    renderer:SetDrawColor(Color.new(0, 200, 0, 255));
    renderer:DrawRect(Rect.new(100, 200, 100, 50));

    local mouse = res:GetMouse();
    if mouse:LeftBtn():IsPressed() then
        local signal = res:get_signal_manager()
        signal:Raise(0, cmds, querier, res, events)
        signal:Raise(1, cmds, querier, res, events)

        local context = res:GetShareContext()
        print(context.context.msg)
    end
end
