
/**
 * TEST 1
 * Test call to CctwRequisites
 * */

import io;

import cctw;

main
{
  int r[] = CctwRequisites(2,2,2);
  foreach v,i in r
  {
    printf("v[%i]: %i", i, v);
  }
}
