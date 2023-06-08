#pragma once

#include "core/pch.hpp"
#include "app/manager.hpp"

class SignalManager final {
public:
    // FIXME: I don't want to see the last void* parameter, want to replace it
    // by a concret type
    using CallbackFunc = std::function<void(
        ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&, void*)>;

    void Regist(uint32_t name, CallbackFunc func) {
        callbacks_.insert_or_assign(name, func);
    }

    void Remove(uint32_t name) {
        callbacks_.erase(name);
    }

    void Raise(uint32_t name, ecs::Commands& cmds, ecs::Querier querier, ecs::Resources res, ecs::Events& events, void* param) {
        if (auto it = callbacks_.find(name); it != callbacks_.end()) {
            (it->second)(cmds, querier, res, events, param);
        }
    }

private:
    std::unordered_map<uint32_t, CallbackFunc> callbacks_;
};