#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "core/vector3.hpp"

TEST_CASE("Vector3 for vector 3D representation and calculate") {
  vector3::Vector3 v(1, 1, 1);
  vector3::Vector3 a(v);
  vector3::Vector3 b = -v;
  vector3::Vector3 c(2, 2, 2);
  vector3::Vector3 d(1, 0, 0);
  vector3::Vector3 e(0, 1, -1);
  vector3::Vector3 f(0, -1, 1);

  REQUIRE( a != vector3::Vector3::kZeroVector );
  a = a+b;
  REQUIRE( a == vector3::Vector3::kZeroVector );
  b.Zero();
  REQUIRE( b == vector3::Vector3::kZeroVector );
  a = b = v;
  a = a - b;
  REQUIRE( a == vector3::Vector3::kZeroVector );
  a = b = v;
  a += v;
  b *= 2;
  REQUIRE( a == b );
  a -= v;
  REQUIRE( a == v );
  a = a*2;
  REQUIRE( a == c );
  REQUIRE( b == c );
  b /= 2;
  REQUIRE( b == v );
  b = 2*b;
  REQUIRE( a == c );
  REQUIRE( b == c );
  REQUIRE( a*b == 12);
  REQUIRE( vector3::Distance(vector3::Vector3::kZeroVector, d) == 1 );
  REQUIRE( vector3::CrossMul(v, d) == e );
  REQUIRE( vector3::CrossMul(d, v) == f );
}