cmake_minimum_required (VERSION 2.6)
project(dux_fixed)

# for tests:
include_directories("src")

set(CMAKE_CXX_FLAGS "-std=c++11")

add_executable(dux_fixed_test_bin src/fixed_int.cpp test/test.cpp)
