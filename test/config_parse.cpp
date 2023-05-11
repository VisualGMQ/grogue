#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "test_helper.hpp"
#include "core/pch.hpp"
#include "app/config_parse.hpp"

// config file member name must as same as struct member name

struct InnerPOD final {
    float fvalue;
    int ivalue;
};

struct TestPOD final {
    std::string svalue;
    float rvalue;
    int ivalue;
    int aivalue;
    std::array<int, 4> arrvalue;
    std::vector<int> arrvalue2;
    InnerPOD inner;
};

std::string TestLuaContent = R"(
Config = {
    svalue = "hello",
    rvalue = 123.3,
    ivalue = 223,
    aivalue = 345,
    arrvalue = {1, 2, 3, 4},
    arrvalue2 = {2, 3, 4},

    inner = {
        fvalue = 334.123,
        ivalue = 7,
    },
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
        .Member(&TestPOD::aivalue, "aivalue")
        .Member(&TestPOD::arrvalue, "arrvalue")
        .Member(&TestPOD::arrvalue2, "arrvalue2")
        .Member(&TestPOD::inner, "inner")
)

ReflRegist(
    refl::Class<InnerPOD>("InnerPOD")
        .Member(&InnerPOD::fvalue, "fvalue")
        .Member(&InnerPOD::ivalue, "ivalue")
)

// declare parse function
// Field don't require declare order

DeclareParseFunc(InnerPOD)
    Field(fvalue, float)
    Field(ivalue, int)
EndDeclareParseFunc()

DeclareParseFunc(TestPOD)
    Field(svalue, std::string)
    Field(rvalue, float)
    Field(ivalue, int)
    Field(aivalue, int)
    ArrayField(arrvalue, int, 4)
    DynArrayField(arrvalue2, int)
    ObjField(inner, InnerPOD)
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
        REQUIRE(pod.aivalue == 345);
        REQUIRE(pod.arrvalue[0] == 1);
        REQUIRE(pod.arrvalue[1] == 2);
        REQUIRE(pod.arrvalue[2] == 3);
        REQUIRE(pod.arrvalue[3] == 4);
        REQUIRE(pod.arrvalue2.size() == 3);
        REQUIRE(pod.arrvalue2[0] == 2);
        REQUIRE(pod.arrvalue2[1] == 3);
        REQUIRE(pod.arrvalue2[2] == 4);
        REQUIRE(pod.inner.fvalue == 334.123f);
        REQUIRE(pod.inner.ivalue == 7);
    }

    SECTION("test lack field content") {
        auto lua = manager.CreateSolitaryFromContent(TestLuaContentLack);
        std::optional<sol::table> table = lua.lua["Config"];
        REQUIRE(table.has_value());
        auto result = ParseTestPOD(table.value());
        REQUIRE_FALSE(result.has_value());
    }
}
