---@class color
---@field r number
---@field g number
---@field b number
---@field a number
local M = {}

---@param r number
---@param g number
---@param b number
---@param a number
---@return color
function M.new(r, g, b, a)
    if a == nil then
        return {r = r, g = g, b = b, a = 255}
    else
        return {r = r, g = g, b = b, a = a}
    end
end

return M
