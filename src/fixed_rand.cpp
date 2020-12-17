#include "fixed_rand.h"

namespace {

inline int dux_clz(unsigned __x) _NOEXCEPT { return __builtin_clz(__x); }
//inline int dux_clz(unsigned long __x) _NOEXCEPT { return __builtin_clzl(__x); }
inline int dux_clz(unsigned long long __x) _NOEXCEPT { return __builtin_clzll(__x); }

template<class _IntType, class _URNG>
_IntType UniformIntDistribution(_URNG& __g, _IntType a, _IntType b) {
    typedef typename std::conditional<sizeof(_IntType) <= sizeof(uint32_t),
                                            uint32_t, uint64_t>::type _UIntType;
    const _UIntType _Rp = _UIntType(b) - _UIntType(a) + _UIntType(1);
    if (_Rp == 1)
        return a;
    const size_t _Dt = std::numeric_limits<_UIntType>::digits;
    typedef std::__independent_bits_engine<_URNG, _UIntType> _Eng;
    if (_Rp == 0)
        return static_cast<_IntType>(_Eng(__g, _Dt)());
    auto clz = dux_clz(_Rp);
    size_t __w = _Dt - clz - 1;
    if ((_Rp & (std::numeric_limits<_UIntType>::max() >> (_Dt - __w))) != 0)
        ++__w;
    _Eng __e(__g, __w);
    _UIntType __u;
    do
    {
        __u = __e();
    } while (__u >= _Rp);
    return static_cast<_IntType>(__u + a);
}

}

namespace dux {

FRandGenerator::FRandGenerator(uint32_t seed) : rng_(seed) {}

void FRandGenerator::SetSeed(uint32_t seed) {
  rng_.seed(seed);
}

dux::FInt FRandGenerator::RandFInt(dux::FInt min, dux::FInt max) {
  assert(max >= min);
  return dux::FInt::FromRawValue(UniformIntDistribution<int64_t>(rng_, min.raw_value_, max.raw_value_));
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
  return UniformIntDistribution<int64_t>(rng_, min, max);
}

int32_t FRandGenerator::RandInt32(int32_t min, int32_t max) {
  assert(max >= min);
  return UniformIntDistribution<int32_t>(rng_, min, max);
}

float FRandGenerator::RandFloat(float max) {
  return std::generate_canonical<double, 12>(rng_) * max;
}

}  // namespace dux
