#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "core/math.hpp"

TEST_CASE("Vector3 for vector 3D representation and calculate") {
    math::Vector3 v(1, 1, 1);
    math::Vector3 a(v);
    math::Vector3 b = -v;
    math::Vector3 c(2, 2, 2);
    math::Vector3 d(1, 0, 0);
    math::Vector3 e(0, 1, -1);
    math::Vector3 f(0, -1, 1);

    SECTION("basic arithmetic") {
        REQUIRE(a != math::Vector3::Zero);
        a = a + b;
        REQUIRE(a == math::Vector3::Zero);
        b = math::Vector3::Zero;
        REQUIRE(b == math::Vector3::Zero);
        a = b = v;
        a = a - b;
        REQUIRE(a == math::Vector3::Zero);
        a = b = v;
        a += v;
        b *= 2;
        REQUIRE(a == b);
        a -= v;
        REQUIRE(a == v);
        a = a * 2;
        REQUIRE(a == c);
        REQUIRE(b == c);
        b /= 2;
        REQUIRE(b == v);
        b = 2 * b;
        REQUIRE(a == c);
        REQUIRE(b == c);
        REQUIRE(Dot(a, b) == 12);
    }

    SECTION("utility functions") {
        REQUIRE(math::Distance(math::Vector3::Zero, d) == 1);
        REQUIRE(math::Cross(v, d) == e);
        REQUIRE(math::Cross(d, v) == f);
        REQUIRE(math::LengthSquare(c) == 12);
        REQUIRE(math::Length(c) == sqrtf(12));
    }
}

TEST_CASE("Vector2 for vector 2D representation and calculate") {
    math::Vector2 v(1, 1);
    math::Vector2 a(v);
    math::Vector2 b = -v;
    math::Vector2 c(2, 2);
    math::Vector2 d(1, 0);
    math::Vector2 e(0, 1);

    SECTION("basic arithmetic") {
        REQUIRE(a != math::Vector2::Zero);
        a = a + b;
        REQUIRE(a == math::Vector2::Zero);
        b = math::Vector2::Zero;
        REQUIRE(b == math::Vector2::Zero);
        a = b = v;
        a = a - b;
        REQUIRE(a == math::Vector2::Zero);
        a = b = v;
        a += v;
        b *= 2;
        REQUIRE(a == b);
        a -= v;
        REQUIRE(a == v);
        a = a * 2;
        REQUIRE(a == c);
        REQUIRE(b == c);
        b /= 2;
        REQUIRE(b == v);
        b = 2 * b;
        REQUIRE(a == c);
        REQUIRE(b == c);
        REQUIRE(Dot(a, b) == 8);
    }

    SECTION("utility functions") {
        REQUIRE(math::Distance(math::Vector2::Zero, d) == 1);
        REQUIRE(math::Cross(v, d) == -1);
        REQUIRE(math::Cross(d, v) == 1);
        REQUIRE(math::LengthSquare(c) == 8);
        REQUIRE(math::Length(c) == sqrtf(8));
    }
}
