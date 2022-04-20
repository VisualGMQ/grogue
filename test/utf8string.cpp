#include "core/utf8string.hpp"
#include "core/log.hpp"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("utf8string check", "[UTF8String]") {
    SECTION("empty UTF8String") {
        grogue::core::UTF8String s;
        REQUIRE(s.Size() == 0);
        REQUIRE(s.Empty());
    }

    SECTION("chinese and english") {
        grogue::core::UTF8String s("测试UTF8String是否有效");
        REQUIRE(s.Size() == 16);
    }

    SECTION("chinese") {
        grogue::core::UTF8String s("纯纯中文");
        REQUIRE(s.Size() == 4);
    }

    SECTION("english") {
        grogue::core::UTF8String s("this is pure english");
        REQUIRE(s.Size() == 20);
    }

    SECTION("check move") {
        grogue::core::UTF8String s = grogue::core::UTF8String("检查move语义");
        REQUIRE(s.Size() == 8);
    }

    SECTION("check copy") {
        grogue::core::UTF8String s1("检查copy语义");
        grogue::core::UTF8String s = s1;
        REQUIRE(s.Size() == 8);
    }
}
