package.cpath = package.cpath .. ";./build/?.dylib"

local script = require "libscript"

local M = {}

---@param p1 vector
---@param p2 vector
---@param color color
function M.DrawLine(p1, p2, color)
    script.DrawLine(p1.x, p1.y,
                    p2.x, p2.y,
                    color.r, color.g, color.b, color.a)
end

return M
