#include <cassert>
#include <cmath>
#include <random>

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

namespace {

std::linear_congruential_engine<uint32_t, 48271, 0, 2147483647> rng;

dux::FInt RandFInt(dux::FInt min, dux::FInt max) {
  assert(max >= min);
  std::uniform_int_distribution<int64_t> uid(min.raw_value_, max.raw_value_);
  return dux::FInt::FromRawValue(uid(rng));
}

}  // namespace

dux::FVec2 RandFVec2(dux::FInt const minX,
                     dux::FInt const maxX,
                     dux::FInt const minY,
                     dux::FInt const maxY) {
  return {RandFInt(minX, maxX), RandFInt(minY, maxY)};
}

}  // namespace dux_test_utils
