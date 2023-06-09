require("defs")


---@param entity Entity
---@param cmds Commands
---@param res Resources
function Startup(entity, cmds, res)
    local signal = res:get_signal_manager();
    signal:regist(1,
        ---@param c Commands
        ---@param q Querier
        ---@param r Resources
        ---@param e Events
        function (c, q, r, e)
            print("regist function called") 
        end)

    local context = res:get_share_context()
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
    local renderer = res:get_renderer();
    renderer:set_drawcolor(Color.new(0, 200, 0, 255));
    renderer:draw_rect(Rect.new(100, 200, 100, 50));

    local mouse = res:get_mouse();
    if mouse:left_btn():is_pressed() then
        local signal = res:get_signal_manager()
        signal:raise(0, cmds, querier, res, events)
        signal:raise(1, cmds, querier, res, events)

        local context = res:get_share_context()
        print(context.context.msg)
    end
end
