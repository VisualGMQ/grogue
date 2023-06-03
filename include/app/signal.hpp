#pragma once

#include "core/pch.hpp"
#include "app/manager.hpp"

class SignalManager final {
public:
    using CallbackFunc = std::function<void(ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&)>;

    void Regist(const std::string& category, const std::string& name, CallbackFunc func) {
        if (auto it = categories_.find(category); it != categories_.end()) {
            it->second.push_back(name);
        } else {
            categories_.insert({category, {name}});
        }
        callbacks_[name] = func;
    }

    void Remove(const std::string& name) {
        callbacks_.erase(name);
    }

    void Raise(const std::string& name, ecs::Commands& cmds, ecs::Querier querier, ecs::Resources res, ecs::Events& events) {
        if (auto it = callbacks_.find(name); it != callbacks_.end()) {
            (it->second)(cmds, querier, res, events);
        }
    }

    void RaiseCategory(const std::string& category, ecs::Commands& cmds, ecs::Querier querier, ecs::Resources res, ecs::Events& events) {
        if (auto it = categories_.find(category); it != categories_.end()) {
            auto& names = it->second;
            for (int i = names.size() - 1; i >= 0; i--) {
                const auto& name = names[i];
                if (auto it2 = callbacks_.find(name); it2 != callbacks_.end()) {
                    (it2->second)(cmds, querier, res, events);
                } else {
                    std::swap(names[i], names.back());
                    names.pop_back();
                }
            }
        }
    }

private:
    std::unordered_map<std::string, std::vector<std::string>> categories_;
    std::unordered_map<std::string, CallbackFunc> callbacks_;
};