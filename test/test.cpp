#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "test_fixed_int.h"
#include "test_fixed_rand.h"
#include "test_fixed_vec2.h"
#include "test_fixed_trig.h"

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  TestFInt();
  TestFRandGenerator();
  TestFVec2();
  TestTrig();
  printf("tests successfully passed\n");
  return EXIT_SUCCESS;
}
