local _M = {
    _cur_id = 0,
}

---@param data table
function _M.RegistComponent(data)
    data.component_id = _M._cur_id
    _M._cur_id = _M._cur_id + 1
end

return _M