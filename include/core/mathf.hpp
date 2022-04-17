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

    Vector(): x{}, y{} {}

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

template <typename T, size_t N, typename U>
auto operator/(const Vector<T, N>& v,
               // std::enable_if_t<!IsVector_v<U, N>, U&&> value) {
               U value) {
    Vector<std::common_type_t<T, U>, N> ret;
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

/*******************
 * Color
*******************/
struct Color final {
    union {
        SDL_Color color;
        struct { uint8_t r, g, b, a; };
    };
};

/*******************
 * Rect
*******************/
struct Rect final {
    union {
        SDL_FRect sdlrect;
        struct { float x, y, w, h; };
        struct {
            Vector<float, 2> position;
            Vector<float, 2> size;
        };
    };

    Rect(): x{}, y{}, w{}, h{} {}
    Rect(float x, float y, float w, float h): x(x), y(y), w(w), h(h) {}
};

}
