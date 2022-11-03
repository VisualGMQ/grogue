#pragma once

#include "engine/engine.hpp"

inline engine::Vec2 AlignCenter(const engine::Size& size, const engine::Rect& dst) {
    return dst.position + (dst.size - size) * 0.5;
}

inline engine::Vec2 AlignLeft(const engine::Size& size, const engine::Rect& dst) {
    return dst.position;
}

inline engine::Vec2 AlignRight(const engine::Size& size, const engine::Rect& dst) {
    return engine::Vec2{dst.position.x - size.w, dst.position.y};
}

inline engine::Vec2 AlignBottom(const engine::Size& size, const engine::Rect& dst) {
    return engine::Vec2{dst.position.x, dst.position.y - size.h};
}

inline engine::Vec2 AlignTop(const engine::Size& size, const engine::Rect& dst) {
    return dst.position;
}

inline engine::Vec2 AlignTopLeft(const engine::Size& size, const engine::Rect& dst) {
    return dst.position;
}

inline engine::Vec2 AlignTopRight(const engine::Size& size, const engine::Rect& dst) {
    return engine::Vec2(dst.position.y, dst.position.x - size.w);
}

inline engine::Vec2 AlignBottomLeft(const engine::Size& size, const engine::Rect& dst) {
    return engine::Vec2(dst.position.x, dst.size.h - size.h);
}

inline engine::Vec2 AlignBottomRight(const engine::Size& size, const engine::Rect& dst) {
    return dst.position + dst.size - size;
}

inline bool IsInSection(float x, float y, float left, float right, float top, float bottom) {
    return x >= left && x <= right && y >= bottom && y <= top;
}