#pragma once

#include <stack>
#include <unordered_map>
#include "core/sparse_sets.hpp"
#include "core/singleton.hpp"

using HandleInnerIDType = uint32_t;

template <typename Tag>
class HandleIDGenerator final {
public:
    HandleInnerIDType Generate() {
        auto id = curID_;
        assert(curID_ != 0);
        curID_++;
        return id;
    }

private:
    HandleInnerIDType curID_ = 1;
};

template <typename HandleIDGeneratorType>
class HandleIDManagerBase final : public Singleton<HandleIDManagerBase<HandleIDGeneratorType>, false> {
public:
    auto Generate() {
        auto id = generator_.Generate();
        sparseSet_.Add(id);
        return id;
    }

    bool Has(HandleInnerIDType id) const {
        return sparseSet_.Contain(id);
    }

    void Remove(HandleInnerIDType id) {
        sparseSet_.Remove(id);
    }

private:
    SparseSets<HandleInnerIDType, 128> sparseSet_;
    HandleIDGeneratorType generator_;
};

template <typename T>
using HandleIDManager = HandleIDManagerBase<HandleIDGenerator<T>>;

template <typename Tag>
class Handle {
public:
    struct Hash final {
        size_t operator()(const HandleInnerIDType& k) const {
            return std::hash<HandleInnerIDType>{}(k);
        }
    };

    struct HashEq final {
        size_t operator()(const Handle& h1, const Handle& h2) const {
            return h1 == h2;
        }
    };

    static Handle Null() {
        return Handle{0};
    }

    static Handle Create() {
        return Handle{HandleIDManager<Tag>::Instance().Generate()};
    }

    static void Destroy(Handle handle) {
        HandleIDManager<Tag>::Instance().Remove(handle.handle_);
    }

    void Close() {
        HandleIDManager<Tag>::Instance().Remove(handle_);
    }

    bool IsValid() const {
        return HandleIDManager<Tag>::Instance().Has(handle_);
    }

    operator bool() const {
        return IsValid();
    }

    bool operator==(const Handle& o) const {
        return handle_ == o.handle_;
    }

    bool operator!=(const Handle& o) const {
        return !(o == *this);
    }

    Handle& operator=(const Handle& o) = default;

private:
    HandleInnerIDType handle_ = Null();

    Handle(HandleInnerIDType handle) : handle_(handle) {}
};