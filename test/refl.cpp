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

class Visitor {
public:
    Visitor(SimplePOD& pod): pod_(pod) {}

    template <typename T>
    void operator()(T&) {}

private:
    SimplePOD& pod_;
};


template <>
void Visitor::operator()<refl::MemberInfo<std::string, decltype(&SimplePOD::svalue)>>(refl::MemberInfo<std::string, decltype(&SimplePOD::svalue)>& info) {
    if (info.Name() == "svalue") {
        pod_.svalue = "anothername";
    }
}

class CheckVisitor {
public:
    CheckVisitor(SimplePOD& pod): pod_(pod) {}

    template <typename T>
    void operator()(T&) {}

private:
    SimplePOD& pod_;
};

template <>
void CheckVisitor::operator()<refl::MemberInfo<std::string, decltype(&SimplePOD::svalue)>>(refl::MemberInfo<std::string, decltype(&SimplePOD::svalue)>& info) {
    if (info.Name() == "svalue") {
        assert(pod_.svalue == "anothername");
    }
}

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


    constexpr auto info = refl::GetClass<SimplePOD>();
    REQUIRE(info.Name() == "SimplePOD");
    REQUIRE(info.HasMember("inner"));
    REQUIRE(info.HasMember("svalue"));
    REQUIRE_FALSE(info.HasMember("outer"));

    Visitor visitor{pod};
    info.VisitMembers(visitor);

    CheckVisitor checkvisitor{pod};
    info.VisitMembers(checkvisitor);

    REQUIRE(std::is_same_v<decltype(info)::type, SimplePOD>);
}