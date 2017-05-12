#include "test_trig.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "dux_fixed.h"
#include "utils.h"

using namespace dux;
using namespace dux::trig;
using namespace dux_test_utils;

void TestTrig() {
  // Test |Sin|, |Cos|, |Sincos|.
  for (double a = -10; a < 10; a += 0.001) {
    dux::FInt angle = dux::FInt::FromDouble(a);
    double expectedCos = cos(angle.DoubleValue());
    double expectedSin = sin(angle.DoubleValue());
    AssertNearlyEqual(expectedCos, Cos(angle));
    AssertNearlyEqual(expectedSin, Sin(angle));
    dux::FInt cos;
    dux::FInt sin;
    dux::trig::Sincos(angle, sin, cos);
    AssertNearlyEqual(expectedCos, cos);
    AssertNearlyEqual(expectedSin, sin);
  }

  // Test |Atan2|.
  for (double x = -10; x < 10; x += 1) {
    for (double y = -10; y < 10; y += 1) {
      // TODO: better handle results for y == 0
      if (y != 0) {
        FInt fixed_atan2_result = dux::trig::Atan2(FInt::FromDouble(y), FInt::FromDouble(x));
        double atan2_result = atan2(y,x);

        fixed_atan2_result += dux::FInt::kTwoPi;
        atan2_result += 2 * M_PI;

        fixed_atan2_result %= dux::FInt::kTwoPi;
        atan2_result = fmod(atan2_result, 2 * M_PI);

        AssertNearlyEqual(atan2_result, fixed_atan2_result);
      }
    }
  }

  // Test |ToRadian|.
  for (int i = -1000; i < 3000; i++) {
    dux::FInt angle = (dux::FInt::kHalfPi * i) / 512;
    AssertNearlyEqual(angle.DoubleValue() * M_PI / 180, dux::trig::ToRadian(angle));
  }
}
