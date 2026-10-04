#include "fixed_vec3.h"
#include "fixed_trig.h"

#include <array>

namespace dux {

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

}  // namespace dux

std::ostream& operator<<(std::ostream& stream, const dux::FVec3& fvec3) {
  stream << "(" << fvec3.x_ << "," << fvec3.y_ << "," << fvec3.z_ << ")";
  return stream;
}
