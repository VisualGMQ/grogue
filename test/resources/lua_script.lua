-- require("defs")

---@param entity Entity
---@param cmds Commands
---@param querier Querier
---@param res Resources
function Run(entity, cmds, querier, res)
    local renderer = res:get_renderer();
    renderer:set_drawcolor(Color.new(0, 200, 0, 255));
    renderer:draw_rect(Rect.new(100, 200, 100, 50));

    local mouse = res:get_mouse();
    if mouse:left_btn():is_pressed() then
        local signal = res:get_signal_manager()
        signal:raise(0)
    end
end
