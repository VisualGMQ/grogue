#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>

#define GROGUE_MATH_FLOAT_COMPARE_USE_EPSILON

namespace math {

inline bool FloatEq(float value1, float value2) {
#ifdef GROGUE_MATH_FLOAT_COMPARE_USE_EPSILON
    return std::abs(value1 - value2) <= std::numeric_limits<float>::epsilon();
#else
    return value1 == value2;
#endif
}

// forward declarations
class Vector3;
float Length(const Vector3& v);

class Vector3 {
public:
    float x, y, z;
    static const Vector3 Zero;
    static const Vector3 XAxis;
    static const Vector3 YAxis;
    static const Vector3 ZAxis;

    Vector3() {}
    Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}
    Vector3(float vx, float vy, float vz) : x(vx), y(vy), z(vz) {}
    ~Vector3() {}

    bool operator==(const Vector3& v) const {
        return FloatEq(x, v.x) && FloatEq(y, v.y) && FloatEq(z, v.z);
    }

    bool operator!=(const Vector3& v) const {
        return !(*this == v);
    }

    Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }  // Vector negation

    Vector3 operator+(const Vector3& v) {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 operator-(const Vector3& v) {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3 operator*(float k) { return Vector3(k * x, k * y, k * z); }

    Vector3 operator/(float k) {
        assert(std::numeric_limits<float>::epsilon() < abs(k));
        float a = 1.0f / k;
        return Vector3(a * x, a * y, a * z);
    }

    Vector3 operator+=(const Vector3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3 operator-=(const Vector3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3 operator*=(float k) {
        x *= k;
        y *= k;
        z *= k;
        return *this;
    }

    Vector3 operator/=(float k) {
        assert(!FloatEq(k, 0.0f));
        float a = 1.0f / k;
        x *= a;
        y *= a;
        z *= a;
        return *this;
    }

    Vector3 operator*(const Vector3& v) {
        return {v.x * x, v.y * y, v.z * z};
    }

    void Normalize() {
        float a = 1.0f / Length(*this);
        *this *= a;
    }
};

inline Vector3 operator*(float k, const Vector3& v) {
    return Vector3(k * v.x, k * v.y, k * v.z);
}

inline std::ostream& operator<<(std::ostream& stream, const Vector3& v) {
    stream << "Vector3[" << v.x << ", " << v.y << ", " << v.z << "]";
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
    return sqrt(dx * dx + dy * dy + dz * dz);
}

inline float LengthSquare(const Vector3& v) {
    return Dot(v, v);
}

inline float Length(const Vector3& v) {
    return std::sqrt(LengthSquare(v));
}

}  // namespace math
