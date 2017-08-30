#include "test_fixed_rand.h"

#include <cassert>

#include "dux_fixed.h"

using namespace dux;

void TestFRandGenerator() {
	FRandGenerator rg(42);

	// Tests that the same sequence is always produced, even with different compilers/libraries.
	for (int i = 0; i < 5000; i++) {
		rg.RandInt();
	}
	assert(rg.RandInt() == -740800590);


	// Tests that the clamping works.
	for (int i = 0; i < 100; i++) {
		dux::FInt v = rg.RandFInt(dux::FInt(1986), dux::FInt(1987));
		assert(v >= dux::FInt(1986));
		assert(v <= dux::FInt(1987));
	}
}
