cmake_minimum_required (VERSION 2.6)
project(dux_fixed)

# for tests:
include_directories("src")

set(CMAKE_CXX_FLAGS "-std=c++17 -Werror")

add_executable(
  dux_fixed_test_bin
  test/test.cpp
  test/test_grid_walking.cpp
  test/test_grid_walking.h
  test/test_fixed_int.cpp
  test/test_fixed_int.h
  test/test_fixed_rand.cpp
  test/test_fixed_rand.h
  test/test_fixed_vec2.cpp
  test/test_fixed_vec2.h
  test/test_fixed_trig.cpp
  test/test_fixed_trig.h
  test/utils.cpp
  src/grid_walking.cpp
  src/grid_walking.h
  src/fixed_int.cpp
  src/fixed_int.h
  src/fixed_rand.cpp
  src/fixed_rand.h
  src/fixed_trig.cpp
  src/fixed_trig.h
  src/fixed_vec2.cpp
  src/fixed_vec2.h
  src/fixed_vec3.cpp
  src/fixed_vec3.h
)