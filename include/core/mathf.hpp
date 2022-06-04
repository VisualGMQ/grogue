#pragma once

#include "pch.hpp"

namespace grogue::core {

template <typename T, size_t N>
struct Vector final { T data[N]; };

/******************
 * Vector2
******************/
template <typename T>
struct Vector<T, 2> {
    union {
        T data[2];
        struct { T x, y; };
        struct { T w, h; };
    };

    Vector(): x(0), y(0) {}

    Vector(T x, T y): x(x), y(y) {}

    explicit Vector(T value): x(value), y(value) {}

    size_t Size() const { return 2; }

    void Set(T x, T y) {
        this->x = x;
        this->y = y;
    }

    template <typename U>
    Vector& operator=(const Vector<U, 2>& o) {
        x = o.x;
        y = o.y;
        return *this;
    }

    template <typename U>
    Vector& operator+=(const Vector<U, 2>& o) {
        x += o.x;
        y += o.y;
        return *this;
    }

    template <typename U>
    Vector& operator-=(const Vector<U, 2>& o) {
        x -= o.x;
        y -= o.y;
        return *this;
    }

    template <typename U>
    Vector& operator*=(const Vector<U, 2>& o) {
        x *= o.x;
        y *= o.y;
        return *this;
    }

    template <typename U>
    bool operator==(const Vector<U, 2>& o) const {
        return x == o.x && y == o.y;
    }

    template <typename U>
    bool operator!=(const Vector<U, 2>& o) const {
        return !(*this == o);
    }

    template <typename U>
    bool Equal(const Vector<U, 2>& o) const {
        auto epsilon = std::numeric_limits<std::common_type_t<T, U>>::epsilon();
        return std::abs(o.x - x) <= epsilon &&
               std::abs(o.y - y) <= epsilon;
    }

    template <typename U>
    Vector& operator/=(const Vector<U, 2>& o) {
        x /= o.x;
        y /= o.y;
        return *this;
    }

    Vector& operator/=(float value) {
        x /= value;
        y /= value;
        return *this;
    }

    float Len2() const {
        return x * x + y * y;
    }

    float Len() const {
        return std::sqrt(Len2());
    }

    T& operator[](size_t idx) {
        return data[idx];
    }
};

template <typename T, typename U, size_t N>
auto operator+(const Vector<T, N>& v1, const Vector<U, N>& v2) {
    Vector<decltype(v1.data[0] + v2.data[0]), N> ret;
    for (size_t i = 0; i < N; i++) {
        ret.data[i] = v1.data[i] + v2.data[i];
    }
    return ret;
}

template <typename T, typename U, size_t N>
auto operator-(const Vector<T, N>& v1, const Vector<U, N>& v2) {
    Vector<decltype(v1.data[0] - v2.data[0]), N> ret;
    for (size_t i = 0; i < N; i++) {
        ret.data[i] = v1.data[i] - v2.data[i];
    }
    return ret;
}

template <typename T, typename U, size_t N>
auto operator*(const Vector<T, N>& v1, const Vector<U, N>& v2) {
    Vector<decltype(v1.data[0] * v2.data[0]), N> ret;
    for (size_t i = 0; i < N; i++) {
        ret.data[i] = v1.data[i] * v2.data[i];
    }
    return ret;
}

template <typename T, typename U, size_t N>
auto operator*(const Vector<T, N>& v, const U& value) {
    Vector<std::common_type_t<T, U>, N> ret;
    for (size_t i = 0; i < N; i++) {
        ret.data[i] = v.data[i] * value;
    }
    return ret;
}

template <typename T, typename U, size_t N>
auto operator*(const U& value, const Vector<T, N>& v) {
    return v * value;
}

template <typename T, typename U, size_t N>
auto operator*(U&& value, const Vector<T, N>& v) {
    return v * value;
}

template <typename T, typename U, size_t N>
auto operator/(const Vector<T, N>& v1, const Vector<U, N>& v2) {
    Vector<decltype(v1.data[0] / v2.data[0]), N> ret;
    for (size_t i = 0; i < N; i++) {
        ret.data[i] = v1.data[i] / v2.data[i];
    }
    return ret;
}

template <typename U, size_t N>
struct IsVector { inline static constexpr bool value = false; };

template <typename U, size_t N>
struct IsVector<Vector<U, N>, N> { inline static constexpr bool value = true; };

template<typename U, size_t N>
inline constexpr bool IsVector_v = IsVector<U, N>::value;

template <typename T, size_t N>
auto operator/(const Vector<T, N>& v,
               float value) {
    Vector<std::common_type_t<T, float>, N> ret;
    for (size_t i = 0; i < N; i++) {
        ret.data[i] = v.data[i] / value;
    }
    return ret;
}

template <typename T, size_t N>
auto Normalize(const Vector<T, N>& v) {
    float len = v.Len();
    return v / (float)len;
}

template <typename T, size_t N>
T Dot(const Vector<T, N>& v1, const Vector<T, N>& v2) {
    T ret{};
    for (size_t i = 0; i < N; i++) {
        ret += v1.data[i] * v2.data[i];
    }
    return ret;
}

template <typename T>
T Cross(const Vector<T, 2>& v1, const Vector<T, 2>& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

using Vec2f = Vector<float, 2>;
using Vec2 = Vector<float, 2>;
using Vec2i = Vector<int, 2>;
using Size = Vector<int, 2>;
using Sizef = Vector<float, 2>;

/*******************
 * Color
*******************/
struct Color final {
    union {
        SDL_Color color;
        struct { uint8_t r, g, b, a; };
    };

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255): r(r), g(g), b(b), a(a) {}
    Color(): r(0), g(0), b(0), a(0) {}
};

/*******************
 * Rect
*******************/
struct Rect final {
    union {
        SDL_FRect sdlrect;
        struct { float x, y, w, h; };
    };

