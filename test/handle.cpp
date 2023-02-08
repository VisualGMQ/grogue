#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "app/handle.hpp"

using TestHandle = Handle<int>;

TEST_CASE("handle test") {
    TestHandle h1 = TestHandle::Create();

    REQUIRE(h1);

    SECTION("handle can copy cheaply") {
        TestHandle h2 = h1;
        REQUIRE(h2);
    }

    SECTION("close handle will invalid all same handle") {
        TestHandle h2 = h1;
        REQUIRE(h2);
        REQUIRE(h1);

        TestHandle::Destroy(h1);
        REQUIRE_FALSE(h1);
        REQUIRE_FALSE(h2);
    }
}
