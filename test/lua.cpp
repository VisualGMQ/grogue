#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "app/lua_manager.hpp"
#include "test_helper.hpp"

#include <iostream>

TEST_CASE("lua") {
    auto manager = lua::LuaManager();

    SECTION("execute lua command") {
        auto script = manager.CreateSolitary();
        REQUIRE(script.lua.script("return 123").get<int>() == 123);
        REQUIRE(script.lua.script("return \"hello\"").get<std::string>() ==
                "hello");
    }

    SECTION("execute lua") {
        auto handle = manager.Load(
            TestHelper::Instance().GetResourcePath() + "test.lua");
        auto& script = manager.Get(handle);
        REQUIRE(script.lua.get<int>("GlobalNumber") == 123);
        auto table = script.lua.get<sol::table>("GlobalTable");
        REQUIRE(table.get<int>("Number") == 1);
        REQUIRE(table.get<std::string>("String") == "123");
        REQUIRE(table.get<bool>("Boolean") == false);
        auto func = script.lua.get<sol::function>("GlobalFunction");
        int result = func();
        REQUIRE(result == 123);
    }
}