#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "core/memory_manager.hpp"

struct MyStruct {
    MyStruct(int value) : value(value) {}

    int value;
};

using Recycler = MemoryRecycler<MyStruct>;

TEST_CASE("memory recycler") {
    MyStruct* structs[10] = {nullptr};

    for (int i = 0; i < 10; i++) {
        structs[i] = Recycler::Instance().New(i);
    }

    for (int i = 0; i < 10; i++) {
        REQUIRE(structs[i]->value == i);
    }

    REQUIRE(Recycler::Instance().CacheNum() == 0);
    REQUIRE(Recycler::Instance().UsingInstanceNum() == 10);

    Recycler::Instance().Recycle();

    REQUIRE(Recycler::Instance().UsingInstanceNum() == 0);
    REQUIRE(Recycler::Instance().CacheNum() == 10);

    auto a = Recycler::Instance().New(11);
    REQUIRE(a->value == 11);
    REQUIRE(Recycler::Instance().UsingInstanceNum() == 1);
    REQUIRE(Recycler::Instance().CacheNum() == 9);
}

using FrameRecycler = SingleFrameRecycler<MyStruct>;

TEST_CASE("single frame recycler") {
    auto& recycler = FrameRecycler::Instance();
    for (int i = 0; i < 10; i++) {
        recycler.EmplaceBack(i);
        REQUIRE(recycler.Capacity() == i + 1);
    }
    for (int i = 0; i < 10; i++) {
        REQUIRE(recycler[i].value == i);
    }

    REQUIRE(recycler.UsingNum() == 10);

    try {
        for (int i = 0; i < 10; i++) {
            REQUIRE(recycler.At(i).value == i);
        }
    } catch (const std::out_of_range& err) {
        REQUIRE(false);
    }

    recycler.InvalidAll();
    REQUIRE(recycler.Capacity() == 10);

    recycler.EmplaceBack(11);
    REQUIRE(recycler.At(0).value == 11);
}