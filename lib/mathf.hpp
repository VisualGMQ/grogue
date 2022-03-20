#pragma once

#include "lib/pch.hpp"

template <typename T,
          typename = std::enable_if_t<std::is_same_v<T, int> ||
                                      std::is_same_v<T, float>>>
struct Vector {
    union {
        std::conditional_t<std::is_same_v<T, int>, SDL_Point, SDL_FPoint> point;
        struct {
            T x;
            T y;
        };
        struct {
            T w;
            T h;
        };
    };

    template <typename U>
    Vector& operator=(const Vector<U>& o) {
        if (&o != this) {
            x = o.x;
            y = o.y;
        }
        return *this;
    }
};

template <typename T, typename U>
auto operator+(const Vector<T>& v1, const Vector<U>& v2) {
    return Vector<decltype(v1.x + v2.x)>{v1.x + v2.x, v1.y + v2.y};
}
template <typename T, typename U>
auto operator-(const Vector<T>& v1, const Vector<U>& v2) {
    return Vector<decltype(v1.x - v2.x)>{v1.x - v2.x, v1.y - v2.y};
}
template <typename T, typename U>
auto operator*(const Vector<T>& v1, const Vector<U>& v2) {
    return Vector<decltype(v1.x * v2.x)>{v1.x * v2.x, v1.y * v2.y};
}
template <typename T, typename U>
auto operator/(const Vector<T>& v1, const Vector<U>& v2) {
    return Vector<decltype(v1.x / v2.x)>{v1.x / v2.x, v1.y / v2.y};
}

template <typename T, typename U>
auto& operator+=(Vector<T>& v1, const Vector<U>& v2) {
    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}
template <typename T, typename U>
auto& operator-=(Vector<T>& v1, const Vector<U>& v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
    return v1;
}
template <typename T, typename U>
auto& operator*=(Vector<T>& v1, const Vector<U>& v2) {
    v1.x *= v2.x;
    v1.y *= v2.y;
    return v1;
}
template <typename T, typename U>
auto& operator/=(Vector<T>& v1, const Vector<U>& v2) {
    v1.x /= v2.x;
    v1.y /= v2.y;
    return v1;
}

template <typename T, typename U>
auto operator*(const Vector<T>& v, U value) {
    return Vector<decltype(v.x * value)>{v.x * value, v.y * value};
}
template <typename T, typename U>
auto operator/(const Vector<T>& v, U value) {
    return Vector<decltype(v.x * value)>{v.x * value, v.y * value};
}

template <typename T, typename U>
auto operator*(U value, const Vector<T>& v) {
    return Vector<decltype(v.x * value)>{v.x * value, v.y * value};
}
template <typename T, typename U>
auto operator/(U value, const Vector<T>& v) {
    return Vector<decltype(v.x * value)>{v.x * value, v.y * value};
}

template <typename T, typename U>
auto& operator*=(Vector<T>& v, U value) {
    v.x *= value;
    v.y *= value;
    return v;
}
template <typename T, typename U>
auto& operator/=(Vector<T>& v, U value) {
    v.x /= value;
    v.y /= value;
    return v;
}
template <typename T, typename U>
auto& operator*=(U value, Vector<T>& v) {
    v.x *= value;
    v.y *= value;
    return v;
}
template <typename T, typename U>
auto& operator/=(U value, Vector<T>& v) {
    v.x /= value;
    v.y /= value;
    return v;
}

using Veci = Vector<int>;
using Vecf = Vector<float>;
using Sizei = Vector<int>;
using Sizef = Vector<float>;


template <typename T,
          typename = std::enable_if_t<std::is_same_v<T, int> ||
                                      std::is_same_v<T, float>>>
struct Rect {
    union {
        std::conditional_t<std::is_same_v<T, int>, SDL_Rect, SDL_FRect> rect;
        struct { Vector<T> pos; Vector<T> size; };
        struct { T x, y, w, h; };
    };
};

template <typename T, typename U>
bool IsRectsIntersect(const Rect<T>& r1, const Rect<U>& r2) {
    return std::min(r1.x + r1.w, r2.x + r2.w) >= std::max(r1.x, r2.x) ||
           std::min(r1.y + r1.h, r2.y + r2.h) >= std::max(r1.y, r2.y);
}
