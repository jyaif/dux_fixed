#include "test_fixed_int.h"

#include <cassert>
#include <cmath>

#include "fixed_int.h"
#include "utils.h"

using namespace dux;
using namespace dux::trig;
using namespace dux_test_utils;

void TestFInt() {
  // Test |FromInt|, |FromRawValue|, |FromDouble|.
  assert(FInt::FromInt(12).raw_value_ == 12 << FInt::kShift);
  assert(FInt::FromRawValue(12).raw_value_ == 12);
  assert(FInt::FromDouble(12) == (12_fx));
  assert(FInt::FromFloat(13) == (13_fx));
  assert(FInt::FromFloat(0.5) == (1_fx / 2_fx));

  // Test |_fx|.
  assert((12_fx) == FInt::FromInt(12));
  assert(2147483647_fx == FInt::FromInt(2147483647));
  assert(4294967294_fx == FInt::FromInt(2147483647) * FInt::FromInt(2));
  assert(-2147483647_fx == FInt::FromInt(-2147483647));
  assert(-4294967294_fx == FInt::FromInt(-2147483647) * FInt::FromInt(2));

  // Test |FromFraction|.
  assert(FInt::FromFraction(24, 2) == 12_fx);
  assert(FInt::FromFraction(3, 2) == 3_fx / 2_fx);

  // Test |Int32|.
  assert((4_fx).Int32() == 4);
  assert((-4_fx).Int32() == -4);

  // Test |DoubleValue|.
  assert(FInt::FromFraction(1, 3).DoubleValue() > 0.333);
  assert(FInt::FromFraction(1, 3).DoubleValue() < 0.334);

  // Test |FloatValue|.
  assert(FInt::FromFraction(1, 3).FloatValue() > 0.333f);
  assert(FInt::FromFraction(1, 3).FloatValue() < 0.334f);

  // Test |Abs|.
  assert((0_fx).Abs() == 0_fx);
  assert((1_fx).Abs() == 1_fx);
  assert((-1_fx).Abs() == 1_fx);
  assert((-15345432_fx).Abs() == 15345432_fx);

  // Test |Floor|.
  assert((0_fx).Floor() == 0_fx);
  // Positive values.
  assert((1_fx).Floor() == 1_fx);
  assert(FInt::FromDouble(1.1).Floor() == 1_fx);
  assert(FInt::FromDouble(1.9).Floor() == 1_fx);
  assert(FInt::FromDouble(1000.001).Floor() == 1000_fx);
  // Negative values.
  assert((-1_fx).Floor() == -1_fx);
  assert((-10_fx).Floor() == -10_fx);
  assert(FInt::FromDouble(-1.1).Floor() == -2_fx);
  assert(FInt::FromDouble(-1.9).Floor() == -2_fx);
  assert(FInt::FromDouble(-1000.001).Floor() == -1001_fx);

  // Test |Ceil|.
  assert((0_fx).Ceil() == 0_fx);
  // Positive values.
  assert((1_fx).Ceil() == 1_fx);
  assert(FInt::FromDouble(1.1).Ceil() == 2_fx);
  assert(FInt::FromDouble(1.9).Ceil() == 2_fx);
  // Negative values.
  assert((-1_fx).Ceil() == -1_fx);
  assert(FInt::FromDouble(-1.1).Ceil() == -1_fx);
  assert(FInt::FromDouble(-1.9).Ceil() == -1_fx);

  // Test |Round|.
  assert((0_fx).Round() == 0_fx);
  // Positive values.
  assert(FInt::FromDouble(0.45).Round() == 0_fx);
  assert(FInt::FromDouble(0.55).Round() == 1_fx);
  assert(FInt::FromDouble(10.45).Round() == 10_fx);
  assert(FInt::FromDouble(10.55).Round() == 11_fx);
  // Negative values.
  assert(FInt::FromDouble(-0.45).Round() == 0_fx);
  assert(FInt::FromDouble(-0.55).Round() == -1_fx);
  assert(FInt::FromDouble(-10.45).Round() == -10_fx);
  assert(FInt::FromDouble(-10.55).Round() == -11_fx);

  // Test |Frac|.
  assert((0_fx).Frac() == 0_fx);
  assert((3_fx).Frac() == 0_fx);
  assert((-3_fx).Frac() == 0_fx);
  assert(FInt::FromFraction(1, 2).Frac() == FInt::FromFraction(1, 2));
  assert(FInt::FromFraction(101, 2).Frac() == FInt::FromFraction(1, 2));
  assert(FInt::FromFraction(-1, 2).Frac() == FInt::FromFraction(1, 2));
  assert(FInt::FromFraction(-101, 2).Frac() == FInt::FromFraction(1, 2));
  assert(FInt::FromFraction(1, 3).Frac() == FInt::FromFraction(1, 3));
  assert(FInt::FromFraction(4, 3).Frac() == FInt::FromFraction(1, 3));
  AssertNearlyEqual(1.0 / 3, FInt::FromFraction(-1, 3).Frac());
  AssertNearlyEqual(1.0 / 3, FInt::FromFraction(-4, 3).Frac());
  AssertNearlyEqual(2.0 / 3, 1_fx - FInt::FromFraction(-1, 3).Frac());
  AssertNearlyEqual(2.0 / 3, 1_fx - FInt::FromFraction(-4, 3).Frac());

  // Test same sign.
  assert((20_fx).IsSameSignAs(10_fx));
  assert((20_fx).IsSameSignAs(0_fx));
  assert((0_fx).IsSameSignAs(0_fx));
  assert((-20_fx).IsSameSignAs(-10_fx));
  assert(!(-20_fx).IsSameSignAs(0_fx));
  assert(!(-20_fx).IsSameSignAs(10_fx));
  assert(FIntMax.IsSameSignAs(10_fx));
  assert(FIntMin.IsSameSignAs(-10_fx));
  assert(!FIntMin.IsSameSignAs(FIntMax));

  // Test |Sqrt|.
  assert((144_fx).Sqrt() == 12_fx);
  // Test values in the int32 range.
  for (int32_t i = 0; i <= 0x7fff; i++) {
    AssertNearlyEqual(static_cast<double>(sqrtf(i)), FInt::FromInt(i).Sqrt());
  }
  // Test large values.
  for (int64_t i = 797003437; i < FIntMax.raw_value_ - (2LL << 50);
       i += 1LL << 50) {
    FInt v = FInt::FromRawValue(i);
    AssertNearlyEqual(
        static_cast<double>(sqrtf(static_cast<float>(v.DoubleValue()))),
        v.Sqrt(), 2.01);
  }

  // Test |Exp|.
  // Test 800 values in the [0, 8] range.
  for (int i = 0; i < 800; i++) {
    double expected = expf((float)i / 100.0f);
    AssertNearlyEqual(expected, Exp(dux::FInt::FromFraction(i, 100)), 1.1);
  }
  // Test 800 vaues in the [-8, 0] range
  for (int i = -800; i < 0; i++) {
    double expected = expf((float)i / 100.0f);
    AssertNearlyEqual(expected, Exp(dux::FInt::FromFraction(i, 100)), 0.001);
  }

  // Test |Ln|.
  // Test 9990 values in the [1, 1000] range.
  for (int i = 10; i < 10000; i++) {
    double expected = logf((float)i / 10.0f);
    AssertNearlyEqual(expected, Ln(dux::FInt::FromFraction(i, 10)), 0.01);
  }

  // Test ==, !=, < operators.
  assert(434_fx == 434_fx);
  assert(-1_fx != 1_fx);
  assert(312_fx < 1432_fx);

  // Test <<, >> operators.
  assert((111_fx << 2) == 444_fx);
  assert((111_fx >> 2) != FInt::FromInt(111 >> 2));
  assert((111_fx >> 2) == FInt::FromFraction(111, 4));
  assert((-111_fx << 2) == -444_fx);
  assert((-111_fx >> 2) == FInt::FromFraction(-111, 4));

  // Test +, -, *, and / operators.
  assert(4_fx * 2_fx == 8_fx);
  assert(4_fx * -1_fx == -4_fx);
  assert(-4_fx * 1_fx == -4_fx);
  assert(-100_fx * -2_fx == 200_fx);
  assert(40_fx / 2_fx == 20_fx);
  assert(40_fx / -2_fx == -20_fx);
  assert(-40_fx / 2_fx == -20_fx);
  assert(-40_fx / -2_fx == 20_fx);
  int32_t int32_t_val = 2;
  uint32_t uint32_t_val = 2;
  int64_t int64_t_val = 2;
  uint64_t uint64_t_val = 2;
  assert(40_fx * int32_t_val == 80_fx);
  assert(40_fx / int32_t_val == 20_fx);
  assert(40_fx * uint32_t_val == 80_fx);
  assert(40_fx / uint32_t_val == 20_fx);
  assert(40_fx * int64_t_val == 80_fx);
  assert(40_fx / int64_t_val == 20_fx);
  assert(40_fx * uint64_t_val == 80_fx);
  assert(40_fx / uint64_t_val == 20_fx);
  int32_t_val = -2;
  int64_t_val = -2;
  assert(40_fx * int32_t_val == -80_fx);
  assert(40_fx / int32_t_val == -20_fx);
  assert(40_fx * int64_t_val == -80_fx);
  assert(40_fx / int64_t_val == -20_fx);

  // Test % operator.
  assert(0_fx % 10_fx == 0_fx);
  assert(FInt::FromRawValue(1) % 10_fx == FInt::FromRawValue(1));
  assert(4_fx % 10_fx == 4_fx);
  assert(10_fx % 10_fx == 0_fx);
  assert(14_fx % 10_fx == 4_fx);
  assert(104_fx % 10_fx == 4_fx);
  // Negative values
  assert(FInt::FromRawValue(-1) % 10_fx == FInt::FromRawValue(-1));
  assert(-4_fx % 10_fx == -4_fx);
  assert(-10_fx % 10_fx == 0_fx);
  assert(-14_fx % 10_fx == -4_fx);
  assert(-104_fx % 10_fx == -4_fx);

  // Euclidean division remainder
  assert((0_fx).EuclideanDivisionRemainder(10_fx) == 0_fx);
  assert(FInt::FromRawValue(1).EuclideanDivisionRemainder(10_fx) ==
         FInt::FromRawValue(1));
  assert((4_fx).EuclideanDivisionRemainder(10_fx) == 4_fx);
  assert((10_fx).EuclideanDivisionRemainder(10_fx) == 0_fx);
  assert((14_fx).EuclideanDivisionRemainder(10_fx) == 4_fx);
  assert((104_fx).EuclideanDivisionRemainder(10_fx) == 4_fx);
  // Negative values
  assert(FInt::FromRawValue(-1).EuclideanDivisionRemainder(10_fx) ==
         FInt::FromRawValue((10_fx).raw_value_ - 1));
  assert((-1_fx).EuclideanDivisionRemainder(10_fx) == 9_fx);
  assert((-10_fx).EuclideanDivisionRemainder(10_fx) == 0_fx);
  assert((-16_fx).EuclideanDivisionRemainder(10_fx) == 4_fx);
  assert((-106_fx).EuclideanDivisionRemainder(10_fx) == 4_fx);

  // Test x^y with integers powers
  for (float x = -50; x < 50; x += 0.1) {
    for (float y = -3; y < 4; y += 1) {
      if (x > -0.1 && x < 0.1) {
        // small numbers to a negative power don't work well, e.g.
        // 0.000188^-2.000000 is supposed to be equal to 28147546,
        // but is computed as 0 because the computation is
        // turned into `1/(0.000188^2)`.
        continue;
      }
      auto expected = pow(x, y);
      auto actual = Pow(dux::FInt::FromDouble(x), dux::FInt::FromDouble(y));
      auto delta = std::max(std::abs(expected / 25.0), 0.1);
      AssertNearlyEqual(expected, actual, delta);
    }
  }

  // 100^0.25 = 3.16227766017
  AssertNearlyEqual(3.16, Pow(100_fx, dux::FInt::FromDouble(0.25)), 0.01);

  // 100^2.25 = 31622.7766017
  AssertNearlyEqual(31622.77, Pow(100_fx, dux::FInt::FromDouble(2.25)), 100);

  // 1000^0.375 = 13.3352143216
  AssertNearlyEqual(13.33, Pow(1000_fx, dux::FInt::FromDouble(0.375)), 0.1);

  // Test +=, -=, *=, and /= operators.
  FInt a = 43_fx;
  a += 1_fx;
  assert(a == 44_fx);
  a -= 2_fx;
  assert(a == 42_fx);
  a *= 3_fx;
  assert(a == FInt::FromInt(42 * 3));
  a *= -3_fx;
  assert(a == FInt::FromInt(42 * -9));
  a /= 3_fx;
  assert(a == FInt::FromInt(42 * -3));
  a /= -3_fx;
  assert(a == 42_fx);

  // Test string representation
  assert(FInt::FromInt(-277).ToString() == "-277.0");
  assert(FInt::FromInt(255).ToString() == "255.0");
  assert(FInt::FromInt(0).ToString() == "0.0");
  assert(FInt::FromInt(-0).ToString() == "0.0");
  assert(FInt::FromFloat(0.5f).ToString() == "0.2048");
  assert(FInt::FromFloat(-0.5f).ToString() == "-0.2048");
  assert(FInt::FromRawValue(2048).ToString() == "0.2048");
  assert(FInt::FromFraction(-1, 3).ToString() == "-0.1365");
  assert((1_fx / 3_fx).ToString() == "0.1365");
  assert(FInt::FromDouble(0.25).ToString() == "0.1024");
  assert((1_fx / -5).ToString() == "-0.819");
  assert((-1_fx / 6_fx).ToString() == "-0.682");
  assert((1_fx / 8).ToString() == "0.512");
  assert((FInt::FromRawValue(-4096) / 8).ToString() == "-0.512");
  assert(FInt::FromFloat(18.0f / 7).ToString() == "2.2340");
  assert((5_fx / -3_fx).ToString() == "-1.2730");
  assert((-1999_fx / 3).ToString() == "-666.1365");
  assert(Sin(FIntPi / 3_fx).ToString() == "0.3545");
  assert(Cos(4_fx * FIntPi / 3_fx).ToString() == "-0.2051");
  assert(FIntMax.ToString() == "2251799813685247.4095");
  assert(FIntMin.ToString() == "-2251799813685248.0");

  // Test constants.
  assert(FIntMax > 10000000_fx);
  assert(FIntMin < -10000000_fx);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winteger-overflow"
  assert(FIntMax.raw_value_ + 1 == FIntMin.raw_value_);
#pragma clang diagnostic pop
  AssertNearlyEqual(FIntQuarterPi.DoubleValue() * 2, FIntHalfPi);
  AssertNearlyEqual(FIntHalfPi.DoubleValue() * 2, FIntPi);
  AssertNearlyEqual(FIntPi.DoubleValue() * 2, FIntTwoPi);
  AssertNearlyEqual(3.14159, FIntPi);
}
