---@class vector
---@field x number
---@field y number
local M = {}

---@param x number
---@param y number
---@return vector
function M.new(x, y)
    return {x = x, y = y}
end

return M
