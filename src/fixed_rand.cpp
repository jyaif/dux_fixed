#include "fixed_rand.h"

namespace {

inline int dux_clz(uint32_t __x) { return __builtin_clz(__x); }
inline int dux_clz(uint64_t __x) { return __builtin_clzll(__x); }

template <unsigned long long _Xp, size_t _Rp>
struct __log2_imp
{
    static const size_t value = _Xp & ((unsigned long long)(1) << _Rp) ? _Rp
                                           : __log2_imp<_Xp, _Rp - 1>::value;
};

template <unsigned long long _Xp>
struct __log2_imp<_Xp, 0>
{
    static const size_t value = 0;
};

template <size_t _Rp>
struct __log2_imp<0, _Rp>
{
    static const size_t value = _Rp + 1;
};

template <class _UIntType, _UIntType _Xp>
struct __log2
{
    static const size_t value = __log2_imp<_Xp,
                                         sizeof(_UIntType) * __CHAR_BIT__ - 1>::value;
};

template<class _Engine, class _UIntType>
class __independent_bits_engine
{
public:
    // types
    typedef _UIntType result_type;

private:
    typedef typename _Engine::result_type _Engine_result_type;
    typedef typename std::conditional
        <
            sizeof(_Engine_result_type) <= sizeof(result_type),
                result_type,
                _Engine_result_type
        >::type _Working_result_type;

    _Engine& __e_;
    size_t __w_;
    size_t __w0_;
    size_t __n_;
    size_t __n0_;
    _Working_result_type __y0_;
    _Working_result_type __y1_;
    _Engine_result_type __mask0_;
    _Engine_result_type __mask1_;

    static constexpr const _Working_result_type _Rp = _Engine::max() - _Engine::min() + _Working_result_type(1);
    static constexpr const size_t __m = __log2<_Working_result_type, _Rp>::value;
    static constexpr const size_t _WDt = std::numeric_limits<_Working_result_type>::digits;
    static constexpr const size_t _EDt = std::numeric_limits<_Engine_result_type>::digits;

public:
    // constructors and seeding functions
    __independent_bits_engine(_Engine& __e, size_t __w);

    // generating functions
    result_type operator()() {return __eval(std::integral_constant<bool, _Rp != 0>());}

private:
    result_type __eval(std::false_type);
    result_type __eval(std::true_type);
};

template<class _Engine, class _UIntType>
__independent_bits_engine<_Engine, _UIntType>
    ::__independent_bits_engine(_Engine& __e, size_t __w)
        : __e_(__e),
          __w_(__w)
{
    __n_ = __w_ / __m + (__w_ % __m != 0);
    __w0_ = __w_ / __n_;
    if (_Rp == 0)
        __y0_ = _Rp;
    else if (__w0_ < _WDt)
        __y0_ = (_Rp >> __w0_) << __w0_;
    else
        __y0_ = 0;
    if (_Rp - __y0_ > __y0_ / __n_)
    {
        ++__n_;
        __w0_ = __w_ / __n_;
        if (__w0_ < _WDt)
            __y0_ = (_Rp >> __w0_) << __w0_;
        else
            __y0_ = 0;
    }
    __n0_ = __n_ - __w_ % __n_;
    if (__w0_ < _WDt - 1)
        __y1_ = (_Rp >> (__w0_ + 1)) << (__w0_ + 1);
    else
        __y1_ = 0;
    __mask0_ = __w0_ > 0 ? _Engine_result_type(~0) >> (_EDt - __w0_) :
                          _Engine_result_type(0);
    __mask1_ = __w0_ < _EDt - 1 ?
                               _Engine_result_type(~0) >> (_EDt - (__w0_ + 1)) :
                               _Engine_result_type(~0);
}

template<class _Engine, class _UIntType>
inline
_UIntType
__independent_bits_engine<_Engine, _UIntType>::__eval(std::false_type)
{
    return static_cast<result_type>(__e_() & __mask0_);
}

template<class _Engine, class _UIntType>
_UIntType
__independent_bits_engine<_Engine, _UIntType>::__eval(std::true_type)
{
    const size_t _WRt = std::numeric_limits<result_type>::digits;
    result_type _Sp = 0;
    for (size_t __k = 0; __k < __n0_; ++__k)
    {
        _Engine_result_type __u;
        do
        {
            __u = __e_() - _Engine::min();
        } while (__u >= __y0_);
        if (__w0_ < _WRt)
            _Sp <<= __w0_;
        else
            _Sp = 0;
        _Sp += __u & __mask0_;
    }
    for (size_t __k = __n0_; __k < __n_; ++__k)
    {
        _Engine_result_type __u;
        do
        {
            __u = __e_() - _Engine::min();
        } while (__u >= __y1_);
        if (__w0_ < _WRt - 1)
            _Sp <<= __w0_ + 1;
        else
            _Sp = 0;
        _Sp += __u & __mask1_;
    }
    return _Sp;
}

template<class _IntType, class _URNG>
_IntType UniformIntDistribution(_URNG& __g, _IntType a, _IntType b) {
    typedef typename std::conditional<sizeof(_IntType) <= sizeof(uint32_t),
                                            uint32_t, uint64_t>::type _UIntType;
    const _UIntType _Rp = _UIntType(b) - _UIntType(a) + _UIntType(1);
    if (_Rp == 1)
        return a;
    const size_t _Dt = std::numeric_limits<_UIntType>::digits;
    typedef __independent_bits_engine<_URNG, _UIntType> _Eng;
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
