#pragma once

#include <tuple>
#include <unordered_map>
#include <string_view>
#include <memory>
#include <functional>

namespace refl {

template <typename PointerT, typename T>
class MemberInfo {
public:
    using type = T;

    constexpr MemberInfo(PointerT pointer, std::string_view name): pointer(pointer), name(name) {}
    constexpr std::string_view Name() const { return name; }

private:
    PointerT pointer;
    std::string_view name;
};

template <typename ClassType, typename... Types>
class ClassInfo final {
public:
    using type = ClassType;

    constexpr ClassInfo(std::tuple<Types...> t): members(t) {}
    constexpr ClassInfo(std::string_view name): name(name) {}

    static constexpr ClassInfo<ClassType> RegistClass(std::string_view name) {
        return ClassInfo<ClassType>(name);
    }

    template <typename T>
    constexpr auto Member(T member, std::string_view name) {
        using RetT = std::decay<std::invoke_result_t<T, ClassType>>;
        return ClassInfo<ClassType, Types..., MemberInfo<T, RetT>>(std::tuple_cat(members, std::make_tuple(MemberInfo<T, RetT>{member, name})));
    }

    constexpr std::string_view Name() const { return name; }

private:
    std::string_view name;
    std::tuple<Types...> members;
};

template <typename ClassType>
constexpr auto Class(std::string_view name) {
    return ClassInfo<ClassType>(name);
}

}