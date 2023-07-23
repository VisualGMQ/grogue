copy = {}

function copy.deepcopy(a)
    if type(a) ~= "table" then
        return a
    end

    local o = {}
    for k, v in ipairs(a) do
        if type(v) ~= "table" then
            o[k] = _M.deepcopy(v)
        end
    end
    local m = getmetatable(a)
    setmetatable(o, m)
    return o
end