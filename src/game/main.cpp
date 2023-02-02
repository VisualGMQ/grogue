#include <iostream>
#include "core/assert.hpp"

int main() {
    std::cout << "hello world" << std::endl;
    Assert(true, "123");
    return 0;
}
