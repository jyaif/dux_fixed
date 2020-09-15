#include "fixed_rand.h"

namespace dux {

FRandGenerator::FRandGenerator(uint32_t seed) : rng_(seed) {}

void FRandGenerator::SetSeed(uint32_t seed) {
  rng_.seed(seed);
}

dux::FInt FRandGenerator::RandFInt(dux::FInt min, dux::FInt max) {
  assert(max >= min);
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

uint32_t FRandGenerator::RandUInt32() {
  return rng_();
}


int64_t FRandGenerator::RandInt64(int64_t min, int64_t max) {
  assert(max >= min);
  std::uniform_int_distribution<int64_t> distribution(min, max);
  return distribution(rng_);
}

int32_t FRandGenerator::RandInt32(int32_t min, int32_t max) {
  assert(max >= min);
  std::uniform_int_distribution<int32_t> distribution(min, max);
  return distribution(rng_);
}

float FRandGenerator::RandFloat(float max) {
  return std::generate_canonical<double, 12>(rng_) * max;
}

}  // namespace dux
