#ifndef DUX_FIXED_SRC_FIXED_INT_H_
#define DUX_FIXED_SRC_FIXED_INT_H_

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <sstream>
#include <type_traits>

namespace dux {

// Class encapsulating 52-12 fixed point numbers.
class FInt {
 public:
  using RawType = int64_t;

  // Initializes to 0.
  constexpr FInt() : raw_value_(0) {
    static_assert(kHalfShift * 2 == kShift);
    static_assert(kFractionMask == (1 << kShift) - 1);
    static_assert(kFractionMask == ~kIntegerMask);
  }

  // Copy constructor.
  constexpr FInt(FInt const& o) = default;

  // Copy assignment operator.
  constexpr FInt& operator=(FInt const& o) = default;

  // Creates a fixed point number from an integer.
  [[nodiscard]] constexpr static FInt FromInt(int32_t value) {
    return FInt(static_cast<RawType>(value) * (1 << kShift));
  }

  // Creates a fixed point number from the underlying representation.
  [[nodiscard]] constexpr static FInt FromRawValue(RawType raw_value) {
    return FInt(raw_value);
  }

  // Creates a fixed point number from the result of the operation
  // numerator/denominator.
  [[nodiscard]] constexpr static FInt FromFraction(int32_t numerator,
                                                   int32_t denominator) {
    assert(denominator != 0);
    FInt i = FromInt(numerator);
    i.raw_value_ /= denominator;
    return i;
  }

  // Creates a fixed point number from a double.
  [[nodiscard]] constexpr static FInt FromDouble(double value) {
    RawType raw_value = static_cast<RawType>(value * (1 << FInt::kShift));
    return FInt::FromRawValue(raw_value);
  }

  // Creates a fixed point number from a float.
  [[nodiscard]] constexpr static FInt FromFloat(float value) {
    RawType raw_value = static_cast<RawType>(value * (1 << FInt::kShift));
    return FInt::FromRawValue(raw_value);
  }

  // Returns the integral part of the fixed point number.
  [[nodiscard]] constexpr int32_t Int32() const {
    return static_cast<int32_t>(raw_value_ / (1 << kShift));
  }

  // Returns the integral part of the fixed point number.
  [[nodiscard]] constexpr int64_t Int64() const {
    return raw_value_ / (1 << kShift);
  }

  // Returns an approximation as a double of the fixed point number.
  [[nodiscard]] double DoubleValue() const;

  // Returns an approximation as a float of the fixed point number.
  [[nodiscard]] float FloatValue() const;

  // Returns the absolute value of |this| object.
  [[nodiscard]] constexpr FInt Abs() const {
    if (raw_value_ > 0) {
      return FInt::FromRawValue(raw_value_);
    }
    return FInt::FromRawValue(-raw_value_);
  }

  // Return the largest integral fixed point number less than or equal to |this|
  // object.
  [[nodiscard]] constexpr FInt Floor() const {
    return FInt::FromRawValue(raw_value_ & kIntegerMask);
  }

  // Return the smallest integral fixed point number greater than or equal to
  // |this| object.
  [[nodiscard]] constexpr FInt Ceil() const {
    return -FInt::FromRawValue(-raw_value_ & kIntegerMask);
  }

  // Returns the integral value nearest by rounding half-way cases away from
  // zero.
  [[nodiscard]] constexpr FInt Round() const {
    bool high_bit_of_fraction_is_one = (raw_value_ & kHighBitOfFraction) > 0;
    if (high_bit_of_fraction_is_one) {
      return Ceil();
    }
    return Floor();
  }

  // Returns the fractional part.
  [[nodiscard]] constexpr FInt Frac() const {
    return FInt::FromRawValue(raw_value_ & kFractionMask);
  }

  // Returns whether |other| is of the same sign as |this|.
  // Considers 0 as being a positive number.
  [[nodiscard]] constexpr bool IsSameSignAs(const FInt& other) const {
    return (raw_value_ ^ other.raw_value_) >= 0;
  }

  // Returns the non-negative square root of |this| object.
  // Asserts if |this| is less than zero.
  [[nodiscard]] FInt Sqrt() const;

