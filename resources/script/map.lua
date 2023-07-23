require "types"
require "matrix"
require "class"
require "copy"

local _M = {}

---@class MapTile
_M.MapTile = {
    ---@type Terrian
    terrian = types.Terrian:new(types.TerrianType.DryLand, nil, nil),
    ---@type Matrix
    items = matrix.Matrix:new(0, 0),

    ---@return MapTile
    new = function(terrian, items)
        local o = class.component(_M.MapTile)
        o.terrian = terrian
        o.items = items
        return o
    end
}

---@class Map
_M.Map = {
    ---@type Matrix Matrix<MapTile>
    tiles = matrix.Matrix:new(0, 0),
    ---@type table<Entity>
    items = {},
    ---@type table<Entity>
    monsters = {},

    ---@return Map
    ---@param tiles Matrix
    ---@param items table<Entity>
    ---@param monsters table<Entity>
    new = function(tiles, items, monsters)
        local o = _M.Map.default()
        o.tiles = tiles
        o.items = items
        o.monsters = monsters
        return o
    end,

    ---@return Map
    default = function()
        local o = class.class(_M.Map)
        return o
    end,
}

---@class MapManager Singleton
---@field private _maps table<Map>
---@field private _currentIdx number
_M.MapManager = {
    _maps = {},
    _currentIdx = 0,

    ---@return MapManager
    new = function()
        local o = class.class(_M.MapManager)
        return o
    end
}

---@param map Map
function _M.MapManager:add(map)
    self._maps[#self._maps + 1] = copy.deepcopy(map)
end

---@type function genereate demo map
---@param cmds Commands
---@param res Resources
---@param querier Querier
---@param w number
---@param h number
---@return Map
function _M.Map.GenDebugDemo(cmds, res, querier, w, h)
    local map = _M.Map.default()
    map.tiles = matrix.Matrix.new(w, h)
    for y = 0, h do
        for x = 0, w do
            local entity = cmds:Spawn()
            local mapTile = _M.MapTile.new(
                    types.Terrian:new(
                        types.TerrianType.DryLand,
                        nil,
                        nil),
                    {})
            cmds:AddComponent(entity, copy.deepcopy(mapTile))
            _M.tiles:Set(x, y, entity)
        end 
    end
    return _M
end

map = _M