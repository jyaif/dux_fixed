#include "fixed_int.h"

#include <cassert>

namespace dux {

const FInt FInt::kMax = FInt::FromRawValue(0x7ffffffffffffffLL);
const FInt FInt::kMin = FInt::FromRawValue(0xffffffffffffffffLL);
const FInt FInt::kZero = FInt::FromRawValue(0LL);
const FInt FInt::kHalfPi = FInt::FromRawValue(6434LL);
const FInt FInt::kPi = FInt::FromRawValue(12868LL);
const FInt FInt::kTwoPi = FInt::FromRawValue(25736LL);

FInt FInt::FromRawValue(int64_t value) {
  FInt i;
  i.raw_value_ = value;
  return i;
}

FInt FInt::FromFraction(int32_t numerator, int32_t denominator) {
  FInt i(numerator);
  i.raw_value_ /= denominator;
  return i;
}

FInt FInt::FromDouble(double value) {
  return FInt::FromFraction(value * (1 << kShift), (1 << kShift));
}

FInt::FInt() {
  static_assert(kHalfShift * 2 == kShift, "");
  static_assert(kFractionMask == (1 << kShift) - 1, "");
  static_assert(kFractionMask == ~kIntegerMask, "");
  raw_value_ = 0;
}

FInt::FInt(int value) {
  raw_value_ = value;
  raw_value_ <<= kShift;
}

FInt::FInt(FInt const& o) { raw_value_ = o.raw_value_; }

int32_t FInt::Int32() const {
  return static_cast<int32_t>(raw_value_ >> kShift);
}

double FInt::DoubleValue() const {
  double v = raw_value_;
  v /= (1 << kShift);
  return v;
}

FInt FInt::Abs() const {
  RawType shifted = raw_value_ >> ((sizeof(raw_value_) * 8) - 1);
  return FInt::FromRawValue((raw_value_ ^ shifted) - shifted);
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
  // TODO: specialize function when value fits in 32 bits.
  assert(raw_value_ >= 0);
  RawType square_root_of_raw_value;
  if (raw_value_ > 0) {
    RawType n = (raw_value_ >> 1) + 1;
    RawType n1 = (n + (raw_value_ / n)) >> 1;
    while (n1 < n) {
      n = n1;
      n1 = (n + (raw_value_ / n)) >> 1;
    }
    square_root_of_raw_value = n1;
  } else {
    square_root_of_raw_value = 0;
  }
  FInt i;
  i.raw_value_ = square_root_of_raw_value << kHalfShift;
  return i;
}

FInt FInt::operator+(const FInt& o) const {
  FInt i;
  i.raw_value_ = raw_value_ + o.raw_value_;
  return i;
}

FInt FInt::operator-(const FInt& o) const {
  FInt i;
  i.raw_value_ = raw_value_ - o.raw_value_;
  return i;
}

FInt FInt::operator*(const FInt& o) const {
  FInt i;
  i.raw_value_ = (raw_value_ * o.raw_value_) >> kShift;
  return i;
}

FInt FInt::operator/(const FInt& o) const {
  FInt i;
  assert(o.raw_value_ != 0);
  i.raw_value_ = (raw_value_ << kShift) / o.raw_value_;
  return i;
}

FInt FInt::operator-() const {
  FInt i;
  i.raw_value_ = -raw_value_;
  return i;
}

FInt FInt::operator++() {
  raw_value_ += (1 << kShift);
  return *this;
}

FInt FInt::operator--() {
  raw_value_ -= (1 << kShift);
  return *this;
}

void FInt::operator+=(const FInt& o) { raw_value_ += o.raw_value_; }

void FInt::operator-=(const FInt& o) { raw_value_ -= o.raw_value_; }

void FInt::operator*=(const FInt& o) {
  raw_value_ *= o.raw_value_;
  raw_value_ >>= kShift;
}

void FInt::operator/=(const FInt& o) {
  raw_value_ <<= kShift;
  assert(o.raw_value_ > 0);
  raw_value_ /= o.raw_value_;
}

void FInt::operator>>=(int shift) { raw_value_ >>= shift; }

void FInt::operator<<=(int shift) { raw_value_ <<= shift; }

bool FInt::operator!=(const FInt& o) const {
  return raw_value_ != o.raw_value_;
}

bool FInt::operator==(const FInt& o) const {
  return raw_value_ == o.raw_value_;
}

bool FInt::operator<(const FInt& o) const {
  return raw_value_ < o.raw_value_;
}

bool FInt::operator<=(const FInt& o) const {
  return raw_value_ <= o.raw_value_;
}

bool FInt::operator>(const FInt& o) const {
  return raw_value_ > o.raw_value_;
}

bool FInt::operator>=(const FInt& o) const {
  return raw_value_ >= o.raw_value_;
}

}  // namespace dux
