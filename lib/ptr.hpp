#pragma once

#include <functional>

// IMPROVE: alloc refcount_ and destroyFunc_ from memory pool
template <typename T>
class SharePtr {
public:
    using DestroyFuncType = std::function<void(T*)>;
    using DataType = T*;

    SharePtr(): data_(nullptr), destroyFunc_(nullptr), refcount_(nullptr) {}
    SharePtr(const DataType& data, DestroyFuncType func = nullptr)
        : data_(data), destroyFunc_(nullptr), refcount_(new size_t) {
        (*refcount_) = 1;
        if (func) {
            destroyFunc_ = new DestroyFuncType;
            (*destroyFunc_) = func;
        }
    }

    SharePtr(const SharePtr& other) {
        setup(other.data_, other.destroyFunc_, other.refcount_);
    }

    SharePtr(SharePtr&& other) {
        setup(other.data_, other.destroyFunc_, other.refcount_);
    }

    SharePtr& operator=(const SharePtr& other) {
        if (&other != this) {
            decRefcount();
            if (*refcount_ == 0) {
                giveup(other.refcount_);
                setup(other.data_, other.destroyFunc_);
            }
        }
        return *this;
    }

    SharePtr& operator=(SharePtr&& other) {
        decRefcount();
        if (*refcount_ == 0) {
            giveup(other.refcount_);
            setup(other.data_, other.destroyFunc_);
        }
        return *this;
    }

    T& operator*() const {
        // TODO use debug
        return *data_;
    }

    DataType operator->() const {
        // TODO use debug
        return data_;
    }

    DataType Ptr() const { return data_; }

    void Reset(const DataType& data, DestroyFuncType func = nullptr) {
        giveup();
        setup(data, func);
    }

    ~SharePtr() {
        giveup();
    }

    size_t Refcount() const {
        if (refcount_)
            return *refcount_;
        else
            return 0;
    }

private:
    DataType data_;
    DestroyFuncType* destroyFunc_;
    size_t* refcount_;

    void tryReleaseData() {
        if (destroyFunc_) {
            (*destroyFunc_)(data_);
        }
    }

    void decRefcount() {
        (*refcount_) --;
    }

    void giveup() {
        decRefcount();
        if (Refcount() == 0) {
            delete refcount_;
            tryReleaseData();
            delete destroyFunc_;
        }
    }

    void setup(const DataType& data, DestroyFuncType func, size_t* refcount) {
        if (refcount) {
            refcount_ = refcount;
            (*refcount) ++;
        } else {
            refcount_ = new size_t;
            (*refcount_) = 1;
        }
        data_ = data;
        if (func) {
            destroyFunc_ = new DestroyFuncType;
            (*destroyFunc_) = func;
        } else {
            destroyFunc_ = nullptr;
        }
    }
};

template <typename T, typename... Args>
SharePtr<T*> MakeShared(const Args&& ...args) {
    return SharePtr(new T(std::forward(args...)), nullptr);
}
