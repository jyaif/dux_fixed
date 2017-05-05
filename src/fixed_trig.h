#ifndef DUX_FILED_FIXED_TRIG_H_
#define DUX_FILED_FIXED_TRIG_H_

#include "fixed_int.h"

namespace dux {

namespace trig {

// Returns the cosinus of the radian angle |angle|.
FInt Cos(FInt angle);

// Returns the sinus of the radian angle |angle|.
FInt Sin(FInt angle);

// Returns the principal value of the arc tangent of y/x.
FInt Atan2(FInt y, FInt x);

// Returns an angle in radians from an angle in degrees.
constexpr FInt ToRadian(FInt angle) {
  if (angle.raw_value_ >= (360 << FInt::kShift))
    angle.raw_value_ %= (360 << FInt::kShift);
  if (angle.raw_value_ <= -(360 << FInt::kShift))
    angle.raw_value_ = -(-angle.raw_value_ % (360 << FInt::kShift));
  // Precision of conversion can be improved.
  // 57 = 1 / ((Pi * 2) / 360)
  return FInt::FromRawValue(angle.raw_value_ / 57);
}

// Returns an angle in degrees from an angle in radians.
FInt ToDegree(FInt angle);

// Generates and prints the lookup tables used in the implementation.
void GenerateLookupTables();

}  // namespace trig

}  // namespace dux

#endif  // DUX_FILED_FIXED_TRIG_H_
