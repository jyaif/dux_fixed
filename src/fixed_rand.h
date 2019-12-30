#ifndef DUX_FIXED_SRC_FIXED_RAND_H_
#define DUX_FIXED_SRC_FIXED_RAND_H_

#include <random>

#include "fixed_int.h"
#include "fixed_vec2.h"

namespace dux {

class FRandGenerator {
 public:
  FRandGenerator(int32_t seed = 0);
  void SetSeed(int64_t seed);
  // Returns a number in the range [min, max]. |max| must be >= |min|.
  dux::FInt RandFInt(dux::FInt min, dux::FInt max);
  dux::FVec2 RandFVec2(dux::FInt const minX,
                       dux::FInt const maxX,
                       dux::FInt const minY,
                       dux::FInt const maxY);
  uint32_t RandInt();
  // Returns a number in the range [min, max]. |max| must be >= |min|.
  int32_t RandInt(int32_t min, int32_t max);
  // Returns a number in the range [0, max].
  float RandFloat(float max);

 private:
  std::minstd_rand rng_;
};

}  // namespace dux

#endif  // DUX_FIXED_SRC_FIXED_RAND_H_
