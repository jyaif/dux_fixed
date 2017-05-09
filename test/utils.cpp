#include <cassert>
#include <cmath>

#include "utils.h"

namespace dux_test_utils {

void AssertNearlyEqual(double expected, dux::FInt actual) {
  double delta = fabs(expected - actual.DoubleValue());
  if (delta >= 0.02) {
  	printf("expected:%f, actual:%f\n", expected, actual.DoubleValue());
  	assert(false);
  }
}

}  // namespace dux_test_utils