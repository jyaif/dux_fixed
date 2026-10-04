#include "fixed_vec2.h"
#include "fixed_trig.h"

#include <array>

namespace dux {

FInt FVec2::Length() {
  // This works poorly with x_ and y_ that are very small:
  // If x_ is less than 0.015625 (sqrt(4096)/4096), x_*x_ results in 0, even
  // though sqrt(x_*x_) would not be 0.
  if (x_ == 0_fx && y_ == 0_fx) {
    return 0_fx;
  }
  constexpr dux::FInt kLimit = dux::FInt::FromFraction(1, 10);
  if (x_ < kLimit && x_ > -kLimit && y_ < kLimit && y_ > -kLimit) {
    const auto x64 = dux::FInt::FromRawValue(x_.raw_value_ * 64);
    const auto y64 = dux::FInt::FromRawValue(y_.raw_value_ * 64);
    const auto result16 = ((x64 * x64) + (y64 * y64)).Sqrt();
    return dux::FInt::FromRawValue(result16.raw_value_ / 64);
  }
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

void FVec2::Rotate(dux::FInt angle) {
  dux::FInt sinn;
  dux::FInt coss;
  dux::trig::Sincos(angle, sinn, coss);
  dux::FInt new_x = x_ * coss - y_ * sinn;
  y_ = x_ * sinn + y_ * coss;
  x_ = new_x;
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

FInt FVec2::Angle() const {
  return dux::trig::Atan2(y_, x_);
}

}  // namespace dux

std::ostream& operator<<(std::ostream& stream, const dux::FVec2& fvec2) {
  stream << "(" << fvec2.x_ << "," << fvec2.y_ << ")";
  return stream;
}
