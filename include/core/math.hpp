#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>

#define GROGUE_MATH_FLOAT_COMPARE_USE_EPSILON

namespace math {

constexpr double PI = 3.1415926535897932384626;

inline bool FloatEq(float value1, float value2) {
#ifdef GROGUE_MATH_FLOAT_COMPARE_USE_EPSILON
    return std::abs(value1 - value2) <= std::numeric_limits<float>::epsilon();
#else
    return value1 == value2;
#endif
}

// forward declarations
class Vector3;
class Vector2;
float Length(const Vector3& v);
float Length(const Vector2& v);

inline float Rad2Deg(float radians) {
    return radians * 180.0 / PI;
}

inline float Deg2Rad(float degree) {
    return degree * PI / 180.0;
}

class Vector3 {
public:
    float x{}, y{}, z{};
    static const Vector3 Zero;
    static const Vector3 XAxis;
    static const Vector3 YAxis;
    static const Vector3 ZAxis;

    Vector3() {}

    Vector3(float vx, float vy, float vz) : x(vx), y(vy), z(vz) {}

    ~Vector3() {}

    bool operator==(const Vector3& v) const {
        return FloatEq(x, v.x) && FloatEq(y, v.y) && FloatEq(z, v.z);
    }

    bool operator!=(const Vector3& v) const { return !(*this == v); }

    Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }  // Vector negation

    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3 operator*(float k) const { return Vector3(k * x, k * y, k * z); }

    Vector3 operator/(float k) const {
        assert(std::numeric_limits<float>::epsilon() < std::abs(k));
        float a = 1.0f / k;
        return Vector3(a * x, a * y, a * z);
    }

    Vector3& operator+=(const Vector3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3& operator-=(const Vector3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3& operator*=(float k) {
        x *= k;
        y *= k;
        z *= k;
        return *this;
    }

    Vector3& operator/=(float k) {
        assert(!FloatEq(k, 0.0f));
        float a = 1.0f / k;
        x *= a;
        y *= a;
        z *= a;
        return *this;
    }

    void Set(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector3 operator*(const Vector3& v) { return {v.x * x, v.y * y, v.z * z}; }

    void Normalize() {
        float a = 1.0f / Length(*this);
        *this *= a;
    }
};

inline Vector3 operator*(float k, const Vector3& v) {
    return Vector3(k * v.x, k * v.y, k * v.z);
}

inline std::ostream& operator<<(std::ostream& stream, const Vector3& v) {
    stream << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return stream;
}

inline Vector3 Cross(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                   v1.x * v2.y - v1.y * v2.x);
}

inline float Dot(const Vector3& v1, const Vector3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float Distance(const Vector3& v1, const Vector3& v2) {
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    float dz = v1.z - v2.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

inline float LengthSquare(const Vector3& v) {
    return Dot(v, v);
}

inline float Length(const Vector3& v) {
    return std::sqrt(LengthSquare(v));
}

class Vector2 {
public:
    static const Vector2 Zero;
    static const Vector2 XAxis;
    static const Vector2 YAxis;

    float x{}, y{};

    Vector2() {}

    Vector2(float vx, float vy) : x(vx), y(vy) {}

    ~Vector2() {}

    bool operator==(const Vector2& v) const {
        return FloatEq(x, v.x) && FloatEq(y, v.y);
    }

    bool operator!=(const Vector2& v) const { return !(*this == v); }

    Vector2 operator-() const { return Vector2(-x, -y); }  // Vector negation

    Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }

    Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }

    Vector2 operator*(float k) const { return Vector2(k * x, k * y); }

    Vector2 operator/(float k) const {
        assert(std::numeric_limits<float>::epsilon() < std::abs(k));
        float a = 1.0f / k;
        return Vector2(a * x, a * y);
    }

    Vector2& operator+=(const Vector2& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vector2& operator*=(float k) {
        x *= k;
        y *= k;
        return *this;
    }

    Vector2& operator*=(const Vector2& o) {
        x *= o.x;
        y *= o.y;
        return *this;
    }

    Vector2& operator/=(float k) {
        assert(!FloatEq(k, 0.0f));
        float a = 1.0f / k;
        x *= a;
        y *= a;
        return *this;
    }

    Vector2 operator*(const Vector2& v) const { return {v.x * x, v.y * y}; }

    void Set(float x, float y) {
        this->x = x;
        this->y = y;
    }

    void Normalize() {
        float a = 1.0f / Length(*this);
        *this *= a;
    }
};

inline Vector2 operator*(float k, const Vector2& v) {
    return Vector2(k * v.x, k * v.y);
}

inline std::ostream& operator<<(std::ostream& stream, const Vector2& v) {
    stream << "[" << v.x << ", " << v.y << "]";
    return stream;
}

inline float Cross(const Vector2& v1, const Vector2& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

inline float Dot(const Vector2& v1, const Vector2& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

inline float Distance(const Vector2& v1, const Vector2& v2) {
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    return std::sqrt(dx * dx + dy * dy);
}

inline float LengthSquare(const Vector2& v) {
    return Dot(v, v);
}

inline float Length(const Vector2& v) {
    return std::sqrt(LengthSquare(v));
}

struct Rect {
    float x, y, w, h;
};

template <typename T>
T Lerp(T a, T b, float t) {
    return a + (b - a)* t;
}

}  // namespace math
