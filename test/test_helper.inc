#pragma once

#include <string>
#include "core/singlton.hpp"

#cmakedefine TEST_RESOURCE_DIR "@TEST_RESOURCE_DIR@"

class TestHelper final: public Singlton<TestHelper, false> {
public:
    std::string GetResourcePath() const {
        return TEST_RESOURCE_DIR;
    }
};