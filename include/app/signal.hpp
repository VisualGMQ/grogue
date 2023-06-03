#pragma once

#include "core/pch.hpp"
#include "app/manager.hpp"

class SignalManager final {
public:
    // FIXME: I don't want to see the last void* parameter, want to replace it
    // by a concret type
    using CallbackFunc = std::function<void(
        ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&, void*)>;

    void Regist(uint32_t category, uint32_t name, CallbackFunc func) {
        if (auto it = categories_.find(category); it != categories_.end()) {
            it->second.push_back(name);
        } else {
            categories_.insert({category, {name}});
        }
        callbacks_[name] = func;
    }

    void Remove(uint32_t name) {
        callbacks_.erase(name);
    }

    void Raise(uint32_t name, ecs::Commands& cmds, ecs::Querier querier, ecs::Resources res, ecs::Events& events, void* param) {
        if (auto it = callbacks_.find(name); it != callbacks_.end()) {
            (it->second)(cmds, querier, res, events, param);
        }
    }

    void RaiseCategory(uint32_t category, ecs::Commands& cmds, ecs::Querier querier, ecs::Resources res, ecs::Events& events, void* param) {
        if (auto it = categories_.find(category); it != categories_.end()) {
            auto& names = it->second;
            for (int i = names.size() - 1; i >= 0; i--) {
                const auto& name = names[i];
                if (auto it2 = callbacks_.find(name); it2 != callbacks_.end()) {
                    (it2->second)(cmds, querier, res, events, param);
                } else {
                    std::swap(names[i], names.back());
                    names.pop_back();
                }
            }
        }
    }

private:
    std::unordered_map<uint32_t, std::vector<uint32_t>> categories_;
    std::unordered_map<uint32_t, CallbackFunc> callbacks_;
};