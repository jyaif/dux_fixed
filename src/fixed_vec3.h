#ifndef DUX_FILED_SRC_FIXED_VEC3_H_
#define DUX_FILED_SRC_FIXED_VEC3_H_

#include "fixed_int.h"

namespace dux {

class FVec3 {
 public:
  FInt x_;
  FInt y_;
  FInt z_;

  FVec3() = default;
  FVec3(FVec3 const& v) = default;
  FVec3(FInt x, FInt y, FInt z);
  FVec3(int x, int y, int z);

  void Init(FInt x, FInt y, FInt z);

  FInt SquareLength() const;
  FInt SquareLengthFrom(FVec3 const&) const;
  FInt Length();
  void Normalize(bool& success);
  void Normalize(bool& success, FInt newLength);

  FVec3 operator+(const FVec3&) const;
  FVec3 operator-(const FVec3&) const;
  FInt operator*(const FVec3&) const;
  FVec3 operator-() const;
  FVec3 operator*(FInt) const;

  void operator+=(const FVec3&);
  void operator-=(const FVec3&);
  void operator*=(FInt);

  FVec3 operator*(const int32_t s) const;
  FVec3 operator/(const int32_t s) const;
  void operator*=(const int32_t s);
  void operator/=(const int32_t s);

  bool operator==(const FVec3&) const;
  bool operator!=(const FVec3&) const;
};

}  // namespace dux

std::ostream& operator<<(std::ostream& stream, const dux::FVec3& fvec3);

#endif  // DUX_FILED_SRC_FIXED_VEC3_H_
