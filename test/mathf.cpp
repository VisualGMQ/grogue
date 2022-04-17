#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "core/mathf.hpp"

TEST_CASE("Vector compute", "[Vector]") {
    grogue::core::Vector<float, 2> v1(1.2, 2.3),
                                   v2(2.2, 3.6);

    SECTION("check member") {
        REQUIRE(v1.x == 1.2f);
        REQUIRE(v1.y == 2.3f);

        v1.Set(2.2, 4);
        REQUIRE(v1.x == 2.2f);
        REQUIRE(v1.y == 4.f);
    }

    SECTION("check operators") {
        auto v = v1 + v2;
        
        REQUIRE(v.x == v1.x + v2.x);
        REQUIRE(v.y == v1.y + v2.y);

        v = v1 - v2;
        REQUIRE(v.x == v1.x - v2.x);
        REQUIRE(v.y == v1.y - v2.y);

        v = v1 * v2;
        REQUIRE(v.x == v1.x * v2.x);
        REQUIRE(v.y == v1.y * v2.y);

        v = v1 / v2;
        REQUIRE(v.x == v1.x / v2.x);
        REQUIRE(v.y == v1.y / v2.y);

        v = v1 / 3.2f;
        REQUIRE(v.x == v1.x / 3.2f);
        REQUIRE(v.y == v1.y / 3.2f);

        v = v1 * 4.5f;
        REQUIRE(v.x == v1.x * 4.5f);
        REQUIRE(v.y == v1.y * 4.5f);

        v = 4.5f * v1;
        REQUIRE(v.x == v1.x * 4.5f);
        REQUIRE(v.y == v1.y * 4.5f);

        REQUIRE(v.Len2() == v.x * v.x + v.y * v.y);
        REQUIRE(v.Len() == std::sqrt(v.x * v.x + v.y * v.y));

        REQUIRE(grogue::core::Dot(v1, v2) == v1.x * v2.x + v1.y * v2.y);
        REQUIRE(grogue::core::Cross(v1, v2) == v1.x * v2.y - v1.y * v2.x);

        auto n = grogue::core::Normalize(v1);
        REQUIRE(n == grogue::core::Normalize(v1));
    }
}


