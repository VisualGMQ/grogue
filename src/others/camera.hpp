#pragma once

#include "engine/engine.hpp"
#include "others/config.hpp"

class Camera final {
public:
    Camera& operator=(const Camera&) = delete;

    const engine::Vec2& GetPosition() const { return pos_; }

    void Move(const engine::Vec2& offset) { pos_ += offset; }
    void MoveTo(const engine::Vec2& pos) { pos_ = pos; }

private:
    engine::Vec2 pos_;
};