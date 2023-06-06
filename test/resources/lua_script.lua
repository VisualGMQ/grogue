-- require("defs")

---@param entity Entity
---@param res Resources
---@param querier Querier
function Run(entity, res, querier)
    local renderer = res:get_renderer();
    renderer:set_drawcolor(Color.new(0, 200, 0, 255));
    renderer:draw_rect(Rect.new(100, 200, 100, 50));
end