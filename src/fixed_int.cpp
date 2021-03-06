#include "fixed_int.h"

#include <cassert>
#include <limits>

namespace dux {

const FInt FInt::kMax = FInt::FromRawValue(std::numeric_limits<RawType>::max());
const FInt FInt::kMin = FInt::FromRawValue(std::numeric_limits<RawType>::min());
const FInt FInt::kZero = FInt::FromRawValue(0LL);
const FInt FInt::kHalfPi = FInt::FromRawValue(6434LL);
const FInt FInt::kQuarterPi = FInt::FromRawValue(3217LL);
const FInt FInt::kPi = FInt::FromRawValue(12868LL);
const FInt FInt::kTwoPi = FInt::FromRawValue(25736LL);

double FInt::DoubleValue() const {
  double v = static_cast<double>(raw_value_);
  v /= (1 << kShift);
  return v;
}

float FInt::FloatValue() const {
  float v = static_cast<float>(raw_value_);
  v /= (1 << kShift);
  return v;
}

FInt FInt::Sqrt() const {
  assert(raw_value_ >= 0);
  if (raw_value_ <= 0) {
    return FInt(0);
  }

  FInt result;
  // Specialisation for when the value fits in a int32_t.
  if (raw_value_ < 0x7FFFFFFF) {
    int32_t value = static_cast<int32_t>(raw_value_);
    int32_t n = (value >> 1) + 1;
    int32_t n1 = (n + (value / n)) >> 1;
    while (n1 < n) {
      n = n1;
      n1 = (n + (value / n)) >> 1;
    }
    RawType square_root_of_raw_value = n1;
    result = FInt::FromRawValue(square_root_of_raw_value << kHalfShift);
  } else {
    RawType n = (raw_value_ >> 1) + 1;
    RawType n1 = (n + (raw_value_ / n)) >> 1;
    while (n1 < n) {
      n = n1;
      n1 = (n + (raw_value_ / n)) >> 1;
    }
    RawType square_root_of_raw_value = n1;
    result = FInt::FromRawValue(square_root_of_raw_value << kHalfShift);
  }
  return result;
}

FInt FInt::EuclideanDivisionRemainder(dux::FInt upper_bound) const {
  assert(upper_bound > 0_fx);
  if (raw_value_ >= 0) {
    return dux::FInt(raw_value_ % upper_bound.raw_value_);
  } else {
    auto raw =
        (upper_bound.raw_value_ + ((raw_value_ + 1) % upper_bound.raw_value_)) -
        1;
    return dux::FInt(raw);
  }
}

}  // namespace dux

std::ostream& operator<<(std::ostream& stream, const dux::FInt& fint) {
  stream << fint.raw_value_ << "(" << fint.DoubleValue() << ")";
  return stream;
}
