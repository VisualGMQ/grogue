#pragma once

#include "engine/core/log.hpp"

#include <algorithm>
#include <iostream>
#include <cmath>

namespace engine {

class Vec2 final {
public:
    union {
        struct { float x, y; };
        struct { float w, h; };
        SDL_FPoint sdl_point;
    };

    Vec2(): x(0), y(0) {}
    Vec2(float x, float y): x(x), y(y) {}

    void Set(float x, float y) {
        this->x = x;
        this->y = y;
    }

    Vec2& operator+=(const Vec2& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vec2& operator*=(const Vec2& rhs) {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    Vec2& operator/=(const Vec2& rhs) {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }

    Vec2& operator*=(float rhs) {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    Vec2& operator/=(float rhs) {
        x /= rhs;
        y /= rhs;
        return *this;
    }
};

inline Vec2 operator*(float lhs, const Vec2& rhs) {
    return Vec2{lhs * rhs.x, lhs * rhs.y};
}

inline Vec2 operator*(const Vec2& rhs, float lhs) {
    return lhs * rhs;
}

inline Vec2 operator*(const Vec2& lhs, const Vec2& rhs) {
    return Vec2{lhs.x * rhs.x, lhs.y * rhs.y};
}

inline Vec2 operator/(const Vec2& lhs, const Vec2& rhs) {
    return Vec2{lhs.x / rhs.x, lhs.y / rhs.y};
}

inline Vec2 operator/(const Vec2& lhs, float rhs) {
    return Vec2{lhs.x / rhs, lhs.y / rhs};
}

inline Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
    return Vec2{lhs.x + rhs.x, lhs.y + rhs.y};
}

inline Vec2 operator-(const Vec2& lhs, const Vec2& rhs) {
    return Vec2{lhs.x - rhs.x, lhs.y - rhs.y};
}

inline bool operator==(const Vec2& lhs, const Vec2& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(const Vec2& lhs, const Vec2& rhs) {
    return !(lhs == rhs);
}

inline std::ostream& operator<<(std::ostream& o, const Vec2& p) {
    o << "Vec2(" << p.x << ", " << p.y << ")";
    return o;
}

inline float Dot(const Vec2& lhs, const Vec2& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

inline float Cross(const Vec2& lhs, const Vec2& rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

inline float Distance(const Vec2& v1, const Vec2& v2) {
    Vec2 dir = v1 - v2;
    return std::sqrt(dir.x * dir.x + dir.y * dir.y);
}

inline float Length2(const Vec2& v) {
    return Dot(v, v);
}

inline float Length(const Vec2& v) {
    return std::sqrt(Length2(v));
}

inline auto Normalize(const Vec2& v) {
    auto len = Length(v);
    if (len == 0) return v;
    return v / Length(v);
}

using Size = Vec2;

struct Rect {
    union {
        struct { Vec2 position, size; };
        SDL_FRect sdl_rect;
    };

    Rect(): position(0, 0), size(0, 0) {}
    Rect(float x, float y, float w, float h): position(x, y), size(w, h) {}
    Rect(const Vec2& pos, const Vec2& size) : position(pos), size(size) {}

    void Set(float x, float y, float w, float h) {
        position.Set(x, y);
        size.Set(w, h);
    }

    void Set(const Vec2& position, const Vec2& size) {
        this->position = position;
        this->size = size;
    }
};

inline std::ostream& operator<<(std::ostream& o, const Rect& rect) {
    o << "Rect(" << rect.position << ", " << rect.size << ")";
    return o;
}

constexpr float PI = 3.14159265358;

inline float Radians(float degrees) {
    return degrees * PI / 180.0;
}

inline float Degrees(float radians) {
    return radians * 180.0 / PI;
}

template <typename T>
inline T Clamp(const T& value, const T& low, const T& high) {
    return value > high ? high : (value < low ? low : value);
}

template <typename T, typename RetT>
RetT Lerp(const T& a, const T& b, float s) {
    return a + (b - a) * s;
}

template <typename T>
int Sign(const T& value) {
    return value > 0 ? 1 : (value < 0 ? -1 : 0);
}

struct Color {
    union {
        struct{ uint8_t r, g, b, a; };
        SDL_Color sdl_color;
    };

    Color(int r, int g, int b, int a = 255): r(r), g(g), b(b), a(a) {}
    Color(): r(0), g(0), b(0), a(255) {}

    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
};

}
