cmake_minimum_required (VERSION 2.6)
project(dux_fixed)

# for tests:
include_directories("src")

set(CMAKE_CXX_FLAGS "-std=c++14")

add_executable(dux_fixed_test_bin src/fixed_int.cpp src/fixed_trig.cpp src/fixed_vec2.cpp src/fixed_vec3.cpp test/test.cpp)
