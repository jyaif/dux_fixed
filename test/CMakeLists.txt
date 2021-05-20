cmake_minimum_required (VERSION 3.6)
project(dux_fixed_project)

add_compile_options(-std=c++17 -Werror)

add_subdirectory(../ dux_fixed_lib_build_dir)

add_executable(
  dux_fixed_test
  test.cpp
  test_grid_walking.cpp
  test_grid_walking.h
  test_fixed_int.cpp
  test_fixed_int.h
  test_fixed_rand.cpp
  test_fixed_rand.h
  test_fixed_vec2.cpp
  test_fixed_vec2.h
  test_fixed_trig.cpp
  test_fixed_trig.h
  utils.cpp
)

target_link_libraries(dux_fixed_test PRIVATE dux_fixed)