  // Returns a value that is always positive.
  // |upper_bound| must be greater than 0.
  [[nodiscard]] FInt EuclideanDivisionRemainder(dux::FInt upper_bound) const;

  // Returns the string representation of a fixedpoint value
  // The returned string is formatted like a fixedpoint literal (-0.2048), not a
  // float (-0.5)
  [[nodiscard]] std::string ToString() const;

  constexpr FInt operator+(const FInt& o) const {
    return FInt::FromRawValue(raw_value_ + o.raw_value_);
  }
  constexpr FInt operator-(const FInt& o) const {
    return FInt::FromRawValue(raw_value_ - o.raw_value_);
  }
  constexpr FInt operator*(const FInt& o) const {
    return FInt::FromRawValue((raw_value_ * o.raw_value_) / (1 << kShift));
  }
  constexpr FInt operator/(const FInt& o) const {
    return FInt::FromRawValue((raw_value_ * (1 << kShift)) / o.raw_value_);
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
  constexpr void operator*=(const FInt& o) {
    raw_value_ *= o.raw_value_;
    raw_value_ /= (1 << kShift);
  }
  constexpr void operator/=(const FInt& o) {
    raw_value_ *= (1 << kShift);
    assert(o.raw_value_ != 0);
    raw_value_ /= o.raw_value_;
  }
  constexpr void operator%=(const FInt& o) {
    assert(o.raw_value_ != 0);
    raw_value_ %= o.raw_value_;
  }
  constexpr void operator>>=(int shift) { raw_value_ /= (1 << shift); }
  constexpr void operator<<=(int shift) { raw_value_ *= (1 << shift); }

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
    static_assert(std::is_integral_v<T>, "Integer required.");
    return dux::FInt::FromRawValue(raw_value_ * static_cast<RawType>(v));
  }

  template <typename T>
  constexpr FInt operator/(const T v) const {
    static_assert(std::is_integral_v<T>, "Integer required.");
    return dux::FInt::FromRawValue(raw_value_ / static_cast<RawType>(v));
  }

  template <typename T>
  constexpr void operator*=(const T v) {
    static_assert(std::is_integral_v<T>, "Integer required.");
    raw_value_ *= v;
  }

  template <typename T>
  constexpr void operator/=(const T v) {
    static_assert(std::is_integral_v<T>, "Integer required.");
    raw_value_ /= v;
  }

  RawType raw_value_;
  static constexpr int kShift = 12;
  static constexpr RawType kFractionMask = (1 << kShift) - 1;
  static constexpr RawType kHighBitOfFraction = (1 << (kShift - 1));
  static constexpr RawType kIntegerMask = ~((1 << kShift) - 1);
  static constexpr int kHalfShift = kShift / 2;

 private:
  // Private. Use |FromRawValue| instead.
  constexpr explicit FInt(RawType raw_value) : raw_value_(raw_value) {}
};

// Returns x^y
// Disclaimer: when `y` is not an integer, the precision is low.
[[nodiscard]] FInt Pow(FInt x, FInt y);

constexpr FInt FIntMax =
    FInt::FromRawValue(std::numeric_limits<dux::FInt::RawType>::max());
constexpr FInt FIntMin =
    FInt::FromRawValue(std::numeric_limits<dux::FInt::RawType>::min());
constexpr FInt FIntHalfPi = FInt::FromRawValue(6434LL);
constexpr FInt FIntQuarterPi = FInt::FromRawValue(3217LL);
constexpr FInt FIntPi = FInt::FromRawValue(12868LL);
constexpr FInt FIntTwoPi = FInt::FromRawValue(25736LL);

}  // namespace dux

// A shorthand for |dux::FInt::FromInt|:
// `dux::FInt::FromInt(42)`
// becomes
// `42_fx`.
constexpr dux::FInt operator""_fx(unsigned long long int v) {
  return dux::FInt::FromRawValue(v * (1 << dux::FInt::kShift));
}

std::ostream& operator<<(std::ostream& stream, const dux::FInt& fint);

#endif  // DUX_FIXED_SRC_FIXED_INT_H_
