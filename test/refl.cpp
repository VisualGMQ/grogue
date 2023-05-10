#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <array>
#include <type_traits>
#include <utility>

#include "core/refl.hpp"

struct InnerPOD {
    float innerF;
    std::vector<float> innerFV;
};

struct SimplePOD {
    std::string svalue;
    float fvalue;
    int ivalue;
    double dvalue;

    InnerPOD inner;
    std::array<int, 3> iarray;
};

struct AnotherPOD {
    float fvalue;
};

ReflRegist(refl::Class<InnerPOD>("InnerPOD")
    .Member(&InnerPOD::innerF, "innerF")
    .Member(&InnerPOD::innerFV, "innerFV"));

ReflRegist(refl::Class<SimplePOD>("SimplePOD")
    .Member(&SimplePOD::svalue, "svalue")
    .Member(&SimplePOD::fvalue, "fvalue")
    .Member(&SimplePOD::ivalue, "ivalue")
    .Member(&SimplePOD::dvalue, "dvalue")
    .Member(&SimplePOD::inner, "inner")
    .Member(&SimplePOD::iarray, "iarray"));

TEST_CASE("refl") {
    SimplePOD pod = {
        "pod",
        123.2,
        124,
        224.029,

        InnerPOD {
            3.1415,
            {12, 1.3, 1.4, 1.5},
        },
        {1, 2, 3},
    };

    auto info = refl::GetClass<SimplePOD>();
    REQUIRE(info.Name() == "SimplePOD");
    // auto svalue = info.GetMember("svalue");
    REQUIRE(std::is_same_v<decltype(info)::type, SimplePOD>);
    // REQUIRE(std::is_same_v(decltype(svalue)::type, std::string));
    // REQUIRE(svalue.Apply(info) == "pod");
}