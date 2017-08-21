#include <cassert>
#include <cmath>

#include "utils.h"

namespace dux_test_utils {

void AssertNearlyEqual(double expected, dux::FInt actual) {
  double delta = fabs(expected - actual.DoubleValue());
  double threshold = 0.02;
  if (expected > 1000) {
    threshold = expected * 0.002;
  }
  if (delta >= threshold) {
    printf("expected:%f, actual:%f\n", expected, actual.DoubleValue());
    assert(false);
  }
}

}  // namespace dux_test_utils
