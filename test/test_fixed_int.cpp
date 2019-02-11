#include "test_fixed_int.h"

#include <cassert>
#include <cmath>

#include "dux_fixed.h"
#include "utils.h"

using namespace dux;
using namespace dux::trig;
using namespace dux_test_utils;

void TestFInt() {
  // Test |Int32|.
  assert(FInt(4).Int32() == 4);
  assert(FInt(-4).Int32() == -4);

  // Test |DoubleValue|.
  assert(FInt::FromFraction(1, 3).DoubleValue() > 0.333);
  assert(FInt::FromFraction(1, 3).DoubleValue() < 0.334);

  // Test |FloatValue|.
  assert(FInt::FromFraction(1, 3).FloatValue() > 0.333);
  assert(FInt::FromFraction(1, 3).FloatValue() < 0.334);

  // Test |Abs|.
  assert(FInt(0).Abs() == FInt(0));
  assert(FInt(1).Abs() == FInt(1));
  assert(FInt(-1).Abs() == FInt(1));
  assert(FInt(-15345432).Abs() == FInt(15345432));

  // Test |Floor|.
  assert(FInt(0).Floor() == FInt(0));
  // Positive values.
  assert(FInt(1).Floor() == FInt(1));
  assert(FInt::FromDouble(1.1).Floor() == FInt(1));
  assert(FInt::FromDouble(1.9).Floor() == FInt(1));
  assert(FInt::FromDouble(1000.001).Floor() == FInt(1000));
  // Negative values.
  assert(FInt(-1).Floor() == FInt(-1));
  assert(FInt(-10).Floor() == FInt(-10));
  assert(FInt::FromDouble(-1.1).Floor() == FInt(-2));
  assert(FInt::FromDouble(-1.9).Floor() == FInt(-2));
  assert(FInt::FromDouble(-1000.001).Floor() == FInt(-1001));

  // Test |Ceil|.
  assert(FInt(0).Ceil() == FInt(0));
  // Positive values.
  assert(FInt(1).Ceil() == FInt(1));
  assert(FInt::FromDouble(1.1).Ceil() == FInt(2));
  assert(FInt::FromDouble(1.9).Ceil() == FInt(2));
  // Negative values.
  assert(FInt(-1).Ceil() == FInt(-1));
  assert(FInt::FromDouble(-1.1).Ceil() == FInt(-1));
  assert(FInt::FromDouble(-1.9).Ceil() == FInt(-1));

  // Test |Round|.
  assert(FInt(0).Round() == FInt(0));
  // Positive values.
  assert(FInt::FromDouble(0.45).Round() == FInt(0));
  assert(FInt::FromDouble(0.55).Round() == FInt(1));
  assert(FInt::FromDouble(10.45).Round() == FInt(10));
  assert(FInt::FromDouble(10.55).Round() == FInt(11));
  // Negative values.
  assert(FInt::FromDouble(-0.45).Round() == FInt(0));
  assert(FInt::FromDouble(-0.55).Round() == FInt(-1));
  assert(FInt::FromDouble(-10.45).Round() == FInt(-10));
  assert(FInt::FromDouble(-10.55).Round() == FInt(-11));

  // Test same sign.
  assert(FInt(20).IsSameSignAs(FInt(10)));
  assert(FInt(20).IsSameSignAs(FInt(0)));
  assert(FInt(0).IsSameSignAs(FInt(0)));
  assert(FInt(-20).IsSameSignAs(FInt(-10)));
  assert(!FInt(-20).IsSameSignAs(FInt(0)));
  assert(!FInt(-20).IsSameSignAs(FInt(10)));
  assert(FInt::kMax.IsSameSignAs(dux::FInt(10)));
  assert(FInt::kMin.IsSameSignAs(dux::FInt(-10)));
  assert(!FInt::kMin.IsSameSignAs(FInt::kMax));

  // Test |Sqrt|.
  assert(FInt(144).Sqrt() == FInt(12));
  // Test values in the int32 range.
  for (int32_t i = 0; i <= 0x7fff; i++) {
    AssertNearlyEqual(static_cast<double>(sqrtf(i)), FInt(i).Sqrt());
  }
  // Test large values.
  for (int64_t i = 797003437; i < FInt::kMax.raw_value_ - (2LL << 50);
       i += 1LL << 50) {
    FInt v = FInt::FromRawValue(i);
    AssertNearlyEqual(
        static_cast<double>(sqrtf(static_cast<float>(v.DoubleValue()))),
        v.Sqrt());
  }

  // Test ==, !=, < operators.
  assert(FInt(434) == FInt(434));
  assert(FInt(-1) != FInt(1));
  assert(FInt(312) < FInt(1432));

  // Test +, -, *, and / operators.
  assert(FInt(4) * FInt(2) == FInt(8));
  assert(FInt(-100) * FInt(-2) == FInt(200));
  assert(FInt(40) / FInt(2) == FInt(20));
  int32_t int32_t_val = 2;
  uint32_t uint32_t_val = 2;
  int64_t int64_t_val = 2;
  uint64_t uint64_t_val = 2;
  assert(FInt(40) * int32_t_val == FInt(80));
  assert(FInt(40) / int32_t_val == FInt(20));
  assert(FInt(40) * uint32_t_val == FInt(80));
  assert(FInt(40) / uint32_t_val == FInt(20));
  assert(FInt(40) * int64_t_val == FInt(80));
  assert(FInt(40) / int64_t_val == FInt(20));
  assert(FInt(40) * uint64_t_val == FInt(80));
  assert(FInt(40) / uint64_t_val == FInt(20));

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

  // Test constants.
  assert(FInt::kMax > dux::FInt(10000000));
  assert(FInt::kMin < dux::FInt(-10000000));
  assert(FInt::kZero == dux::FInt(0));
  AssertNearlyEqual(FInt::kQuarterPi.DoubleValue() * 2, FInt::kHalfPi);
  AssertNearlyEqual(FInt::kHalfPi.DoubleValue() * 2, FInt::kPi);
  AssertNearlyEqual(FInt::kPi.DoubleValue() * 2, FInt::kTwoPi);
  AssertNearlyEqual(3.14159, FInt::kPi);
}
