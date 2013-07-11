
/**
 * TEST 1
 * Test call to CctwRequisites
 * */

import io;

import cctw;

global const int MAX_X = 3;
global const int MAX_Y = 3;
global const int MAX_Z = 3;

main
{
  int r[] = CctwRequisites(MAX_X, MAX_Y, MAX_Z, 2, 2, 2);
  foreach v,i in r
  {
    printf("v[%i]: %i", i, v);
  }
}
