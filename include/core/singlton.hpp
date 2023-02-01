#pragma once

#include <memory>
#include <cassert>

template <typename T, bool ExplicitInit>
class Singlton;

template <typename T>
class Singlton<T, false> {
public:
    static T& Instance() {
        static auto instance;
        return instance;
    }
};

template <typename T>
class Singlton<T, true> {
public:
    static T& Instance() {
        assert(instance_ == nullptr);
        return instance_.get();
    }

    template <typename... Args>
    static void Init(Args&&... args) {
        instance_ = new T(std::forward<Args>(args)...);
    }

    static void Delete() {
        instance_.reset();
        instance_ = nullptr;
    }

private:
    inline static std::unique_ptr<T> instance_;
};