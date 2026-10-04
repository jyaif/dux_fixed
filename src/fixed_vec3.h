#ifndef DUX_FILED_SRC_FIXED_VEC3_H_
#define DUX_FILED_SRC_FIXED_VEC3_H_

#include "fixed_int.h"

namespace dux {

class FVec3 {
 public:
  FInt x_;
  FInt y_;
  FInt z_;

  constexpr FVec3() = default;
  constexpr FVec3(FVec3 const& v) = default;
  constexpr FVec3(FInt x, FInt y, FInt z) : x_(x), y_(y), z_(z) {}
  constexpr FVec3(int x, int y, int z)
      : x_(FInt::FromInt(x)), y_(FInt::FromInt(y)), z_(FInt::FromInt(z)) {}

  constexpr void Init(FInt x, FInt y, FInt z) {
    x_ = x;
    y_ = y;
    z_ = z;
  }

  constexpr FInt SquareLength() const { return x_ * x_ + y_ * y_ + z_ * z_; }
  constexpr FInt SquareLengthFrom(FVec3 const& c) const {
    FInt dx = c.x_ - x_;
    FInt dy = c.y_ - y_;
    FInt dz = c.z_ - z_;
    return dx * dx + dy * dy + dz * dz;
  }
  FInt Length();
  void Normalize(bool& success);
  void Normalize(bool& success, FInt newLength);

  constexpr FVec3 operator+(const FVec3& a) const {
    return FVec3(a.x_ + x_, a.y_ + y_, a.z_ + z_);
  }
  constexpr FVec3 operator-(const FVec3& a) const {
    return FVec3(x_ - a.x_, y_ - a.y_, z_ - a.z_);
  }
  constexpr FInt operator*(const FVec3& a) const {
    return x_ * a.x_ + y_ * a.y_ + z_ * a.z_;
  }
  constexpr FVec3 operator-() const { return FVec3(-x_, -y_, -z_); }
  constexpr FVec3 operator*(FInt v) const {
    return FVec3(x_ * v, y_ * v, z_ * v);
  }

  constexpr void operator+=(const FVec3& a) {
    x_ += a.x_;
    y_ += a.y_;
    z_ += a.z_;
  }
  constexpr void operator-=(const FVec3& a) {
    x_ -= a.x_;
    y_ -= a.y_;
    z_ -= a.z_;
  }
  constexpr void operator*=(FInt v) {
    x_ *= v;
    y_ *= v;
    z_ *= v;
  }

  constexpr FVec3 operator*(const int32_t s) const {
    return FVec3(x_ * s, y_ * s, z_ * s);
  }
  constexpr FVec3 operator/(const int32_t s) const {
    return FVec3(x_ / s, y_ / s, z_ / s);
  }
  constexpr void operator*=(const int32_t s) {
    x_ *= s;
    y_ *= s;
    z_ *= s;
  }
  constexpr void operator/=(const int32_t s) {
    x_ /= s;
    y_ /= s;
    z_ /= s;
  }

  constexpr bool operator==(const FVec3& other) const {
    return x_ == other.x_ && y_ == other.y_ && z_ == other.z_;
  }
  constexpr bool operator!=(const FVec3& other) const {
    return x_ != other.x_ || y_ != other.y_ || z_ != other.z_;
  }
};

}  // namespace dux

std::ostream& operator<<(std::ostream& stream, const dux::FVec3& fvec3);

#endif  // DUX_FILED_SRC_FIXED_VEC3_H_
