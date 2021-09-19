set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
  dux_fixed
  src/grid_walking.cpp
  src/grid_walking.h
  src/fixed_int.cpp
  src/fixed_int.h
  src/fixed_trig.cpp
  src/fixed_trig.h
  src/fixed_vec2.cpp
  src/fixed_vec2.h
  src/fixed_vec3.cpp
  src/fixed_vec3.h
)

source_group(src/.*)

target_include_directories(dux_fixed PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")