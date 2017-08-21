#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "test_fint.h"
#include "test_fvec2.h"
#include "test_trig.h"

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  TestFInt();
  TestFVec2();
  TestTrig();
  printf("tests successfully passed\n");
  return EXIT_SUCCESS;
}
