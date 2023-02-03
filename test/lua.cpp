#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "app/lua_manager.hpp"

TEST_CASE("lua") {
    SECTION("execute lua command") {
        auto script = lua::LuaManager::Instance().CreateSolitary();
        auto result = script.RunCmd("return 123");
        REQUIRE(script.RunCmd("return 123").get<int>() == 123);
        REQUIRE(script.RunCmd("return \"hello\"").get<std::string>() == "hello");
    }

    SECTION("lua handler") {
        auto handle = lua::LuaManager::Instance().Create();
        
    }
}