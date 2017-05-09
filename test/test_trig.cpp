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
  // Test sin, cos.
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

  // Test atan2.
  AssertNearlyEqual(atan2(1,1), dux::trig::Atan2(FInt(1), FInt(1)));
  AssertNearlyEqual(atan2(-1,1), dux::trig::Atan2(FInt(-1), FInt(1)));
  AssertNearlyEqual(atan2(-1,-1), dux::trig::Atan2(FInt(-1), FInt(-1)));
  AssertNearlyEqual(atan2(1,-1), dux::trig::Atan2(FInt(1), FInt(-1)));

  // Test ToRadian.
  for (int i = -1000; i < 3000; i++) {
    dux::FInt angle = (dux::FInt::kHalfPi * i) / 512;
    AssertNearlyEqual(angle.DoubleValue() * M_PI / 180, dux::trig::ToRadian(angle));
  }
}
