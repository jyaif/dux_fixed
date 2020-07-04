#include "test_fixed_vec2.h"

#include <cmath>

#include "dux_fixed.h"
#include "utils.h"

using namespace dux;
using namespace dux::trig;
using namespace dux_test_utils;

void TestFVec2() {
  // Test |Length|.
  assert(dux::FVec2(10, 10).SquareLength() == dux::FInt::FromInt(200));
  assert(dux::FVec2(3, 4).Length() == dux::FInt::FromInt(5));

  // Test |DotProduct|.
  assert(dux::FVec2(0, 0).DotProduct(dux::FVec2(0, 0)) ==
         dux::FInt::FromInt(0));
  assert(dux::FVec2(10, 0).DotProduct(dux::FVec2(0, 0)) ==
         dux::FInt::FromInt(0));
  assert(dux::FVec2(10, 0).DotProduct(dux::FVec2(0, 10)) ==
         dux::FInt::FromInt(0));
  assert(dux::FVec2(10, 10).DotProduct(dux::FVec2(10, -10)) ==
         dux::FInt::FromInt(0));
  assert(dux::FVec2(10, 0).DotProduct(dux::FVec2(10, 0)) ==
         dux::FInt::FromInt(100));
  assert(dux::FVec2(10, 10).DotProduct(dux::FVec2(10, 0)) ==
         dux::FInt::FromInt(100));
  assert(dux::FVec2(10, 10).DotProduct(dux::FVec2(10, 10)) ==
         dux::FInt::FromInt(200));

  // Test |Normalize|.
  bool normalized = true;
  dux::FVec2(0, 0).Normalize(normalized);
  assert(normalized == false);
  dux::FVec2(1, 0).Normalize(normalized);
  assert(normalized == true);

  dux::FVec2 v1(10, 0);
  v1.Normalize(normalized);
  assert(normalized == true);
  assert(dux::FVec2(1, 0) == v1);

  dux::FVec2 v2(1000, 1000);
  v2.Normalize(normalized);
  AssertNearlyEqual(sqrt(0.5), v2.x_);
  AssertNearlyEqual(sqrt(0.5), v2.y_);

  dux::FVec2 v3(-1000, 1000);
  v3.Normalize(normalized);
  AssertNearlyEqual(-sqrt(0.5), v3.x_);
  AssertNearlyEqual(sqrt(0.5), v3.y_);

  // Test |FromAngle|.
  for (double float_angle = -10; float_angle < 20; float_angle += 0.01) {
    FInt angle = FInt::FromDouble(float_angle);
    FVec2 v_from_angle = FVec2::FromAngle(angle, dux::FInt::FromInt(10));
    AssertNearlyEqual(cos(float_angle) * 10, v_from_angle.x_);
    AssertNearlyEqual(sin(float_angle) * 10, v_from_angle.y_);
  }

  // Test |Rotate|.
  // Initialise a vector with 30 degrees (PI/6), and rotate it.
  for (dux::FInt rotation; rotation < dux::FInt::kTwoPi;
       rotation += (dux::FInt::kTwoPi * dux::FInt::FromFraction(1, 64))) {
    FVec2 v = FVec2::FromAngle(dux::FInt::kPi * dux::FInt::FromFraction(1, 6),
                               dux::FInt::FromInt(10));
    v.Rotate(rotation);
    AssertNearlyEqual(cos(M_PI / 6 + rotation.FloatValue()) * 10, v.x_, 0.04);
    AssertNearlyEqual(sin(M_PI / 6 + rotation.FloatValue()) * 10, v.y_, 0.04);
  }

  // TODO: test everything.
}
