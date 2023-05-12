#pragma once

#include <memory>
#include <cassert>

//! @addtogroup utilities
//! @brief some utilities
//! @{

//! @brief A help class to auto-singlton by inherit this
//! @tparam T the class you want to be singleton
//! @tparam ExplicitInit does you need explicit init/destroy the instance
template <typename T, bool ExplicitInit>
class Singleton;

//! @brief A help class to auto-singlton by inherit this
//! @tparam T the class you want to be singleton
template <typename T>
class Singleton<T, false> {
public:
    //! @brief Get the singleton of class T
    //! @return The singleton instance
    static T& Instance() {
        static T instance;
        return instance;
    }
};

//! @brief A help class to auto-singlton by inherit this, but the singleton need use `Init()` and `Delete()` to create and destroy
//! this is for which need initialize/destroy in order
//! @tparam T the class you want to be singleton
template <typename T>
class Singleton<T, true> {
public:
    //! @brief Get the singleton of class T
    //! @return The singleton instance
    static T& Instance() {
        assert(instance_ == nullptr);
        return instance_.get();
    }

    //! @brief Explicit init the singleton.You must call this before call `Instance()`
    //! @param ...args the params pass to constructor of T
    template <typename... Args>
    static void Init(Args&&... args) {
        instance_ = new T(std::forward<Args>(args)...);
    }

    //! @brief Explicit destroy the singlton.You can't call `Instance()` after call this
    static void Delete() {
        instance_.reset();
        instance_ = nullptr;
    }

private:
    inline static std::unique_ptr<T> instance_;
};

//! @}