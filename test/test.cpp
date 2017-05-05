#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <math.h>

#include "dux_fixed.h"

using namespace dux;

namespace {

void AssertNearlyEqual(FInt actual, double expected) {
  double delta = fabs(expected - actual.DoubleValue());
  assert(delta < 0.02);
}

FInt FromDouble(double value) {
  return FInt::FromFraction(value * (1 << FInt::kShift), (1 << FInt::kShift));
}
  
}  // namespace

void TestFixedInt() {
  // Test |Int32|.
  assert(FInt(4).Int32() == 4);
  assert(FInt(-4).Int32() == -4);
  
  // Test |DoubleValue|.
  assert(FInt::FromFraction(1, 3).DoubleValue() > 0.333);
  assert(FInt::FromFraction(1, 3).DoubleValue() < 0.334);
  
  // Test |Abs|.
  assert(FInt(0).Abs() == FInt(0));
  assert(FInt(1).Abs() == FInt(1));
  assert(FInt(-1).Abs() == FInt(1));
  assert(FInt(-15345432).Abs() == FInt(15345432));
  
  // Test |Floor|.
  assert(FInt(0).Floor() == FInt(0));
  // Positive values.
  assert(FInt(1).Floor() == FInt(1));
  assert(FromDouble(1.1).Floor() == FInt(1));
  assert(FromDouble(1.9).Floor() == FInt(1));
  assert(FromDouble(1000.001).Floor() == FInt(1000));
  // Negative values.
  assert(FInt(-1).Floor() == FInt(-1));
  assert(FInt(-10).Floor() == FInt(-10));
  assert(FromDouble(-1.1).Floor() == FInt(-2));
  assert(FromDouble(-1.9).Floor() == FInt(-2));
  assert(FromDouble(-1000.001).Floor() == FInt(-1001));
  
  // Test |Ceil|.
  assert(FInt(0).Ceil() == FInt(0));
  // Positive values.
  assert(FInt(1).Ceil() == FInt(1));
  assert(FromDouble(1.1).Ceil() == FInt(2));
  assert(FromDouble(1.9).Ceil() == FInt(2));
  // Negative values.
  assert(FInt(-1).Ceil() == FInt(-1));
  assert(FromDouble(-1.1).Ceil() == FInt(-1));
  assert(FromDouble(-1.9).Ceil() == FInt(-1));
  
  // Test |Round|.
  assert(FInt(0).Round() == FInt(0));
  // Positive values.
  assert(FromDouble(0.45).Round() == FInt(0));
  assert(FromDouble(0.55).Round() == FInt(1));
  assert(FromDouble(10.45).Round() == FInt(10));
  assert(FromDouble(10.55).Round() == FInt(11));
  // Negative values.
  assert(FromDouble(-0.45).Round() == FInt(0));
  assert(FromDouble(-0.55).Round() == FInt(-1));
  assert(FromDouble(-10.45).Round() == FInt(-10));
  assert(FromDouble(-10.55).Round() == FInt(-11));
  
  // Test |Sqrt|.
  assert(FInt(144).Sqrt() == FInt(12));
  
  // Test ==, !=, < operators.
  assert(FInt(434) == FInt(434));
  assert(FInt(-1) != FInt(1));
  assert(FInt(312) < FInt(1432));
  
  // Test +, -, *, and / operators.
  assert(FInt(4) * FInt(2) == FInt(8));
  assert(FInt(-100) * FInt(-2) == FInt(200));
  assert(FInt(40) / FInt(2) == FInt(20));
  
  // Test +=, -=, *=, and /= operators.
  FInt a(43);
  a += FInt(1);
  assert(a == FInt(44));
  a -= FInt(2);
  assert(a == FInt(42));
  a *= FInt(3);
  assert(a == FInt(42 * 3));
  a /= FInt(3);
  assert(a == FInt(42));
}

void TestTrig() {
  // Test sin, cos.
  AssertNearlyEqual(dux::trig::Cos(FInt(0)), 1);
  AssertNearlyEqual(dux::trig::Sin(FInt(0)), 0);
  AssertNearlyEqual(dux::trig::Cos(FInt(1)), cos(1));
  AssertNearlyEqual(dux::trig::Sin(FInt(1)), sin(1));
  AssertNearlyEqual(dux::trig::Cos(FInt(-1)), cos(-1));
  AssertNearlyEqual(dux::trig::Sin(FInt(-1)), sin(-1));
  
  // Test atan2.
  AssertNearlyEqual(dux::trig::Atan2(FInt(1), FInt(1)), atan2(1,1));
  AssertNearlyEqual(dux::trig::Atan2(FInt(-1), FInt(1)), atan2(-1,1));
  AssertNearlyEqual(dux::trig::Atan2(FInt(-1), FInt(-1)), atan2(-1,-1));
  AssertNearlyEqual(dux::trig::Atan2(FInt(1), FInt(-1)), atan2(1,-1));

  // Test ToRadian.
  AssertNearlyEqual(dux::trig::ToRadian(FInt(0)), 0);
  AssertNearlyEqual(dux::trig::ToRadian(FInt(360)), 0);
  AssertNearlyEqual(dux::trig::ToRadian(FInt(720)), 0);
  AssertNearlyEqual(dux::trig::ToRadian(FInt(-720)), 0);
  AssertNearlyEqual(dux::trig::ToRadian(FInt(180)), 3.141);
  AssertNearlyEqual(dux::trig::ToRadian(FInt(-180)), -3.141);
}

void TestVec2() {
  // Test length.
  assert(dux::FVec2(10, 10).SquareLength() == dux::FInt(200));
  assert(dux::FVec2(3, 4).Length() == dux::FInt(5));
  
  // Test normalization.
  bool normalized = true;
  dux::FVec2(0, 0).Normalize(normalized);
  assert(normalized == false);
  dux::FVec2(1, 0).Normalize(normalized);
  assert(normalized == true);
  
  // TODO: test everything.
}

int main(int argc, char *argv[]) {
  TestFixedInt();
  TestTrig();
  TestVec2();
  printf("tests successfully passed\n");
  return EXIT_SUCCESS;
}
