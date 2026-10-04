#ifndef DUX_FILED_SRC_FIXED_VEC2_H_
#define DUX_FILED_SRC_FIXED_VEC2_H_

#include <sstream>

#include "fixed_int.h"

namespace dux {

class FVec2 {
 public:
  FInt x_;
  FInt y_;

  constexpr FVec2() = default;
  constexpr FVec2(FVec2 const& v) = default;
  constexpr FVec2(FInt x, FInt y) : x_(x), y_(y) {}
  constexpr FVec2(int x, int y) : x_(FInt::FromInt(x)), y_(FInt::FromInt(y)) {}

  constexpr void Init(FInt x, FInt y) {
    x_ = x;
    y_ = y;
  }
  static FVec2 FromAngle(FInt angle, FInt radius);
  static FVec2 FromAngle(FInt angle);

  constexpr FInt SquareLength() const { return x_ * x_ + y_ * y_; }
  constexpr FInt SquareLengthFrom(FVec2 const& c) const {
    FInt dx = c.x_ - x_;
    FInt dy = c.y_ - y_;
    return dx * dx + dy * dy;
  }
  FInt Length();
  void Normalize(bool& success);
  void Normalize(bool& success, FInt newLength);
  constexpr FInt DotProduct(const FVec2& v) const {
    return (x_ * v.x_) + (y_ * v.y_);
  }
  // Returns a value in the range [0, 2*pi[.
  FInt Angle() const;
  constexpr void Rotate90Deg() {
    FInt x = x_;
    x_ = -y_;
    y_ = x;
  }
  void Rotate(dux::FInt angle);

  constexpr FVec2 operator+(const FVec2& a) const {
    return FVec2(a.x_ + x_, a.y_ + y_);
  }
  constexpr FVec2 operator-(const FVec2& a) const {
    return FVec2(x_ - a.x_, y_ - a.y_);
  }
  constexpr FInt operator*(const FVec2& a) const {
    return x_ * a.x_ + y_ * a.y_;
  }
  constexpr FVec2 operator-() const { return FVec2(-x_, -y_); }
  constexpr FVec2 operator*(FInt v) const { return FVec2(x_ * v, y_ * v); }

  constexpr void operator+=(const FVec2& a) {
    x_ += a.x_;
    y_ += a.y_;
  }
  constexpr void operator-=(const FVec2& a) {
    x_ -= a.x_;
    y_ -= a.y_;
  }
  constexpr void operator*=(FInt v) {
    x_ *= v;
    y_ *= v;
  }

  constexpr FVec2 operator*(const int32_t s) const {
    return FVec2(x_ * s, y_ * s);
  }
  constexpr FVec2 operator/(const int32_t s) const {
    return FVec2(x_ / s, y_ / s);
  }
  constexpr void operator*=(const int32_t s) {
    x_ *= s;
    y_ *= s;
  }
  constexpr void operator/=(const int32_t s) {
    x_ /= s;
    y_ /= s;
  }

  inline bool operator==(const FVec2& other) const {
    return x_ == other.x_ && y_ == other.y_;
  }
  inline bool operator!=(const FVec2& other) const {
    return x_ != other.x_ || y_ != other.y_;
  }

  // Returns true if the inequality is true for both |x_| and |y_|.
  inline bool operator>=(const FVec2& other) const {
    return x_ >= other.x_ && y_ >= other.y_;
  }
  inline bool operator<=(const FVec2& other) const {
    return x_ <= other.x_ && y_ <= other.y_;
  }
  inline bool operator>(const FVec2& other) const {
    return x_ > other.x_ && y_ > other.y_;
  }
  inline bool operator<(const FVec2& other) const {
    return x_ < other.x_ && y_ < other.y_;
  }
};

}  // namespace dux

std::ostream& operator<<(std::ostream& stream, const dux::FVec2& fvec2);

#endif  // DUX_FILED_SRC_FIXED_VEC2_H_
