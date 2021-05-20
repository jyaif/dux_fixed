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
make
./dux_fixed_test
```

## Example

```cpp
// Arithmetic
dux::FInt a = dux::FInt::FromInteger(42);
dux::FInt b = dux::FInt::FromInteger(2);
dux::FInt c = dux::FInt::FromInteger(84);
assert(a * b == c);
// Trigonometry
dux::FInt zero = dux::FInt::FromInteger(0);
assert(dux::trig::Sin(zero) == zero);
// 2D Vector
dux::FVec2 v(3, 4);
assert(v.Length() == dux::FInt::FromInteger(5));
// And more (Grid walking, Random)
```
