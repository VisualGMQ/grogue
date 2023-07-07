#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>

//! \breif  if define this, will use std::numeric_limits<float>::epsilon() to compare floats. Or use ==
#define GROGUE_MATH_FLOAT_COMPARE_USE_EPSILON

namespace math {

//! @defgroup math
//! @brief some math utilities
//! @{

constexpr double PI = 3.1415926535897932384626;

//! @brief  compare two float value by epsilon
//! @param value1 
//! @param value2 
//! @return  whether two float equal
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
Vector3 Normalize(const Vector3& v);
Vector2 Normalize(const Vector2& v);
float Cross(const Vector2& v1, const Vector2& v2);
float Dot(const Vector2& v1, const Vector2& v2);
float Distance(const Vector2& v1, const Vector2& v2);
float LengthSqrd(const Vector2& v);
float Length(const Vector2& v);

//! @brief convert radians to degress
//! @param radians 
//! @return  degrees
inline float [[refl, luabind]] Rad2Deg(float radians) {
    return radians * 180.0 / PI;
}

//! @brief covert degress to radians
//! @param degree 
//! @return  radians
inline float [[refl, luabind]] Deg2Rad(float degree) {
    return degree * PI / 180.0;
}

//! @brief 3D vector which has x, y, z components
class [[refl, luabind]] Vector3 {
public:
    float x = 0;
    float y = 0;
    float z = 0;

    static const Vector3 Zero;  //!< @brief (0, 0, 0)
    static const Vector3 XAxis; //!< @breif (1, 0, 0)
    static const Vector3 YAxis; //!< @brief (0, 1, 0)
    static const Vector3 ZAxis; //!< @brief (0, 0, 1)

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

    //! @brief normalize vector itself. Use global `Normalize` function to return a normalized vector
    //! @sa Normalize
    void Normalize() {
        *this = ::math::Normalize(*this);
    }
};

inline Vector3 operator*(float k, const Vector3& v) {
    return Vector3(k * v.x, k * v.y, k * v.z);
}

inline std::ostream& operator<<(std::ostream& stream, const Vector3& v) {
    stream << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return stream;
}

inline Vector3 [[refl, luabind]] Cross(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                   v1.x * v2.y - v1.y * v2.x);
}

