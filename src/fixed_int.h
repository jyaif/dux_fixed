#ifndef DUX_FIXED_SRC_FIXED_INT_H_
#define DUX_FIXED_SRC_FIXED_INT_H_

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <type_traits>

namespace dux {

#define DUX_NO_UB __attribute__((no_sanitize("shift")))

// Class encapsulating 52-12 fixed point numbers.
class FInt {
 public:
  using RawType = int64_t;

  // Initializes to 0.
  constexpr FInt() : raw_value_(0) {
    static_assert(kHalfShift * 2 == kShift, "");
    static_assert(kFractionMask == (1 << kShift) - 1, "");
    static_assert(kFractionMask == ~kIntegerMask, "");
  }

  // Sets the integral part of the fixed point number to |value|.
  constexpr explicit FInt(int32_t value) DUX_NO_UB
      : raw_value_(static_cast<RawType>(value) << kShift) {}

  // Copy constructor.
  constexpr FInt(FInt const& o) : raw_value_(o.raw_value_) {}

  // Copy assignment operator.
  constexpr FInt& operator=(FInt const& o) {
    raw_value_ = o.raw_value_;
    return *this;
  }

  // Creates a fixed point number from the underlying representation.
  constexpr static FInt FromRawValue(RawType raw_value) {
    return FInt(raw_value);
  }

  // Creates a fixed point number from the result of the operation
  // numerator/denominator.
  constexpr static FInt FromFraction(int32_t numerator, int32_t denominator) {
    assert(denominator != 0);
    FInt i(numerator);
    i.raw_value_ /= denominator;
    return i;
  }

  // Creates a fixed point number from a double.
  constexpr static FInt FromDouble(double value) {
    return FInt::FromFraction(static_cast<int32_t>(value * (1 << FInt::kShift)),
                              (1 << FInt::kShift));
  }

  // Returns the integral part of the fixed point number.
  constexpr int32_t Int32() const DUX_NO_UB {
    return static_cast<int32_t>(raw_value_ >> kShift);
  }

  // Returns an approximation as a double of the fixed point number.
  double DoubleValue() const;

  // Returns an approximation as a float of the fixed point number.
  float FloatValue() const;

  // Returns the absolute value of |this| object.
  constexpr FInt Abs() const {
    if (raw_value_ > 0) {
      return FInt::FromRawValue(raw_value_);
    }
    return FInt::FromRawValue(-raw_value_);
  }

  // Return the largest integral fixed point number less than or equal to |this|
  // object.
  constexpr FInt Floor() const {
    return FInt::FromRawValue(raw_value_ & kIntegerMask);
  }

  // Return the smallest integral fixed point number greater than or equal to
  // |this| object.
  constexpr FInt Ceil() const {
    return -FInt::FromRawValue(-raw_value_ & kIntegerMask);
  }

  // Returns the integral value nearest by rounding half-way cases away from
  // zero.
  constexpr FInt Round() const {
    bool high_bit_of_fraction_is_one = (raw_value_ & kHighBitOfFraction) > 0;
    if (high_bit_of_fraction_is_one) {
      return Ceil();
    } else {
      return Floor();
    }
  }

  // Returns whether |other| is of the same sign as |this|.
  // Considers 0 as being a positive number.
  constexpr bool IsSameSignAs(const FInt& other) const {
    return (raw_value_ ^ other.raw_value_) >= 0;
  }

  // Returns the non-negative square root of |this| object.
  // Asserts if |this| is less than zero.
  FInt Sqrt() const;

  constexpr FInt operator+(const FInt& o) const {
    return FInt::FromRawValue(raw_value_ + o.raw_value_);
  }
  constexpr FInt operator-(const FInt& o) const {
    return FInt::FromRawValue(raw_value_ - o.raw_value_);
  }
  constexpr FInt operator*(const FInt& o) const DUX_NO_UB {
    return FInt::FromRawValue((raw_value_ * o.raw_value_) >> kShift);
  }
  constexpr FInt operator/(const FInt& o) const DUX_NO_UB {
    return FInt::FromRawValue((raw_value_ << kShift) / o.raw_value_);
  }
  constexpr FInt operator%(const FInt& o) const {
    assert(o.raw_value_ != 0);
    return dux::FInt::FromRawValue(raw_value_ % o.raw_value_);
  }
  constexpr FInt operator-() const {
    return dux::FInt::FromRawValue(-raw_value_);
  }

