#pragma once

#include "app/lua.hpp"
#include "core/pch.hpp"
#include "game/types.hpp"

enum class ParseErrorType {
    NoError,
    IOError,
    ParseError,
};

struct IOError final {
    std::string desc;
};

struct ParseError final {
    std::vector<std::string_view>
        nodePath;  // parse stack contain node path in config file
    std::string desc;

    enum class Type {
        Unknown,
        FieldNotFound,
        SyntaxError,
        TypeIncorrect,
        Custom,
    } type = Type::Unknown;

    static std::string_view Type2Str(Type type) {
#define CASE(e)   \
    case Type::e: \
        return #e;
        switch (type) {
            CASE(Unknown);
            CASE(FieldNotFound);
            CASE(SyntaxError);
            CASE(TypeIncorrect);
            CASE(Custom);
            default:
                return "Unknwon";
        }
#undef CASE
    }
};

#define MemberAssignmentFunc(classtype, member, instance, value)              \
    [&](auto& data) {                                                         \
        if constexpr (std::is_same_v<                                         \
                          std::remove_reference_t<decltype(data)>::pointer_t, \
                          decltype(&classtype::member)>) {                    \
            if (#member == data.Name()) instance.*(data.Pointer()) = value;   \
        }                                                                     \
    }

//! @addtogroup auto-parser
//! @{
//! @brief a tool to generate parse function by parse ruler

//! @brief start declare a parse ruler
//! @see EndDeclareParseFunc
//! @see Field
//! @see ArrayField
//! @see DynArrayField
//! @see ObjField
//! @param classtype  the serialize class type without any namespace prefix
//!
//! Here is a small example in ./test/config_parse.cpp
//! 
//! This is the class you want to read from file:
//! @snippet ./test/config_parse.cpp TestPOD Define
//! First use dynamic-reflect tool to reflect your class:
//! @snippet ./test/config_parse.cpp Reflect TestPOD
//! Then you can declare parse function as follow:
//! @snippet ./test/config_parse.cpp Declare TestPOD
//! This macro will generate a `std::optional<classtype> Parse[classtype](const sol::table& root)` function,
//! you can pass your lua table to it and get the parsed class instance:
//! @snippet ./test/config_parse.cpp Use AutoParse
#define DeclareParseFunc(classtype)                   \
    std::optional<classtype> Parse ## classtype ## (const sol::table& root) { \
        classtype instance;                                 \
        auto classinfo = refl::GetClass<classtype>();

//! @brief end declare a parse ruler
//! @see EndDeclareParseFunc
#define EndDeclareParseFunc() \
    return instance;          \
    }

//! @brief define a basic field(number, std::string) to be parsed
//! @see ArrayField
//! @see DynArrayField
//! @see ObjField
//! @param name  the serialize class member variable name
//! @param mtype  the serialize class member variable type(must be primitive type or std::string) 
#define Field(name, mtype)                                              \
    {                                                                   \
        auto field = root.get<std::optional<mtype>>(#name);             \
        if (!field.has_value()) return std::nullopt;                    \
        classinfo.VisitMembers(MemberAssignmentFunc(                    \
            decltype(classinfo)::type, name, instance, field.value())); \
    }

//! @brief define a array(std::array) to be parsed
//! @see Field
//! @see DynArrayField
//! @see ObjField
//! @param name  the serialize class member variable name
//! @param mtype  the member array element type
//! @param count  the member array size
#define ArrayField(name, mtype, count)                                         \
    {                                                                          \
        auto field = root.get<std::optional<std::array<mtype, count>>>(#name); \
        if (!field.has_value()) return std::nullopt;                           \
        classinfo.VisitMembers(MemberAssignmentFunc(                           \
            decltype(classinfo)::type, name, instance, field.value()));        \
    }

//! @brief define a dynamic array(std::vector) to be parsed
//! @see Field
//! @see ArrayField
//! @see ObjField
//! @param name  the serialize class member variable name
//! @param mtype  the member array element type
#define DynArrayField(name, mtype)                                       \
    {                                                                    \
        auto field = root.get<std::optional<std::vector<mtype>>>(#name); \
        if (!field.has_value()) return std::nullopt;                     \
        classinfo.VisitMembers(MemberAssignmentFunc(                     \
            decltype(classinfo)::type, name, instance, field.value()));  \
    }

//! @brief define a table array(class type) to be parsed
//! @see Field
//! @see ArrayField
//! @see DynArrayField
//! @param name  the serialize class member variable name
//! @param mtype  the member type(must be class that also declared by `DeclareParseFunc`)
#define ObjField(name, mtype)                                            \
    {                                                                    \
        auto table = root.get<std::optional<sol::table>>(#name);         \
        if (!table.has_value()) return std::nullopt;                     \
        auto member = Parse##mtype##(table.value());                     \
        if (!member.has_value()) return std::nullopt;                    \
        classinfo.VisitMembers(MemberAssignmentFunc(                     \
            decltype(classinfo)::type, name, instance, member.value())); \
    }

//! @}