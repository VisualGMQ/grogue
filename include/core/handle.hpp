#pragma once
#include "sington.hpp"
#include "assert.hpp"

namespace grogue::core {

template <typename T>
class HandleMgr;

template <typename T>
class Handle final {
public:
    friend class HandleMgr<T>;

    Handle(std::uint16_t index) {
        Recreate(index);
    }

    void Recreate(std::uint16_t index) {
        static uint16_t magicNumIter = 1;
        GROGUE_ASSERT(!IsNull(), "don't allow recreate on an valid handle");
        GROGUE_ASSERT(magicNumIter < MaxMagic, "magic number out of use");
        GROGUE_ASSERT(index < MaxIndex, "index out of uint16_t");

        if (!IsNull()) {
            handle_ = 0;
        } else {
            index_ = index;
            magic_ = magicNumIter++;
        }
    }

    std::uint16_t GetHandle() const { return handle_; }
    std::uint16_t GetIndex() const { return index_; }
    std::uint16_t GetMagic() const { return magic_; }

    bool IsNull() const {
        return handle_ != 0;
    }

    bool operator==(const Handle& other) const {
        return handle_ == other.handle_;
    }

    bool operator!=(const Handle& other) const {
        return !(*this == other);
    }

    operator bool() const {
        return !IsNull();
    }

private:
    union {
        struct {
            std::uint16_t index_;
            std::uint16_t magic_;
        };
        std::uint32_t handle_;
    };

    static constexpr uint16_t MaxMagic = (1 << 16) - 1;
    static constexpr std::uint16_t MaxIndex = (1 << 16) - 1;
};

template <typename T>
class HandleMgr final {
public:
    Handle<T> Create(std::uint16_t index) {
        return Handle<T>(index);
    }

    void Destroy(Handle<T>& handle) {
        handle.handle = 0;
    }

    Handle<T> GetNull() const {
        return _nullHandle;
    }

private:
    static Handle<T> _nullHandle;
};

template <typename T>
Handle<T> HandleMgr<T>::_nullHandle(0, 0);

}
