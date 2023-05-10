#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string_view>
#include <tuple>
#include <unordered_map>

namespace refl {

template <typename... Args, typename Func>
void tuple_for_each(std::tuple<Args...> &t, Func &f) {
    _tuple_processor<decltype(t), Func, sizeof...(Args)>::_tuple_process(t, f);
};

template <typename... Args, typename Func>
void tuple_for_each_const(const std::tuple<Args...> &t, Func &f) {
    _tuple_processor<decltype(t), Func, sizeof...(Args)>::_tuple_process_const(
        t, f);
};

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

template <typename T, typename PointerT>
class MemberInfo {
public:
    using type = T;

    constexpr MemberInfo(PointerT pointer, std::string_view name)
        : pointer(pointer), name(name) {}

    constexpr std::string_view Name() const { return name; }

    constexpr auto Pointer() const { return pointer; }

private:
    PointerT pointer;
    std::string_view name;
};

template <size_t N, typename F, typename T, typename... Types>
constexpr std::optional<size_t> Foreach(const std::tuple<T, Types...> &t, F f) {
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

    constexpr ClassInfo(std::string_view name, std::tuple<Types...> t)
        : name_(name), members_(t) {}
    constexpr ClassInfo(std::string_view name) : name_(name) {}

    template <typename T>
    constexpr auto Member(T member, std::string_view name) {
        using RetT = std::decay<std::invoke_result_t<T, ClassType>>;
        return ClassInfo<ClassType, Types..., MemberInfo<RetT, T>>(
            name_, std::tuple_cat(members_, std::make_tuple(MemberInfo<RetT, T>{
                                                member, name})));
    }

    constexpr std::string_view Name() const { return name_; }

    constexpr bool HasMember(std::string_view name) const {
        return hasMember<0>(name);
    }

    constexpr const auto &GetMembers() const { return members; }

    template <typename F>
    void VisitMembers(F& f) {
        tuple_for_each(members_, f);
    }

    template <typename F>
    void VisitMembers(F& f) const {
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

template <typename ClassType>
constexpr auto Class(std::string_view name) {
    return ClassInfo<ClassType>(name);
}

template <typename ClassType>
struct TypeInfo;

#define ReflRegist(x)                    \
    namespace refl {                     \
    template <>                          \
    struct TypeInfo<decltype(x)::type> { \
        constexpr static auto info = x;  \
    };                                   \
    }

template <typename T>
constexpr decltype(TypeInfo<T>::info) GetClass() {
    return TypeInfo<T>::info;
}

}  // namespace refl