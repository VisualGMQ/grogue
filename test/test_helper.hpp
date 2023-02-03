#pragma once

#include <string>
#include "core/singlton.hpp"

#define TEST_RESOURCE_DIR "D:/Codes/grogue/test/resources/"

class TestHelper final: public Singlton<TestHelper, false> {
public:
    std::string GetResourcePath() const {
        return TEST_RESOURCE_DIR;
    }
};
