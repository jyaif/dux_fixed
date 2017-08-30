#include "fixed_rand.h"

namespace dux {

FRandGenerator::FRandGenerator(int32_t seed) {
  rng_.reset(new std::mt19937(seed));
}

dux::FInt FRandGenerator::RandFInt(dux::FInt min, dux::FInt max) {
  assert(max > min);
  std::uniform_int_distribution<int64_t> distribution(min.raw_value_, max.raw_value_);
  return dux::FInt::FromRawValue(distribution(*rng_.get()));
}

uint32_t FRandGenerator::RandInt() { return (*rng_)(); }

}  // namespace dux
