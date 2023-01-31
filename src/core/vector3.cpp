#include "core/vector3.hpp"

using namespace vector3;

const Vector3 Vector3::kZeroVector(0, 0, 0);

void Vector3::Zero() { x = y = z = 0.0f; }  // Set zero vector

void Vector3::Normalize() {
  float a = 1.0f/vector3::Modulus(*this);
  *this *= a;
}