    Rect(): x(0), y(0), w(0), h(0) {}
    Rect(float x, float y, float w, float h): x(x), y(y), w(w), h(h) {}
    Rect(const Vec2& pos, const Size& size): x(pos.x), y(pos.y), w(size.w), h(size.h) {}

    void Reset(float x, float y, float w, float h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
};

struct Recti final {
    union {
        SDL_Rect sdlrect;
        struct { int x, y, w, h; };
    };

    Recti(): x(0), y(0), w(0), h(0) {}
    Recti(int x, int y, int w, int h): x(x), y(y), w(w), h(h) {}

    void Reset(int x, int y, int w, int h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
};

template <typename T, size_t N>
bool IsPointInRect(const Rect& r, const Vector<T, N>& p) {
    return p.x >= r.x && p.x <= r.x + r.w &&
           p.y >= r.y && p.y <= r.y + r.h;
}

template <typename T, size_t N>
bool IsPointInRect(const Recti& r, const Vector<T, N>& p) {
    return p.x >= r.x && p.x <= r.x + r.w &&
           p.y >= r.y && p.y <= r.y + r.h;
}

inline Vec2 AlignLeftPos(const Rect& target, const Rect& r) {
    return Vec2(target.x, r.y);
}

inline Vec2 AlignRightPos(const Rect& target, const Rect& r) {
    return Vec2(target.x + target.w - r.w, r.y);
}

inline Vec2 AlignTopPos(const Rect& target, const Rect& r) {
    return Vec2(r.x, target.y);
}

inline Vec2 AlignBottomPos(const Rect& target, const Rect& r) {
    return Vec2(r.x, target.y + target.h - r.h);
}

inline Vec2 AlignCenterXPos(const Rect& target, const Rect& r) {
    return Vec2(target.x + (target.w - r.w) / 2, r.y);
}

inline Vec2 AlignCenterYPos(const Rect& target, const Rect& r) {
    return Vec2(r.x, target.y + (target.h - r.h) / 2);
}

inline bool IsRectsIntersect(const core::Rect& r1,
                             const core::Rect& r2) {
    return !(r1.x + r1.w <= r2.x || r1.x >= r2.x + r2.w ||
             r1.y + r1.h <= r2.y || r1.y >= r2.y + r2.h);
}

inline core::Rect RectsIntersect(const core::Rect& r1,
                                 const core::Rect& r2) {
    Vec2 lt(std::max(r1.x, r2.x), std::max(r1.y, r2.y)),
         rb(std::min(r1.x + r1.w, r2.x + r2.w), std::min(r1.y + r1.h, r2.y + r2.h));
    return core::Rect(lt.x, lt.y, rb.x - lt.x, rb.y - lt.y);
}

template <typename T>
inline int Sign(T value) {
    return value > 0 ? 1 : (value < 0 ? -1 : 0);
}

#define feq(v1, v2) (std::abs(v1 - v2) <= std::numeric_limits<decltype(v1)>::epsilon())

}
