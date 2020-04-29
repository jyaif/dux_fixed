#ifndef DUX_FILED_SRC_FIXED_VEC2_H_
#define DUX_FILED_SRC_FIXED_VEC2_H_

#include <sstream>

#include "fixed_int.h"

namespace dux {

class FVec2 {
 public:
  FInt x_;
  FInt y_;

  FVec2() = default;
  FVec2(FVec2 const& v) = default;
  FVec2(FInt x, FInt y);
  FVec2(int x, int y);

  void Init(FInt x, FInt y);
  static FVec2 FromAngle(FInt angle, FInt radius);
  static FVec2 FromAngle(FInt angle);

  FInt SquareLength() const;
  FInt SquareLengthFrom(FVec2 const&) const;
  FInt Length();
  void Normalize(bool& success);
  void Normalize(bool& success, FInt newLength);
  FInt DotProduct(const FVec2& v) const;
  FInt Angle();
  void Rotate90Deg();

  FVec2 operator+(const FVec2&) const;
  FVec2 operator-(const FVec2&) const;
  FInt operator*(const FVec2&) const;
  FVec2 operator-() const;
  FVec2 operator*(FInt) const;

  void operator+=(const FVec2&);
  void operator-=(const FVec2&);
  void operator*=(FInt);

  FVec2 operator*(const int32_t s) const;
  FVec2 operator/(const int32_t s) const;
  void operator*=(const int32_t s);
  void operator/=(const int32_t s);

  bool operator==(const FVec2&) const;
  bool operator!=(const FVec2&) const;
};

}  // namespace dux

std::ostream& operator<<(std::ostream& stream, const dux::FVec2& fvec2);

#endif  // DUX_FILED_SRC_FIXED_VEC2_H_
