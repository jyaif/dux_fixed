# dux_fixed

[![Build Status](https://api.travis-ci.org/jyaif/dux_fixed.svg)](https://travis-ci.org/jyaif/dux_fixed)

dux_fixed is a small library handling fixed point calculation.

* Only uses 52:12 fixed point values.
* Readable, but not optimized.
* Uses C++14.
* Follows roughly the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
* MIT license.

To run tests locally (requires cmake):

```
mkdir out
cd out
cmake ..
make
./dux_fixed_test_bin
```