  constexpr FInt operator++() {
    raw_value_ += (1 << kShift);
    return *this;
  }
  constexpr FInt operator--() {
    raw_value_ -= (1 << kShift);
    return *this;
  }

  constexpr void operator+=(const FInt& o) { raw_value_ += o.raw_value_; }
  constexpr void operator-=(const FInt& o) { raw_value_ -= o.raw_value_; }
  constexpr void operator*=(const FInt& o) DUX_NO_UB {
    raw_value_ *= o.raw_value_;
    raw_value_ >>= kShift;
  }
  constexpr void operator/=(const FInt& o) DUX_NO_UB {
    raw_value_ <<= kShift;
    assert(o.raw_value_ > 0);
    raw_value_ /= o.raw_value_;
  }
  constexpr void operator%=(const FInt& o) {
    assert(o.raw_value_ > 0);
    raw_value_ %= o.raw_value_;
  }
  constexpr void operator>>=(int shift) DUX_NO_UB { raw_value_ >>= shift; }
  constexpr void operator<<=(int shift) DUX_NO_UB { raw_value_ <<= shift; }

  constexpr bool operator!=(const FInt& o) const {
    return raw_value_ != o.raw_value_;
  }
  constexpr bool operator==(const FInt& o) const {
    return raw_value_ == o.raw_value_;
  }
  constexpr bool operator<(const FInt& o) const {
    return raw_value_ < o.raw_value_;
  }
  constexpr bool operator<=(const FInt& o) const {
    return raw_value_ <= o.raw_value_;
  }
  constexpr bool operator>(const FInt& o) const {
    return raw_value_ > o.raw_value_;
  }
  constexpr bool operator>=(const FInt& o) const {
    return raw_value_ >= o.raw_value_;
  }

  constexpr FInt operator>>(int shift) const {
    return dux::FInt::FromRawValue(raw_value_ >> shift);
  }
  constexpr FInt operator<<(int shift) const {
    return dux::FInt::FromRawValue(raw_value_ << shift);
  }

  template <typename T>
  constexpr FInt operator*(const T v) const {
    static_assert(std::is_integral<T>::value, "Integer required.");
    return dux::FInt::FromRawValue(raw_value_ * static_cast<RawType>(v));
  }

  template <typename T>
  constexpr FInt operator/(const T v) const {
    static_assert(std::is_integral<T>::value, "Integer required.");
    return dux::FInt::FromRawValue(raw_value_ / static_cast<RawType>(v));
  }

  template <typename T>
  constexpr void operator*=(const T v) {
    static_assert(std::is_integral<T>::value, "Integer required.");
    raw_value_ *= v;
  }

  template <typename T>
  constexpr void operator/=(const T v) {
    static_assert(std::is_integral<T>::value, "Integer required.");
    raw_value_ /= v;
  }

  RawType raw_value_;
  static const int kShift = 12;
  static const RawType kFractionMask = (1 << kShift) - 1;
  static const RawType kHighBitOfFraction = (1 << (kShift - 1));
  static const RawType kIntegerMask = ~((1 << kShift) - 1);
  static const int kHalfShift = kShift / 2;
  static const FInt kMax;
  static const FInt kMin;
  static const FInt kZero;
  static const FInt kHalfPi;
  static const FInt kQuarterPi;
  static const FInt kPi;
  static const FInt kTwoPi;

 private:
  // Private. Use |FromRawValue| instead.
  constexpr explicit FInt(RawType raw_value) : raw_value_(raw_value) {}
};

}  // namespace dux

std::ostream& operator<<(std::ostream& stream, const dux::FInt& fint);

#endif  // DUX_FIXED_SRC_FIXED_INT_H_
