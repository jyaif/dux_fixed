#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "test_fint.h"
#include "test_fvec2.h"
#include "test_trig.h"

int main(int argc, char *argv[]) {
  TestFInt();
  TestFVec2();
  TestTrig();
  printf("tests successfully passed\n");
  return EXIT_SUCCESS;
}
