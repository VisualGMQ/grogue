local class = require "class"
local copy = require "copy"
matrix = {}

---@class Matrix
matrix.Matrix = {
    _w = 0,
    _h = 0,
    _data = {}
}

---@param w number
---@param h number
---@return Matrix
function matrix.Matrix.new(w, h)
    local o = class.class(matrix.Matrix)
    o._w = w
    o._h = h
    return o
end

---@return number
function matrix.Matrix:Width()
    return self._w
end

---@return number
function matrix.Matrix:Height()
    return self._h
end

---@return any
function matrix.Matrix:Get(x, y)
    return self._data[x + self._w * y]
end

function matrix.Matrix:Set(x, y, value)
    self._data[x + self._w * y] = value
end

function matrix.Matrix:Size(x, y, value)
    return self:Width() * self:Height()
end

function matrix.Matrix:Fill(value)
    for i, _ in ipairs(self._data) do
        self._data[i] = copy.deepcopy(value)
    end
end