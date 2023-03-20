# dux_fixed

[![Build Status](https://api.travis-ci.org/jyaif/dux_fixed.svg)](https://travis-ci.org/jyaif/dux_fixed)

dux_fixed is a library doing deterministic computations.

* Only implements 52:12 fixed point values.
* Uses C++17.
* Follows roughly the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
* MIT license.

To run tests locally (requires cmake):

```bash
mkdir out
cd out
cmake ../test/
cmake --build . --parallel

./dux_fixed_test
```

## Example

```cpp
// Arithmetic
dux::FInt a = 42_fx;
dux::FInt b = 2_fx;
dux::FInt c = 84_fx;
assert(a * b == c);
// Trigonometry
assert(dux::trig::Sin(0_fx) == 0_fx);
// 2D Vector
dux::FVec2 v(3_fx, 4_fx);
assert(v.Length() == 5_fx);
```
