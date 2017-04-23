#ifndef DUX_FILED_FIXED_TRIG_H_
#define DUX_FILED_FIXED_TRIG_H_

#include "fixed_int.h"

namespace dux {

namespace trig {
  
// Returns the cosinus of the radian angle |angle|.
FInt cos(FInt angle);

// Returns the sinus of the radian angle |angle|.
FInt sin(FInt angle);

// Returns the principal value of the arc tangent of y/x.
FInt atan2(FInt y, FInt x);
  
// Returns an angle in radian from an angle in degrees.
FInt ToRadian(FInt angle);
  
// Generates and prints the lookup tables used in the implementation.
void generateLookupTables();

}  // namespace trig
  
}  // namespace dux

#endif  // DUX_FILED_FIXED_TRIG_H_
