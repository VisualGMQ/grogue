require("debug")
require("log")

class = { }

---@enum Tag
class.Tag = {
    Normal = 0,
    Component = 1,
    Resource = 2,
}

class.LuaComponentStartIdx = 100
class._curId = class.LuaComponentStartIdx

function class._GenerateID()
    local id = class._curId
    class._curId = class._curId + 1
    return id
end

function class.class(clazz)
    if clazz._tag == nil then
        clazz._tag = class.Tag.Normal
    end
    local o = {}
    setmetatable(o, clazz)
    clazz.__index = clazz
    return o
end

function class.component(clazz)
    if clazz._component_id == nil and clazz._tag == nil then
        clazz._tag = class.Tag.Component
        clazz._component_id = class._GenerateID()
        clazz.GetComponentID = function()
            return clazz._component_id
        end
    end
    local o = {}
    setmetatable(o, clazz)
    clazz.__index = clazz
    return o
end

function class.resources(clazz)
    if clazz._component_id == nil and clazz._tag == nil then
        clazz._tag = class.Tag.Resource
        clazz._component_id = class._GenerateID()
        clazz.GetComponentID = function()
            return clazz._component_id
        end
    end
    local o = {}
    setmetatable(o, clazz)
    clazz.__index = clazz
    return o
end