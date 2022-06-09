local gfx = require "script.gfx"
local vector = require "script.vector"
local color = require "script.color"

function Update()
    gfx.DrawLine(vector.new(0, 0), vector.new(100, 100), color.new(0, 255, 0))
end
