#pragma once

#include <iostream>
#include <limits>
#include <cassert>
#include <cmath>

namespace vector3 {
class Vector3 {
public:
  float x, y, z;
  static const Vector3 kZeroVector;
  Vector3() {}
  Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}
  Vector3(float vx, float vy, float vz) : x(vx), y(vy), z(vz) {}
  ~Vector3() {}

  Vector3& operator=(const Vector3& v) {
    x = v.x; y = v.y; z = v.z;
    return *this;
  }

  bool operator==(const Vector3& v) const {
    return x==v.x && y==v.y && z==v.z;
  }

  bool operator!=(const Vector3& v) const {
    return x!=v.x || y!=v.y || z!=v.z;
  }

  Vector3 operator-() const { return Vector3(-x, -y, -z); }    // Vector negation

  Vector3 operator+(const Vector3 &v) {
    return Vector3(x + v.x, y + v.y, z + v.z);
  }

  Vector3 operator-(const Vector3 &v) {
    return Vector3(x - v.x, y - v.y, z - v.z);
  }

  Vector3 operator*(float k) {
    return Vector3(k*x, k*y, k*z);
  }

  Vector3 operator/(float k) {
    assert(std::numeric_limits<float>::epsilon() < abs(k));
    float a = 1.0f/k;
    return Vector3(a*x, a*y, a*z);
  }

  Vector3 operator+=(const Vector3& v) {
    x += v.x; y += v.y; z += v.z;
    return *this;
  }

  Vector3 operator-=(const Vector3& v) {
    x -= v.x; y -= v.y; z -= v.z;
    return *this;
  }

  Vector3 operator*=(float k) {
    x *= k; y *= k; z *= k;
    return *this;
  }

  Vector3 operator/=(float k) {
    assert(std::numeric_limits<float>::epsilon() < abs(k));
    float a = 1.0f/k;
    x *= a; y *= a; z *= a;
    return *this;
  }

  float operator*(const Vector3& v) {
    return x*v.x + y*v.y + z*v.z;
  }
  
  void Zero();  // Set zero vector
  void Normalize();
};

inline Vector3 operator*(float k, const Vector3& v) {
  return Vector3(k*v.x, k*v.y, k*v.z);
}

inline std::ostream& operator<<(std::ostream& stream, const Vector3& v) {
  stream << "[" << v.x << ", " << v.y << ", " << v.z << "]";
  return stream;
}

inline Vector3 CrossMul(const Vector3& v1, const Vector3& v2) {
  return Vector3(
    v1.y*v2.z - v1.z*v2.y,
    v1.z*v2.x - v1.x*v2.z,
    v1.x*v2.y - v1.y*v2.x
  );
}

inline float Modulus(const Vector3& v) {
  float modulus = v.x*v.x + v.y*v.y + v.z*v.z;
  assert(std::numeric_limits<float>::epsilon() < abs(modulus));
  return sqrt(modulus);
}

inline float Distance(const Vector3& v1, const Vector3& v2) {
  float dx = v1.x - v2.x;
  float dy = v1.y - v2.y;
  float dz = v1.z - v2.z;
  return sqrt(dx*dx + dy*dy + dz*dz);
}
}

