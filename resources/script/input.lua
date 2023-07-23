-- a generic input module for cross platform

require("gamelib")
require("class")

input = {}

---@enum State
input.State = {
    Unknown = 0,
    Pressed = 1,
    Pressing = 2,
    Released = 3,
    Releasing = 4,
}

---@class ActionState
---@field private _state State
input.ActionState = {
    _state = input.State.Unknown,
}

---@param state State
---@return ActionState
function input.ActionState.new(state)
    local o = class.class(input.ActionState)
    o._state = state
    return o
end

---@return boolean
function input.ActionState:IsPressed()
    return self._state == input.State.Pressed
end

---@return boolean
function input.ActionState:IsPressing()
    return self._state == input.State.Pressing
end

---@return boolean
function input.ActionState:IsReleased()
    return self._state == input.State.Released
end

---@return boolean
function input.ActionState:IsReleasing()
    return self._state == input.State.Releasing
end

---@class KeyboardInput
---@field private _actions table<string, Key>
---@field private _keyboard Keyboard|nil
input.KeyboardInput = {
    _actions = {},
    _keyboard = nil,
}

---@param keyboard Keyboard
---@param actions table<string, Key>
---@return KeyboardInput
function input.KeyboardInput.new(keyboard, actions)
    local o = class.resources(input.KeyboardInput)
    o._actions = actions
    o._keyboard = keyboard
    return o
end

---@param action_name string
---@return ActionState
function input.KeyboardInput:GetActionState(action_name)
    if self._actions[action_name] == nil then
        log.log("[Input]: your action ", action_name, " not defined")
        return input.ActionState.new(input.State.Unknown)
    end

    local action_key = self._actions[action_name]
    local key = self._keyboard:Key(action_key)
    if key:IsPressed() then
        return input.ActionState.new(input.State.Pressed)
    elseif key:IsPressing() then
        return input.ActionState.new(input.State.Pressing)
    elseif key:IsReleased() then
        return input.ActionState.new(input.State.Released)
    else
        return input.ActionState.new(input.State.Releasing)
    end
end

---@return Vector2
function input.KeyboardInput:Axis()
    local axis = gamelib.Vector2.new()
    local state = self:GetActionState("left")
    if state:IsPressed() or state:IsPressing() then
        axis.x = axis.x - 1;
    end
    state = self:GetActionState("right")
    if state:IsPressed() or state:IsPressing() then
        axis.x = axis.x + 1;
    end
    state = self:GetActionState("up")
    if state:IsPressed() or state:IsPressing() then
        axis.y = axis.y + 1;
    end
    state = self:GetActionState("down")
    if state:IsPressed() or state:IsPressing() then
        axis.y = axis.y - 1;
    end
    return axis
end