#include "fixed_vec3.h"
#include "fixed_trig.h"

#include <array>

namespace dux {

FVec3::FVec3(FInt x, FInt y, FInt z) : x_(x), y_(y), z_(z) {}

FVec3::FVec3(int x, int y, int z)
    : x_(FInt::FromInt(x)), y_(FInt::FromInt(y)), z_(FInt::FromInt(z)) {}

FInt FVec3::SquareLength() const {
  return x_ * x_ + y_ * y_ + z_ * z_;
}

FInt FVec3::SquareLengthFrom(FVec3 const& c) const {
  FInt dx = c.x_ - x_;
  FInt dy = c.y_ - y_;
  FInt dz = c.z_ - z_;
  return dx * dx + dy * dy + dz * dz;
}

FInt FVec3::Length() {
  return SquareLength().Sqrt();
}

void FVec3::Normalize(bool& success) {
  FInt l = Length();
  if (l.raw_value_ != 0) {
    x_ /= l;
    y_ /= l;
    z_ /= l;
    success = true;
  } else {
    success = false;
  }
}

void FVec3::Normalize(bool& success, FInt newLength) {
  FInt l = Length();
  if (l.raw_value_ != 0) {
    newLength = newLength / l;
    x_ *= newLength;
    y_ *= newLength;
    z_ *= newLength;
    success = true;
  } else {
    success = false;
  }
}

void FVec3::Init(FInt x, FInt y, FInt z) {
  x_ = x;
  y_ = y;
  z_ = z;
}

FVec3 FVec3::operator+(const FVec3& a) const {
  FVec3 v(a.x_ + x_, a.y_ + y_, a.z_ + z_);
  return v;
}

void FVec3::operator+=(const FVec3& a) {
  x_ += a.x_;
  y_ += a.y_;
  z_ += a.z_;
}

FVec3 FVec3::operator-(const FVec3& a) const {
  FVec3 c(x_ - a.x_, y_ - a.y_, z_ - a.z_);
  return c;
}

FVec3 FVec3::operator-() const {
  FVec3 c(-x_, -y_, -z_);
  return c;
}

void FVec3::operator-=(const FVec3& a) {
  x_ -= a.x_;
  y_ -= a.y_;
  z_ -= a.z_;
}

FInt FVec3::operator*(const FVec3& a) const {
  return (x_ * a.x_ + y_ * a.y_ + z_ * a.z_);
}

FVec3 FVec3::operator*(FInt v) const {
  FVec3 c(x_ * v, y_ * v, z_ * v);
  return c;
}

void FVec3::operator*=(FInt v) {
  x_ *= v;
  y_ *= v;
  z_ *= v;
}

FVec3 FVec3::operator*(const int32_t s) const {
  FVec3 v(x_ * s, y_ * s, z_ * s);
  return v;
}

FVec3 FVec3::operator/(const int32_t s) const {
  FVec3 v(x_ / s, y_ / s, z_ / s);
  return v;
}

void FVec3::operator*=(const int32_t s) {
  x_ *= s;
  y_ *= s;
  z_ *= s;
}

void FVec3::operator/=(const int32_t s) {
  x_ /= s;
  y_ /= s;
  z_ /= s;
}

bool FVec3::operator==(const FVec3& other) const {
  return x_ == other.x_ && y_ == other.y_ && z_ == other.z_;
}

bool FVec3::operator!=(const FVec3& other) const {
  return x_ != other.x_ || y_ != other.y_ || z_ != other.z_;
}

}  // namespace dux

std::ostream& operator<<(std::ostream& stream, const dux::FVec3& fvec3) {
  stream << "(" << fvec3.x_ << "," << fvec3.y_ << "," << fvec3.z_ << ")";
  return stream;
}
