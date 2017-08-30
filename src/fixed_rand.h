#ifndef DUX_FIXED_SRC_FIXED_RAND_H_
#define DUX_FIXED_SRC_FIXED_RAND_H_

#include <random>
#include <memory>

#include "fixed_int.h"

namespace dux {

class FRandGenerator {
 public:
  FRandGenerator(int32_t seed);
  dux::FInt RandFInt(dux::FInt min, dux::FInt max);
  uint32_t RandInt();

 private:
  std::unique_ptr<std::mt19937> rng_;
};
  
}  // namespace dux

#endif  // DUX_FIXED_SRC_FIXED_RAND_H_
