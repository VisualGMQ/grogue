#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "test_helper.hpp"
#include "core/pch.hpp"
#include "app/config_parse.hpp"

// config file member name must as same as struct member name

struct TestPOD final {
    std::string svalue;
    float rvalue;
    int ivalue;
};

std::string TestLuaContent = R"(
Config = {
    svalue = "hello",
    rvalue = 123.3,
    ivalue = 223,
}
)";

std::string TestLuaContentLack = R"(
Config = {
    svalue = "hello",
    ivalue = 223,
}
)";

// reflect Test POD. You must reflect the struct you want to parse
ReflRegist(
    refl::Class<TestPOD>("TestPOD")
        .Member(&TestPOD::svalue, "svalue")
        .Member(&TestPOD::rvalue, "rvalue")
        .Member(&TestPOD::ivalue, "ivalue")
)


// declare parse function
// Field don't require declare order

DeclareParseFunc(ParseTestPOD, TestPOD)
    Field(svalue, std::string)
    Field(rvalue, float)
    Field(ivalue, int)
EndDeclareParseFunc()

TEST_CASE("config_parse") {
    LuaManager manager;

    SECTION("test complete content") {
        auto lua = manager.CreateSolitaryFromContent(TestLuaContent);
        std::optional<sol::table> table = lua.lua["Config"];
        REQUIRE(table.has_value());

        auto result = ParseTestPOD(table.value());
        REQUIRE(result.has_value());
        auto& pod = result.value();
        REQUIRE(pod.svalue == "hello");
        REQUIRE(pod.rvalue == 123.3f);
        REQUIRE(pod.ivalue == 223);
    }

    SECTION("test lack field content") {
        auto lua = manager.CreateSolitaryFromContent(TestLuaContentLack);
        std::optional<sol::table> table = lua.lua["Config"];
        REQUIRE(table.has_value());
        auto result = ParseTestPOD(table.value());
        REQUIRE_FALSE(result.has_value());
    }
}