#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "app/lua_manager.hpp"
#include "test_helper.hpp"

TEST_CASE("lua") {
    SECTION("execute lua command") {
        auto script = lua::LuaManager::Instance().CreateSolitary();
        auto result = script.RunCmd("return 123");
        REQUIRE(script.RunCmd("return 123").get<int>() == 123);
        REQUIRE(script.RunCmd("return \"hello\"").get<std::string>() == "hello");
    }

    SECTION("execute lua") {
        auto handle = lua::LuaManager::Instance().Load(TestHelper::Instance().GetResourcePath() + "test.lua");
        REQUIRE(lua::LuaManager::Instance().Get(handle).Execute().get<int>() == 123);
    }
}