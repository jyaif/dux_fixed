#include "fixed_rand.h"

namespace dux {

FRandGenerator::FRandGenerator(int32_t seed) : rng_(seed) {}

void FRandGenerator::SetSeed(int32_t seed) {
  rng_.seed(seed);
}

dux::FInt FRandGenerator::RandFInt(dux::FInt min, dux::FInt max) {
  assert(max > min);
  std::uniform_int_distribution<int64_t> distribution(min.raw_value_,
                                                      max.raw_value_);
  return dux::FInt::FromRawValue(distribution(rng_));
}

dux::FVec2 FRandGenerator::RandFVec2(dux::FInt const minX,
                                     dux::FInt const maxX,
                                     dux::FInt const minY,
                                     dux::FInt const maxY) {
  return dux::FVec2(RandFInt(minX, maxX), RandFInt(minY, maxY));
}

uint32_t FRandGenerator::RandInt() {
  return rng_();
}

int32_t FRandGenerator::RandInt(int32_t min, int32_t max) {
  std::uniform_int_distribution<int64_t> distribution(min, max);
  return distribution(rng_);
}

}  // namespace dux
