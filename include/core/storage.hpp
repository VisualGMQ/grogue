#pragma once

#include "pch.hpp"
#include "log.hpp"

namespace grogue::core {

template <typename KeyT, typename ElemT>
class Storage {
public:
    using Container = std::unordered_map<KeyT, ElemT>;

    virtual ~Storage() = default;
    Storage() = default;
    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;

    template <typename... Args>
    ElemT& Create(const KeyT& name, Args&&... args) {
        auto it = datas_.find(name);
        if (it != datas_.end()) {
            // TODO  Add Log here
            return it->second;
        } else {
            auto it = datas_.emplace(std::piecewise_construct,
                                     std::forward_as_tuple(name),
                                     std::forward_as_tuple(std::forward<Args>(args)...));
            return it.first->second;
        }
    }

    void Remove(const KeyT& name) {
        auto it = datas_.find(name);
        if (it != datas_.end()) {
            datas_.erase(it);
        }
    }

    ElemT* Find(const KeyT& name) {
        auto it = datas_.find(name);
        if (it == datas_.end()) {
            return nullptr;
        } else {
            return &(it->second);
        }
    }

    bool IsExists(const KeyT& name) const {
        return datas_.find(name) != datas_.end();
    }

    void Clear() {
        datas_.clear();
    }

    auto begin() {
        return datas_.begin();
    }

    auto end() {
        return datas_.end();
    }

    auto cbegin() const {
        return datas_.cbegin();
    }

    auto cend() const {
        return datas_.cend();
    }

protected:
    Container datas_;
};

}
