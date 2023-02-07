#pragma once

#include <string>
#include "core/singleton.hpp"

#cmakedefine TEST_RESOURCE_DIR "@TEST_RESOURCE_DIR@"

class TestHelper final: public Singleton<TestHelper, false> {
public:
    std::string GetResourcePath() const {
        return TEST_RESOURCE_DIR;
    }
};