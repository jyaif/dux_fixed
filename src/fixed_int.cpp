#include "fixed_int.h"

#include <cassert>

namespace dux {

const FInt FInt::kMax = FInt::FromRawValue(0x7fffffffffffffffLL);
const FInt FInt::kMin = FInt::FromRawValue(0x8000000000000000LL);
const FInt FInt::kZero = FInt::FromRawValue(0LL);
const FInt FInt::kHalfPi = FInt::FromRawValue(6434LL);
const FInt FInt::kQuarterPi = FInt::FromRawValue(3217LL);
const FInt FInt::kPi = FInt::FromRawValue(12868LL);
const FInt FInt::kTwoPi = FInt::FromRawValue(25736LL);

double FInt::DoubleValue() const {
  double v = raw_value_;
  v /= (1 << kShift);
  return v;
}

float FInt::FloatValue() const {
  float v = raw_value_;
  v /= (1 << kShift);
  return v;
}

FInt FInt::Floor() const {
  return FInt::FromRawValue(raw_value_ & kIntegerMask);
}

FInt FInt::Ceil() const {
  return -FInt::FromRawValue(-raw_value_ & kIntegerMask);
}

FInt FInt::Round() const {
  bool high_bit_of_fraction_is_one = (raw_value_ & kHighBitOfFraction) > 0;
  if (high_bit_of_fraction_is_one) {
    return Ceil();
  } else {
    return Floor();
  }
}

FInt FInt::Sqrt() const {
  assert(raw_value_ >= 0);
  if (raw_value_ <= 0)
    return FInt(0);

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
    return FInt(square_root_of_raw_value << kHalfShift);
  } else {
    RawType n = (raw_value_ >> 1) + 1;
    RawType n1 = (n + (raw_value_ / n)) >> 1;
    while (n1 < n) {
      n = n1;
      n1 = (n + (raw_value_ / n)) >> 1;
    }
    RawType square_root_of_raw_value = n1;
    return FInt::FromRawValue(square_root_of_raw_value << kHalfShift);
  }
}

}  // namespace dux

std::ostream& operator <<(std::ostream& stream, const dux::FInt& fint) {
  stream << fint.raw_value_;
  return stream;
}
