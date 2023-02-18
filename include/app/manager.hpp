#pragma once

#include "app/handle.hpp"

template <typename T>
class Manager {
public:
    void Destroy(Handle<T> handle) {
        datas_.erase(handle);
    }

    T& Get(Handle<T> handle) {
        return *datas_[handle];
    }

    bool Has(Handle<T> handle) {
        return datas_.find(handle) != datas_.end();
    }

protected:
    void storeNewItem(Handle<T> handle, std::unique_ptr<T>&& item) {
        if (handle) {
            datas_.emplace(handle, std::move(item));
        }
    }

private:
    std::unordered_map<Handle<T>, std::unique_ptr<T>,
                       typename Handle<T>::Hash, typename Handle<T>::HashEq>
        datas_;
};

template <typename T>
using ResourceManager = Manager<T>;
