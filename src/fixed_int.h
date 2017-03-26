#ifndef DUX_FIXED_FIXED_INT_H_
#define DUX_FIXED_FIXED_INT_H_

#include <cstdint>

namespace dux {

// Class encapsulating 52-12 fixed point numbers.
class FInt {
 public:
  
  // Initializes to 0.
  FInt();

  // Sets the integral part of the fixed point number to |value|.
  explicit FInt(int32_t value);
  
  // Copy constructor.
  FInt(FInt const& value);

  // Creates a fixed point number from the underlying representation.
  static FInt FromRawValue(int64_t value);

  // Creates a fixed point number from the result of the operation
  // numerator/denominator.
  static FInt FromFraction(int32_t numerator, int32_t denominator);

  // Creates a fixed point number from |value|.
  static FInt FromDouble(double value);

  // Returns the integral part of the fixed point number.
  int32_t Int32() const;

  // Returns an approximation as a double of the fixed point number.
  double DoubleValue() const;

  // Returns the absolute value of |this| object.
  FInt Abs() const;

  // Return the largest integral fixed point number less than or equal to |this|
  // object.
  FInt Floor() const;

  // Return the smallest integral fixed point number greater than or equal to
  // |this| object.
  FInt Ceil() const;

  // Returns the integral value nearest by rounding half-way cases away from
  // zero.
  FInt Round() const;

  // Returns the non-negative square root of |this| object.
  // Asserts if |this| is less than zero.
  FInt Sqrt() const;

  FInt operator+(const FInt&) const;
  FInt operator-(const FInt&) const;
  FInt operator*(const FInt&) const;
  FInt operator/(const FInt&) const;
  FInt operator-() const;

  FInt operator++();
  FInt operator--();

  void operator+=(const FInt&);
  void operator-=(const FInt&);
  void operator*=(const FInt&);
  void operator/=(const FInt&);
  void operator>>=(int shift);
  void operator<<=(int shift);

  bool operator!=(const FInt&) const;
  bool operator==(const FInt&) const;
  bool operator<(const FInt&) const;
  bool operator<=(const FInt&) const;
  bool operator>(const FInt&) const;
  bool operator>=(const FInt&) const;

  using RawType = int64_t;
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
  static const FInt kPi;
  static const FInt kTwoPi;
};
  
}  // namespace dux

#endif  // DUX_FIXED_FIXED_INT_H_
