#ifndef DUX_FILED_TEST_TEST_UTILS_H_
#define DUX_FILED_TEST_TEST_UTILS_H_

#include "dux_fixed.h"

namespace dux_test_utils {

void AssertNearlyEqual(double expected,
                       dux::FInt actual,
                       double threshold = 0.02);

}  // namespace dux_test_utils

#endif  // DUX_FILED_TEST_TEST_UTILS_H_
