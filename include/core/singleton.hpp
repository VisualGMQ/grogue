#pragma once

#include <memory>
#include <cassert>

template <typename T, bool ExplicitInit>
class Singleton;

template <typename T>
class Singleton<T, false> {
public:
    static T& Instance() {
        static T instance;
        return instance;
    }
};

template <typename T>
class Singleton<T, true> {
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