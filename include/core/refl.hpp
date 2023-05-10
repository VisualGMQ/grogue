#pragma once

#include <functional>
#include <memory>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <optional>

namespace refl {

template <typename PointerT, typename T>
class MemberInfo {
public:
    using type = T;

    constexpr MemberInfo(PointerT pointer, std::string_view name)
        : pointer(pointer), name(name) {}

    constexpr std::string_view Name() const { return name; }

private:
    PointerT pointer;
    std::string_view name;
};

template <size_t N, typename F, typename T, typename... Types>
constexpr std::optional<size_t> Foreach(const std::tuple<T, Types...>& t, F f) {
    if constexpr (std::tuple_size_v<decltype(t)> == N) {
        return std::nullopt;
    } else {
        bool value = f.Call<N>(t);
        if (value) {
            return N;
        } else {
            return Foreach<N + 1>(t, f);
        }
    }
}

template <typename ClassType, typename... Types>
class ClassInfo final {
public:
    using type = ClassType;

    constexpr ClassInfo(std::string_view name, std::tuple<Types...> t) : name_(name), members_(t) {}

    constexpr ClassInfo(std::string_view name) : name_(name) {}

    template <typename T>
    constexpr auto Member(T member, std::string_view name) {
        using RetT = std::decay<std::invoke_result_t<T, ClassType>>;
        return ClassInfo<ClassType, Types..., MemberInfo<T, RetT>>(name_,
            std::tuple_cat(members_,
                           std::make_tuple(MemberInfo<T, RetT>{member, name})));
    }

    constexpr std::string_view Name() const { return name_; }

    constexpr const auto& GetMembers() const {
        return members;
    }

private:
    std::string_view name_;
    std::tuple<Types...> members_;
};

template <typename ClassType>
constexpr auto Class(std::string_view name) {
    return ClassInfo<ClassType>(name);
}

template <typename ClassType>
struct TypeInfo;

#define ReflRegist(x) \
namespace refl { \
    template<> \
    struct TypeInfo<decltype(x)::type> { \
        constexpr static auto info = x; \
    }; \
}

template <typename T>
constexpr decltype(TypeInfo<T>::info) GetClass() {
    return TypeInfo<T>::info; 
}

}  // namespace refl