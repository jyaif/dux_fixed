# dux_fixed

[![Build Status](https://api.travis-ci.org/jyaif/dux_fixed.svg)](https://travis-ci.org/jyaif/dux_fixed)

dux_fixed is a library handling fixed point calculation.

* Only implements 52:12 fixed point values.
* Uses C++17.
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
