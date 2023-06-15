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
        ---@param p table
        function (c, q, r, e, p)
            print("regist function called") 
            print(p.msg)
        end)

    local context = res:GetLuaShareContext()
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
    -- print("running")
    local renderer = res:GetRenderer();
    renderer:SetDrawColor(Color.new(0, 200, 0, 255));
    renderer:DrawRect(Rect.new(100, 200, 100, 50));

    local mouse = res:GetMouse();
    if mouse:LeftBtn():IsPressed() then
        local signal = res:GetSignalManager()
        signal:Raise(0, cmds, querier, res, events)
        signal:Raise(1, cmds, querier, res, events, {msg = "raised"})

        local context = res:GetLuaShareContext()
        print(context.context.msg)
    end
end
