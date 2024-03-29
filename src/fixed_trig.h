#ifndef DUX_FILED_SRC_FIXED_TRIG_H_
#define DUX_FILED_SRC_FIXED_TRIG_H_

#include "fixed_int.h"

namespace dux::trig {

// Returns the cosinus of the radian angle |angle|.
FInt Cos(FInt angle);

// Returns the sinus of the radian angle |angle|.
FInt Sin(FInt angle);

// Stores the sinus and cosinus of the radian angle |angle| in |sin| and |cos|.
void Sincos(FInt angle, FInt& sin, FInt& cos);

// Returns the principal value of the arc tangent of y/x.
// Returns a value in the range [0, 2*pi[.
FInt Atan2(FInt y, FInt x);

// Returns an angle in radians from an angle in degrees.
constexpr FInt ToRadian(FInt angle) {
  return FInt((angle * FIntTwoPi) / 360);
}

// Generates and prints the lookup tables used in the implementation.
void GenerateLookupTables();

}  // namespace dux::trig

#endif  // DUX_FILED_SRC_FIXED_TRIG_H_
