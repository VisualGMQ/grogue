#pragma once

#include "core/math.hpp"

enum Flip {
    None = 0,
    Horizontal = 0x01,
    Vertical = 0x02,
    Both = Vertical | Horizontal,
};

class Transform final {
public:
    Transform& Resize(const math::Vector2& size) {
        size_ = size;
        return *this;
    }

    Transform& Scale(const math::Vector2& scale) {
        size_ *= scale;
        return *this;
    }

    Transform& SetPos(const math::Vector2& pos) {
        pos_ = pos;
        return *this;
    }

    Transform& Move(const math::Vector2& offset) {
        pos_ += offset;
        return *this;
    }

    auto& GetPos() const { return pos_; }

    auto& GetSize() const { return size_; }

    Transform& SetRotation(float radians) {
        rotation_ = radians;
        return *this;
    }

    Transform& SetRotateAnchor(const math::Vector2& anchor) {
        rotateCenter_ = anchor;
        return *this;
    }

    float GetRotation() const { return rotation_; }

    auto& GetRotateAnchor() const { return rotateCenter_; }

    Transform& SetFlip(Flip flip) {
        flip_ = flip;
        return *this;
    }

    Flip GetFlip() const { return flip_; }

private:
    math::Vector2 pos_{math::Vector2::Zero};
    math::Vector2 rotateCenter_ {0.5, 0.5};
    float rotation_ = 0;  // in radians
    math::Vector2 size_ {-1, -1};
    Flip flip_ = None;
};