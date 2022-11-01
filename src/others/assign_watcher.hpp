#pragam once

#include "engine/engine.hpp"

template <typename T>
class AssignWatcher final {
public:
    AssignWatcher(): value_{}, changed_(false) {}
    explicit AssignWatcher(const T& value): value_(value), changed_(false) {}
    explicit AssignWatcher(T&& value): value_(std::move(value)), changed_(false) {}

    AssignWatcher& operator=(const T& value) {
        value_ = value;
        changed_ = true;
        return *this;
    }

    AssignWatcher& operator=(const AssignWatcher& o) {
        if (&o == this) return *this;
        value_ = o.value_;
        changed_ = true;
        return *this;
    }

    operator T() {
        return value_;
    }

    T* operator->() {
        return &value_;
    }

    void Reset() { 
        changed_ = false;
    }

private:
    T value_;
    bool changed_;
};