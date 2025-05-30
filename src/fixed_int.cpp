#include "fixed_int.h"

#include <cassert>

namespace dux {

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

[[nodiscard]] FInt Pow(FInt const x, FInt const y) {
  if (y.raw_value_ == 0) {
    return 1_fx;  // x^0 = 1
  }
  if (x.raw_value_ == 0) {
    return 0_fx;  // 0^y = 0  if y!=0
  }

  bool y_is_negative = y < 0_fx;
  FInt const positive_y = y_is_negative ? -y : y;
  int32_t fractional_y = positive_y.raw_value_ & dux::FInt::kFractionMask;
  int64_t integral_y = positive_y.raw_value_ & dux::FInt::kIntegerMask;
  dux::FInt result = 1_fx;
  dux::FInt squared = x;
  if (integral_y) {
    while (integral_y > dux::FInt::kFractionMask) {
      if ((integral_y & (1 << dux::FInt::kShift)) != 0) {
        result *= squared;
      }
      squared = (squared * squared);
      integral_y >>= 1;
    }
  }
  auto square_rooted = x;
  while (fractional_y != 0) {
    square_rooted = square_rooted.Sqrt();
    if (square_rooted == 1_fx) {
      break;
    }
    if (fractional_y & dux::FInt::kHighBitOfFraction) {
      result *= square_rooted;
    }
    fractional_y <<= 1;
    fractional_y &= dux::FInt::kFractionMask;
  }
  if (y_is_negative) {
    return 1_fx / result;
  }
  return result;
}

[[nodiscard]] std::string FInt::ToString() const {
  return (raw_value_ < 0 ? "-" : "") + std::to_string(Int64()) + "." +
         std::to_string(Frac().raw_value_);
}

}  // namespace dux

std::ostream& operator<<(std::ostream& stream, const dux::FInt& fint) {
  stream << fint.raw_value_ << "(" << fint.DoubleValue() << ")";
  return stream;
}
