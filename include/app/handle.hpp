#pragma once

#include <stack>

using HandleInnerDataType = uint32_t;
constexpr uint32_t ValidBitMask = 1 << 31;

template <typename Tag>
class HandleIDGenerator {
public:
    static HandleIDGenerator& Instance() {
        static HandleIDGenerator instance;
        return instance;
    }

    HandleInnerDataType Generate() {
        auto id = curID_;
        assert(curID_ != ValidBitMask - 1);
        curID_++;
        id |= ValidBitMask;
        return id;
    }

private:
    HandleInnerDataType curID_ = 0;
};

template <typename Tag>
class Handle {
public:
    static Handle Create() {
        return Handle{HandleIDGenerator<Tag>::Instance().Generate()};
    }

    void Close() {
        handle_ &= ValidBitMask - 1;
    }

    bool IsValid() const {
        return handle_ & ValidBitMask;
    }

    operator bool() const {
        return IsValid();
    }

private:
    DataType handle_;

    DataType(HandleInnerDataType handle) : handle_(handle) {}
};