#include "test_fixed_int.h"

#include <cassert>
#include <cmath>

#include "dux_fixed.h"
#include "utils.h"

using namespace dux;
using namespace dux::trig;
using namespace dux_test_utils;

void TestFInt() {
  // Test |FromInt|, |FromRawValue|, |FromDouble|.
  assert(FInt::FromInt(12).raw_value_ == 12 << FInt::kShift);
  assert(FInt::FromRawValue(12).raw_value_ == 12);
  assert(FInt::FromDouble(12) == FInt::FromInt(12));

  // Test |Int32|.
  assert(FInt::FromInt(4).Int32() == 4);
  assert(FInt::FromInt(-4).Int32() == -4);

  // Test |DoubleValue|.
  assert(FInt::FromFraction(1, 3).DoubleValue() > 0.333);
  assert(FInt::FromFraction(1, 3).DoubleValue() < 0.334);

  // Test |FloatValue|.
  assert(FInt::FromFraction(1, 3).FloatValue() > 0.333);
  assert(FInt::FromFraction(1, 3).FloatValue() < 0.334);

  // Test |Abs|.
  assert(FInt::FromInt(0).Abs() == FInt::FromInt(0));
  assert(FInt::FromInt(1).Abs() == FInt::FromInt(1));
  assert(FInt::FromInt(-1).Abs() == FInt::FromInt(1));
  assert(FInt::FromInt(-15345432).Abs() == FInt::FromInt(15345432));

  // Test |Floor|.
  assert(FInt::FromInt(0).Floor() == FInt::FromInt(0));
  // Positive values.
  assert(FInt::FromInt(1).Floor() == FInt::FromInt(1));
  assert(FInt::FromDouble(1.1).Floor() == FInt::FromInt(1));
  assert(FInt::FromDouble(1.9).Floor() == FInt::FromInt(1));
  assert(FInt::FromDouble(1000.001).Floor() == FInt::FromInt(1000));
  // Negative values.
  assert(FInt::FromInt(-1).Floor() == FInt::FromInt(-1));
  assert(FInt::FromInt(-10).Floor() == FInt::FromInt(-10));
  assert(FInt::FromDouble(-1.1).Floor() == FInt::FromInt(-2));
  assert(FInt::FromDouble(-1.9).Floor() == FInt::FromInt(-2));
  assert(FInt::FromDouble(-1000.001).Floor() == FInt::FromInt(-1001));

  // Test |Ceil|.
  assert(FInt::FromInt(0).Ceil() == FInt::FromInt(0));
  // Positive values.
  assert(FInt::FromInt(1).Ceil() == FInt::FromInt(1));
  assert(FInt::FromDouble(1.1).Ceil() == FInt::FromInt(2));
  assert(FInt::FromDouble(1.9).Ceil() == FInt::FromInt(2));
  // Negative values.
  assert(FInt::FromInt(-1).Ceil() == FInt::FromInt(-1));
  assert(FInt::FromDouble(-1.1).Ceil() == FInt::FromInt(-1));
  assert(FInt::FromDouble(-1.9).Ceil() == FInt::FromInt(-1));

  // Test |Round|.
  assert(FInt::FromInt(0).Round() == FInt::FromInt(0));
  // Positive values.
  assert(FInt::FromDouble(0.45).Round() == FInt::FromInt(0));
  assert(FInt::FromDouble(0.55).Round() == FInt::FromInt(1));
  assert(FInt::FromDouble(10.45).Round() == FInt::FromInt(10));
  assert(FInt::FromDouble(10.55).Round() == FInt::FromInt(11));
  // Negative values.
  assert(FInt::FromDouble(-0.45).Round() == FInt::FromInt(0));
  assert(FInt::FromDouble(-0.55).Round() == FInt::FromInt(-1));
  assert(FInt::FromDouble(-10.45).Round() == FInt::FromInt(-10));
  assert(FInt::FromDouble(-10.55).Round() == FInt::FromInt(-11));

  // Test |Frac|.
  assert(FInt::FromInt(0).Frac() == FInt::FromInt(0));
  assert(FInt::FromInt(3).Frac() == FInt::FromInt(0));
  assert(FInt::FromInt(-3).Frac() == FInt::FromInt(0));
  assert(FInt::FromFraction(1, 2).Frac() == FInt::FromFraction(1, 2));
  assert(FInt::FromFraction(101, 2).Frac() == FInt::FromFraction(1, 2));
  assert(FInt::FromFraction(-1, 2).Frac() == FInt::FromFraction(1, 2));
  assert(FInt::FromFraction(-101, 2).Frac() == FInt::FromFraction(1, 2));
  assert(FInt::FromFraction(1, 3).Frac() == FInt::FromFraction(1, 3));
  assert(FInt::FromFraction(4, 3).Frac() == FInt::FromFraction(1, 3));
  AssertNearlyEqual(2.0 / 3, FInt::FromFraction(-1, 3).Frac());
  AssertNearlyEqual(2.0 / 3, FInt::FromFraction(-4, 3).Frac());

  // Test same sign.
  assert(FInt::FromInt(20).IsSameSignAs(FInt::FromInt(10)));
  assert(FInt::FromInt(20).IsSameSignAs(FInt::FromInt(0)));
  assert(FInt::FromInt(0).IsSameSignAs(FInt::FromInt(0)));
  assert(FInt::FromInt(-20).IsSameSignAs(FInt::FromInt(-10)));
  assert(!FInt::FromInt(-20).IsSameSignAs(FInt::FromInt(0)));
  assert(!FInt::FromInt(-20).IsSameSignAs(FInt::FromInt(10)));
  assert(FInt::kMax.IsSameSignAs(dux::FInt::FromInt(10)));
  assert(FInt::kMin.IsSameSignAs(dux::FInt::FromInt(-10)));
  assert(!FInt::kMin.IsSameSignAs(FInt::kMax));

  // Test |Sqrt|.
  assert(FInt::FromInt(144).Sqrt() == FInt::FromInt(12));
  // Test values in the int32 range.
  for (int32_t i = 0; i <= 0x7fff; i++) {
    AssertNearlyEqual(static_cast<double>(sqrtf(i)), FInt::FromInt(i).Sqrt());
  }
  // Test large values.
  for (int64_t i = 797003437; i < FInt::kMax.raw_value_ - (2LL << 50);
       i += 1LL << 50) {
    FInt v = FInt::FromRawValue(i);
    AssertNearlyEqual(
        static_cast<double>(sqrtf(static_cast<float>(v.DoubleValue()))),
        v.Sqrt(), 2.01);
  }

  // Test ==, !=, < operators.
  assert(FInt::FromInt(434) == FInt::FromInt(434));
  assert(FInt::FromInt(-1) != FInt::FromInt(1));
  assert(FInt::FromInt(312) < FInt::FromInt(1432));

  // Test <<, >> operators.
  assert((FInt::FromInt(111) << 2) == FInt::FromInt(444));
  assert((FInt::FromInt(111) >> 2) != FInt::FromInt(111 >> 2));
  assert((FInt::FromInt(111) >> 2) == FInt::FromFraction(111, 4));
  assert((FInt::FromInt(-111) << 2) == FInt::FromInt(-444));
  assert((FInt::FromInt(-111) >> 2) == FInt::FromFraction(-111, 4));

  // Test +, -, *, and / operators.
  assert(FInt::FromInt(4) * FInt::FromInt(2) == FInt::FromInt(8));
  assert(FInt::FromInt(4) * FInt::FromInt(-1) == FInt::FromInt(-4));
  assert(FInt::FromInt(-4) * FInt::FromInt(1) == FInt::FromInt(-4));
  assert(FInt::FromInt(-100) * FInt::FromInt(-2) == FInt::FromInt(200));
  assert(FInt::FromInt(40) / FInt::FromInt(2) == FInt::FromInt(20));
  assert(FInt::FromInt(40) / FInt::FromInt(-2) == FInt::FromInt(-20));
  assert(FInt::FromInt(-40) / FInt::FromInt(2) == FInt::FromInt(-20));
  assert(FInt::FromInt(-40) / FInt::FromInt(-2) == FInt::FromInt(20));
  int32_t int32_t_val = 2;
  uint32_t uint32_t_val = 2;
  int64_t int64_t_val = 2;
  uint64_t uint64_t_val = 2;
  assert(FInt::FromInt(40) * int32_t_val == FInt::FromInt(80));
  assert(FInt::FromInt(40) / int32_t_val == FInt::FromInt(20));
  assert(FInt::FromInt(40) * uint32_t_val == FInt::FromInt(80));
  assert(FInt::FromInt(40) / uint32_t_val == FInt::FromInt(20));
  assert(FInt::FromInt(40) * int64_t_val == FInt::FromInt(80));
  assert(FInt::FromInt(40) / int64_t_val == FInt::FromInt(20));
  assert(FInt::FromInt(40) * uint64_t_val == FInt::FromInt(80));
  assert(FInt::FromInt(40) / uint64_t_val == FInt::FromInt(20));
  int32_t_val = -2;
  int64_t_val = -2;
  assert(FInt::FromInt(40) * int32_t_val == FInt::FromInt(-80));
  assert(FInt::FromInt(40) / int32_t_val == FInt::FromInt(-20));
  assert(FInt::FromInt(40) * int64_t_val == FInt::FromInt(-80));
  assert(FInt::FromInt(40) / int64_t_val == FInt::FromInt(-20));

  // Test % operator.
  assert(FInt::FromInt(0) % FInt::FromInt(10) == FInt::FromInt(0));
  assert(FInt::FromRawValue(1) % FInt::FromInt(10) == FInt::FromRawValue(1));
  assert(FInt::FromInt(4) % FInt::FromInt(10) == FInt::FromInt(4));
  assert(FInt::FromInt(10) % FInt::FromInt(10) == FInt::FromInt(0));
  assert(FInt::FromInt(14) % FInt::FromInt(10) == FInt::FromInt(4));
  assert(FInt::FromInt(104) % FInt::FromInt(10) == FInt::FromInt(4));
  // Negative values
  assert(FInt::FromRawValue(-1) % FInt::FromInt(10) == FInt::FromRawValue(-1));
  assert(FInt::FromInt(-4) % FInt::FromInt(10) == FInt::FromInt(-4));
  assert(FInt::FromInt(-10) % FInt::FromInt(10) == FInt::FromInt(0));
  assert(FInt::FromInt(-14) % FInt::FromInt(10) == FInt::FromInt(-4));
  assert(FInt::FromInt(-104) % FInt::FromInt(10) == FInt::FromInt(-4));

  // Euclidean division remainder
  assert(FInt::FromInt(0).EuclideanDivisionRemainder(FInt::FromInt(10)) ==
         FInt::FromInt(0));
  assert(FInt::FromRawValue(1).EuclideanDivisionRemainder(FInt::FromInt(10)) ==
         FInt::FromRawValue(1));
  assert(FInt::FromInt(4).EuclideanDivisionRemainder(FInt::FromInt(10)) ==
         FInt::FromInt(4));
  assert(FInt::FromInt(10).EuclideanDivisionRemainder(FInt::FromInt(10)) ==
         FInt::FromInt(0));
  assert(FInt::FromInt(14).EuclideanDivisionRemainder(FInt::FromInt(10)) ==
         FInt::FromInt(4));
  assert(FInt::FromInt(104).EuclideanDivisionRemainder(FInt::FromInt(10)) ==
         FInt::FromInt(4));
  // Negative values
  assert(FInt::FromRawValue(-1).EuclideanDivisionRemainder(FInt::FromInt(10)) ==
         FInt::FromRawValue(FInt::FromInt(10).raw_value_ - 1));
  assert(FInt::FromInt(-1).EuclideanDivisionRemainder(FInt::FromInt(10)) ==
         FInt::FromInt(9));
  assert(FInt::FromInt(-10).EuclideanDivisionRemainder(FInt::FromInt(10)) ==
         FInt::FromInt(0));
  assert(FInt::FromInt(-16).EuclideanDivisionRemainder(FInt::FromInt(10)) ==
         FInt::FromInt(4));
  assert(FInt::FromInt(-106).EuclideanDivisionRemainder(FInt::FromInt(10)) ==
         FInt::FromInt(4));

  // Test +=, -=, *=, and /= operators.
  FInt a = FInt::FromInt(43);
  a += FInt::FromInt(1);
  assert(a == FInt::FromInt(44));
  a -= FInt::FromInt(2);
  assert(a == FInt::FromInt(42));
  a *= FInt::FromInt(3);
  assert(a == FInt::FromInt(42 * 3));
  a *= FInt::FromInt(-3);
  assert(a == FInt::FromInt(42 * -9));
  a /= FInt::FromInt(3);
  assert(a == FInt::FromInt(42 * -3));
  a /= FInt::FromInt(-3);
  assert(a == FInt::FromInt(42));

  // Test constants.
  assert(FInt::kMax > dux::FInt::FromInt(10000000));
  assert(FInt::kMin < dux::FInt::FromInt(-10000000));
  assert(FInt::kMax.raw_value_ + 1 == FInt::kMin.raw_value_);
  assert(FInt::kZero == dux::FInt::FromInt(0));
  AssertNearlyEqual(FInt::kQuarterPi.DoubleValue() * 2, FInt::kHalfPi);
  AssertNearlyEqual(FInt::kHalfPi.DoubleValue() * 2, FInt::kPi);
  AssertNearlyEqual(FInt::kPi.DoubleValue() * 2, FInt::kTwoPi);
  AssertNearlyEqual(3.14159, FInt::kPi);
}
