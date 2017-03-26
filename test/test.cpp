#include <cassert>
#include <cstdio>
#include <cstdlib>

#include "fixed_int.h"

using namespace dux;

int main(int argc, char *argv[]) {
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
  
  printf("tests successfully passed\n");
  return EXIT_SUCCESS;
}
