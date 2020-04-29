#include "fixed_vec2.h"
#include "fixed_trig.h"

#include <array>

namespace dux {

FVec2::FVec2(FInt x, FInt y) : x_(x), y_(y) {}

FVec2::FVec2(int x, int y) : x_(FInt::FromInt(x)), y_(FInt::FromInt(y)) {}

FInt FVec2::SquareLength() const {
  return x_ * x_ + y_ * y_;
}

FInt FVec2::SquareLengthFrom(FVec2 const& c) const {
  FInt dx = c.x_ - x_;
  FInt dy = c.y_ - y_;
  return dx * dx + dy * dy;
}

FInt FVec2::Length() {
  return ((x_ * x_) + (y_ * y_)).Sqrt();
}

void FVec2::Normalize(bool& success) {
  FInt l = Length();
  if (l.raw_value_ != 0) {
    x_ /= l;
    y_ /= l;
    success = true;
  } else {
    success = false;
  }
}

void FVec2::Normalize(bool& success, FInt newLength) {
  FInt l = Length();
  if (l.raw_value_ != 0) {
    newLength = newLength / l;
    x_ *= newLength;
    y_ *= newLength;
    success = true;
  } else {
    success = false;
  }
}

FInt FVec2::DotProduct(const FVec2& v) const {
  return (x_ * v.x_) + (y_ * v.y_);
}

void FVec2::Rotate90Deg() {
  std::swap(x_, y_);
  x_ = -x_;
}

void FVec2::Init(FInt x, FInt y) {
  x_ = x;
  y_ = y;
}

FVec2 FVec2::FromAngle(FInt angle, FInt radius) {
  FVec2 v;
  dux::trig::Sincos(angle, v.y_, v.x_);
  v *= radius;
  return v;
}

FVec2 FVec2::FromAngle(FInt angle) {
  FVec2 v;
  dux::trig::Sincos(angle, v.y_, v.x_);
  return v;
}

FVec2 FVec2::operator+(const FVec2& a) const {
  FVec2 v(a.x_ + x_, a.y_ + y_);
  return v;
}

void FVec2::operator+=(const FVec2& a) {
  x_ += a.x_;
  y_ += a.y_;
}

FVec2 FVec2::operator-(const FVec2& a) const {
  FVec2 c(x_ - a.x_, y_ - a.y_);
  return c;
}

FVec2 FVec2::operator-() const {
  FVec2 c(-x_, -y_);
  return c;
}

void FVec2::operator-=(const FVec2& a) {
  x_ -= a.x_;
  y_ -= a.y_;
}

FInt FVec2::operator*(const FVec2& a) const {
  return (x_ * a.x_ + y_ * a.y_);
}

FVec2 FVec2::operator*(FInt v) const {
  FVec2 c(x_ * v, y_ * v);
  return c;
}

void FVec2::operator*=(FInt v) {
  x_ *= v;
  y_ *= v;
}

FVec2 FVec2::operator*(const int32_t s) const {
  FVec2 v(x_ * s, y_ * s);
  return v;
}

FVec2 FVec2::operator/(const int32_t s) const {
  FVec2 v(x_ / s, y_ / s);
  return v;
}

void FVec2::operator*=(const int32_t s) {
  x_ *= s;
  y_ *= s;
}

void FVec2::operator/=(const int32_t s) {
  x_ /= s;
  y_ /= s;
}

bool FVec2::operator==(const FVec2& other) const {
  return x_ == other.x_ && y_ == other.y_;
}

bool FVec2::operator!=(const FVec2& other) const {
  return x_ != other.x_ || y_ != other.y_;
}

FInt FVec2::Angle() {
  return dux::trig::Atan2(y_, x_);
}

}  // namespace dux

std::ostream& operator<<(std::ostream& stream, const dux::FVec2& fvec2) {
  stream << "(" << fvec2.x_ << "," << fvec2.y_ << ")";
  return stream;
}
