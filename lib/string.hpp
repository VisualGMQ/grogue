#pragma once

#include <cstring>
#include "ptr.hpp"

class String {
public:
    String();
    template <size_t N>
    String(const char chars[N]);
    ~String();

    inline size_t Size() const { return size_; }

private:
    bool constStr_;
    const char* cdata_;
    SharePtr<char> data_;
    size_t size_;
};
