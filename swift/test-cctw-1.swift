
/**
 * TEST 1
 * Just test that we can call the C++ layer
 * */

import blob;

import cctw;

main
{
  blob i = blob_null();
  blob o = CctwTransform(1,1,1,i,1);
}
