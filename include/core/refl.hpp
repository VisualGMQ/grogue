#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string_view>
#include <tuple>
#include <unordered_map>

//! @brief a very small dynamic reflection framework, only use for POD and can
//! only reflect public member variables. See unittest "refl.cpp" for usage
//! @file test/refl.cpp
namespace refl {

template <typename Tuple, typename Func, size_t N>
struct _tuple_processor {
    inline static void _tuple_process(Tuple &t, Func &f) {
        _tuple_processor<Tuple, Func, N - 1>::_tuple_process(t, f);
        f(std::get<N - 1>(t));
    }

    inline static void _tuple_process_const(const Tuple &t, Func &f) {
        _tuple_processor<Tuple, Func, N - 1>::_tuple_process(t, f);
        f(std::get<N - 1>(t));
    }
};

template <typename Tuple, typename Func>
struct _tuple_processor<Tuple, Func, 1> {
    inline static void _tuple_process(Tuple &t, Func &f) { f(std::get<0>(t)); }

    inline static void _tuple_process_const(const Tuple &t, Func &f) {
        f(std::get<0>(t));
    }
};

//! @brief a help function to travese std::tuple, for elems need to be change
//! @tparam ...Args will be auto-derivated
//! @tparam Func will be auto-deriveted
//! @param t the traversed tuple
//! @param f a function like `void(auto& elem)` which elem is member of t
//! @sa tuple_for_each_const
template <typename... Args, typename Func>
void tuple_for_each(std::tuple<Args...> &t, Func &f) {
    _tuple_processor<decltype(t), Func, sizeof...(Args)>::_tuple_process(t, f);
};

//! @brief a help function to travese std::tuple, for elems not need to be change
//! @tparam ...Args will be auto-derivated
//! @tparam Func will be auto-deriveted
//! @param t the traversed tuple
//! @param f a function like `void(const auto& elem)` which elem is member of t
//! @sa tuple_for_each
template <typename... Args, typename Func>
void tuple_for_each_const(const std::tuple<Args...> &t, Func &f) {
    _tuple_processor<decltype(t), Func, sizeof...(Args)>::_tuple_process_const(
        t, f);
};

//! @brief class member variable information, contain the member pointer and member name
//! @tparam T member type
//! @tparam PointerT member pointer type
template <typename T, typename PointerT>
class MemberInfo {
public:
    using type = T;
    using pointer_t = PointerT;

    constexpr MemberInfo(pointer_t pointer, std::string_view name)
        : pointer(pointer), name(name) {}

    //! @brief get member name, can use in compile-time
    //! @return a literal name string
    constexpr std::string_view Name() const { return name; }

    //! @brief get member variable pointer, use `classInstance.*(pointer)` to access instance member variable
    //! @return the memeber variable pointer
    constexpr auto Pointer() const { return pointer; }

private:
    pointer_t pointer;       //!< @brief member variable pointer, use `classInstance.*(pointer)` to access instance member variable
    std::string_view name;  //!< @brief member name
};

//! @brief class informat, contain all reflected member vairbales information
//! @tparam ClassType class type
//! @tparam ...Types class member types
template <typename ClassType, typename... Types>
class ClassInfo final {
public:
    //! @brief class type
    using type = ClassType;

    constexpr ClassInfo(std::string_view name, std::tuple<Types...> t)
        : name_(name), members_(t) {}
    constexpr ClassInfo(std::string_view name) : name_(name) {}

    //! @brief reflect member variable and generate a new ClassInfo conatine the member
    //! @tparam T member variable pointer type
    //! @param member member variable pointer
    //! @param name member variable name
    //! @return a new ClassInfo which append the member info at the end
    template <typename T>
    constexpr auto Member(T member, std::string_view name) {
        using RetT = std::decay<std::invoke_result_t<T, ClassType>>;
        return ClassInfo<ClassType, Types..., MemberInfo<RetT, T>>(
            name_, std::tuple_cat(members_, std::make_tuple(MemberInfo<RetT, T>{
                                                member, name})));
    }

    //! @brief get class name
    //! @return class name literal string
    constexpr std::string_view Name() const { return name_; }

    //! @brief query whether has a member variable
    //! @param name member vairbale name
    //! @return 
    constexpr bool HasMember(std::string_view name) const {
        return hasMember<0>(name);
    }

    //! @brief get all reflected member vairbales in this class
    //! @return a tuple which contains all member variables
    constexpr const auto &GetMembers() const { return members_; }

    //! @brief pass a function f to visit all reflected class member variable,
    //! for variabel need change
    //! @tparam F function type, will auto-deriveted normally
    //! @param f function-object like `void(auto& elem)`
    template <typename F>
    void VisitMembers(F &f) {
        tuple_for_each(members_, f);
    }

    //! @brief pass a function f to visit all reflected class member variable,
    //! for variable don't need change
    //! @tparam F function type, will auto-deriveted normally
    //! @param f function-object like `void(auto& elem)`
    template <typename F>
    void VisitMembers(F &f) const {
        tuple_for_each_const(members_, f);
    }

private:
    std::string_view name_;
    std::tuple<Types...> members_;

    template <size_t N>
    constexpr bool hasMember(std::string_view name) const {
        if constexpr (std::tuple_size_v<decltype(members_)> == N) {
            return false;
        } else {
            if (std::get<N>(members_).Name() == name) {
                return true;
            } else {
                return hasMember<N + 1>(name);
            }
        }
    }
};

//! @brief a help function to create a empty class
//! @tparam ClassType class type
//! @param name class name
//! @return 
template <typename ClassType>
constexpr auto Class(std::string_view name) {
    return ClassInfo<ClassType>(name);
}

//! @brief a help class to save reflected class
//! @tparam ClassType class type
template <typename ClassType>
struct TypeInfo;

//! @brief a help macro to reflect a POD
#define ReflRegist(x)                    \
    namespace refl {                     \
    template <>                          \
    struct TypeInfo<decltype(x)::type> { \
        constexpr static auto info = x;  \
    };                                   \
    }

//! @brief get a reflected class
//! @tparam T class type
//! @return ClassInfo<...> which contain class information
//! @sa ClassInfo
template <typename T>
constexpr decltype(TypeInfo<T>::info) GetClass() {
    return TypeInfo<T>::info;
}

}  // namespace refl