#pragma once

#include "pch.hpp"

namespace grogue::core {

template <typename T, size_t N>
struct Vector {
    T data[N];

    template <typename... Args>
    Vector(Args&&... args) {
        Set(std::forward(args...));
    }

    Vector(T value) {
        for (auto& elem : data) {
            data = value;
        }
    }

    size_t Size() const { return N; }

    template <typename... Args>
    void Set(Args&&... args) {
        Set(0, args...);
    }

    template <typename U, typename... Args>
    void Set(size_t index, U&& value, Args&&... args) {
        data[index] = std::forward(value);
        if (index < N) {
            Set(index + 1, args...);
        }
    }

    template <typename U>
    Vector& operator=(const Vector<U, N>& o) {
        for (size_t i = 0; i < N; i++) {
            data[i] = static_cast<T>(o.data[i]);
        }
    }

    template <typename U>
    Vector& operator+=(const Vector<U, N>& o) {
        for (size_t i = 0; i < N; i++) {
            data[i] = static_cast<T>(data[i] / o.data[i]);
        }
        return *this;
    }

    template <typename U>
    Vector& operator-=(const Vector<U, N>& o) {
        for (size_t i = 0; i < N; i++) {
            data[i] = static_cast<T>(data[i] / o.data[i]);
        }
        return *this;
    }

    template <typename U>
    Vector& operator*=(const Vector<U, N>& o) {
        for (size_t i = 0; i < N; i++) {
            data[i] = static_cast<T>(data[i] / o.data[i]);
        }
        return *this;
    }

    template <typename U>
    Vector& operator/=(const Vector<U, N>& o) {
        for (size_t i = 0; i < N; i++) {
            data[i] = static_cast<T>(data[i] / o.data[i]);
        }
        return *this;
    }

    float Len2() const {
        return std::accumulate(std::begin(data),
                               std::end(data),
                               0,
                               [](T a, T b){
                                   return std::move(a) + b * b;
                               });
    }

    float Len() const {
        return std::sqrt(Len2());
    }

private:
    void Set(size_t index) {}
};

template <typename T, typename U, size_t N>
auto operator+(const Vector<T, N>& v1, const Vector<U, N>& v2) {
    Vector<decltype(v1.x + v2.x), N> ret;
    for (size_t i = 0; i < N; i++) {
        ret.data[i] = v1.data[i] + v2.data[i];
    }
    return ret;
}

template <typename T, typename U, size_t N>
Vector<T, N> operator-(const Vector<T, N>& v1, const Vector<U, N>& v2) {
    Vector<decltype(v1.x - v2.x), N> ret;
    for (size_t i = 0; i < N; i++) {
        ret.data[i] = v1.data[i] - v2.data[i];
    }
    return ret;
}

template <typename T, typename U, size_t N>
Vector<T, N> operator*(const Vector<T, N>& v1, const Vector<U, N>& v2) {
    Vector<decltype(v1.x * v2.x), N> ret;
    for (size_t i = 0; i < N; i++) {
        ret.data[i] = v1.data[i] * v2.data[i];
    }
    return ret;
}

template <typename T, typename U, size_t N>
Vector<T, N> operator/(const Vector<T, N>& v1, const Vector<U, N>& v2) {
    Vector<decltype(v1.x / v2.x), N> ret;
    for (size_t i = 0; i < N; i++) {
        ret.data[i] = v1.data[i] / v2.data[i];
    }
    return ret;
}

template <typename T, size_t N, typename U>
auto operator*(const Vector<T, N>& v, U&& value) {
    Vector<decltype(v.x * value), N> ret;
    for (size_t i = 0; i < N; i++) {
        ret.data[i] = v.data[i] * value;
    }
    return ret;
}

template <typename T, size_t N, typename U>
auto operator*(U&& value, const Vector<T, N>& v) {
    return v * value;
}

template <typename T, size_t N, typename U>
auto operator/(const Vector<T, N>& v, U&& value) {
    Vector<decltype(v.x / value), N> ret;
    for (size_t i = 0; i < N; i++) {
        ret.data[i] = v.data[i] / value;
    }
    return ret;
}


/*******************
 * Vector2
*******************/
template <typename T>
struct Vector<T, 2> {
    union {
        std::conditional_t<std::is_same_v<T, float>,
                           SDL_FPoint,
                           std::conditional_t<std::is_same_v<T, int>,
                                              SDL_Point,
                                              void>> point;
        struct { T x, y; };
    };

    // TODO other implement
};


template <typename T, size_t N>
auto Normalize(const Vector<T, N>& v) {
    float len = v.Len();
    return v / (float)len;
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
};

}
