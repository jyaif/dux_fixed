#include "test_fixed_rand.h"

#include <cassert>

#include "dux_fixed.h"

using namespace dux;

void TestFRandGenerator() {
  FRandGenerator rg(42);

  // Tests that the same sequence is always produced, even with different
  // compilers/libraries.
  // Also test that copy works.
  for (int i = 0; i < 5000; i++) {
    rg.RandUInt32();
  }

  FRandGenerator rg2 = rg;
  assert(rg.RandUInt32() == 1301828430);
  assert(rg2.RandUInt32() == 1301828430);

  // Tests that the clamping works.
  for (int i = 0; i < 100; i++) {
    dux::FInt v =
        rg.RandFInt(dux::FInt::FromInt(1986), dux::FInt::FromInt(1987));
    assert(v >= dux::FInt::FromInt(1986));
    assert(v <= dux::FInt::FromInt(1987));

    v = rg.RandFInt(dux::FInt::FromInt(1986), dux::FInt::FromInt(1986));
    assert(v == dux::FInt::FromInt(1986));
  }

  // Tests that setting the seed works.
  rg.SetSeed(2019);
  FRandGenerator rg3(2019);
  assert(rg.RandUInt32() == rg3.RandUInt32());

  // Tests that float rand works.
  for (int i = 0; i < 1000; i++) {
    float v = rg.RandFloat(10);
    assert(v >= 0);
    assert(v <= 10);
  }
  while (rg.RandFloat(10) < 9.999) {
  }
  while (rg.RandFloat(10) > 0.001) {
  }
}
