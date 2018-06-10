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
    rg.RandInt();
  }

  FRandGenerator rg2 = rg;
  assert(rg.RandInt() == -740800590);
  assert(rg2.RandInt() == -740800590);

  // Tests that the clamping works.
  for (int i = 0; i < 100; i++) {
    dux::FInt v = rg.RandFInt(dux::FInt(1986), dux::FInt(1987));
    assert(v >= dux::FInt(1986));
    assert(v <= dux::FInt(1987));
  }

  // Tests that setting the seed works.
  rg.SetSeed(2019);
  FRandGenerator rg3(2019);
  assert(rg.RandInt() == rg3.RandInt());
}