inline float [[refl, luabind]] Dot(const Vector3& v1, const Vector3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float [[refl, luabind]] Distance(const Vector3& v1, const Vector3& v2) {
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    float dz = v1.z - v2.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

inline float [[refl, luabind]] LengthSquare(const Vector3& v) {
    return Dot(v, v);
}

inline float [[refl, luabind]] Length(const Vector3& v) {
    return std::sqrt(LengthSquare(v));
}

//! @brief return a normalized vector. Don't change origin vector
inline Vector3 [[refl, luabind]] Normalize(const Vector3& v) {
    float a = 1.0f / Length(v);
    return v * a;
}

//! @brief 2D vector which has x, y components
class [[refl, luabind]] Vector2 {
public:
    static const Vector2 Zero;  //!< @brief (0, 0)
    static const Vector2 XAxis; //!< @brief (1, 0)
    static const Vector2 YAxis; //!< @brief (0, 1)

    float x = 0;
    float y = 0;

    Vector2() {}
    explicit Vector2(float value): x(value), y(value) {}

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

    //! @brief normalize vector itself. Use global `Normalize` function to return a normalized vector
    //! @sa Normalize
    void Normalize() {
        *this = ::math::Normalize(*this);
    }

    float Dot(const Vector2& v) const {
        return ::math::Dot(*this, v);
    }

    float Cross(const Vector2& v) const {
        return ::math::Cross(*this, v);
    }

    float Distance(const Vector2& v) const {
        return ::math::Distance(*this, v);
    }

    float Length() const {
        return ::math::Length(*this);
    }

    float LengthSqrd() const {
        return ::math::LengthSqrd(*this);
    }
};

//! @brief return a normalized vector. Don't change origin vector
inline Vector2 [[refl, luabind]] Normalize(const Vector2& v) {
    float len = Length(v);
    if (FloatEq(len, 0)) {
        return Vector2::Zero;
    }
    float a = 1.0f / len;
    return v * a;
}

inline Vector2 operator*(float k, const Vector2& v) {
    return Vector2(k * v.x, k * v.y);
}

inline std::ostream& operator<<(std::ostream& stream, const Vector2& v) {
    stream << "[" << v.x << ", " << v.y << "]";
    return stream;
}

inline float [[refl, luabind]] Cross(const Vector2& v1, const Vector2& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

inline float [[refl, luabind]] Dot(const Vector2& v1, const Vector2& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

inline float [[refl, luabind]] Distance(const Vector2& v1, const Vector2& v2) {
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    return std::sqrt(dx * dx + dy * dy);
}

inline float [[refl, luabind]] LengthSqrd(const Vector2& v) {
    return Dot(v, v);
}

inline float [[refl, luabind]] Length(const Vector2& v) {
    return std::sqrt(LengthSqrd(v));
}

struct [[refl, luabind]] Rect {
    float x = 0;
    float y = 0;
    float w = 0;
    float h = 0;

    Rect() = default;
    Rect(float x, float y, float w, float h): x(x), y(y), w(w), h(h) {
    }

    static Rect Create(float x, float y, float w, float h) {
        return Rect{x, y, w, h};
    }

    static Rect Create(const Vector2& position, const Vector2& size) {
        return Rect{position.x, position.y, size.x, size.y};
    }

    bool ContainPt(const Vector2& pt) const {
        return pt.x > x && pt.x < x + w && pt.y > y && pt.y < y + h;
    }

    bool IsIntersect(const Rect& o) const {
        return !(x + w <= o.x || x >= o.x + o.w || y + h <= o.y ||
                 y >= o.y + o.h);
    }

    std::optional<Rect> Union(const Rect& o) const {
        Vector2 min(std::max(o.x, x), std::max(o.y, y)),
                max(std::min(o.x + o.w, x + w), std::min(o.y + o.h, y + h));
        if (min.x >= max.x || min.y >= max.y) {
            return std::nullopt;
        } else {
            return Rect::Create(min, max - min);
        }
    }
};

template <typename T>
T Lerp(T a, T b, float t) {
    return a + (b - a)* t;
}

// row-major matrix
template <typename T>
class HeapMatrix final {
public:
    HeapMatrix(int w, int h): w_(w), h_(h) {
        datas_ = std::unique_ptr<T[]>(new T[w * h]);
    }

    void Fill(const T& elem) {
        for (int i = 0; i < w_ * h_; i++) {
            *(datas_.get() + i) = elem;
        }
    }

    int Width() const {
        return w_;
    }

    int Height() const {
        return h_;
    }

    const T& Get(int x, int y) const {
        assert(x + y * w_ < w_ * h_ && x >= 0 && y >= 0);
        return datas_.get()[x + y * w_];
    }

    T& Get(int x, int y) {
        assert(x + y * w_ < w_ * h_ && x >= 0 && y >= 0);
        return datas_.get()[x + y * w_];
    }

    void Set(int x, int y, const T& t) {
        assert(x + y * w_ < w_ * h_ && x >= 0 && y >= 0);
        datas_.get()[x + y * w_] = t;
    }

    void Set(int x, int y, T&& t) {
        assert(x + y * w_ < w_ * h_ && x >= 0 && y >= 0);
        datas_.get()[x + y * w_] = std::move(t);
    }

    bool IsInRange(int x, int y) const {
        return x >= 0 && x < Width() && y >= 0 && y < Height();
    }

private:
    std::unique_ptr<T[]> datas_;
    int w_;
    int h_;
};

//! @brief rotate a 2D vector
//! @param v src vector
//! @param radians angle in radians, in clockwise
//! @return rotated vector
inline Vector2 [[refl, luabind]] Rotate(const Vector2& v, float radians) {
    float c = std::cos(radians);
    float s = std::sin(radians);

    return Vector2(v.x * c - v.y * s, v.x * s + v.y * c);
}

inline Vector2 [[refl, luabind]] Scale(const Vector2& v, const Vector2& scale) {
    return Vector2(v.x * scale.x, v.y * scale.y);
}

inline Vector2 [[refl, luabind]] Translate(const Vector2& v, const Vector2& offset) {
    return Vector2(v.x + offset.x, v.y + offset.y);
}

//! @}

}  // namespace math
