#include <cassert>
#include <cmath>

#include "utils.h"

namespace dux_test_utils {

void AssertNearlyEqual(double expected, dux::FInt actual, double threshold) {
  double delta = fabs(expected - actual.DoubleValue());
  if (delta >= threshold) {
    printf("expected:%f, actual:%f\n", expected, actual.DoubleValue());
    printf("diff:%f, max threshold:%f\n", delta, threshold);

    assert(false);
  }
}

}  // namespace dux_test_utils
