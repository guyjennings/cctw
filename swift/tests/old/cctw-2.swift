
/**
 * TEST 2
 * Currently a sketch of dataflow pattern
 * Dataflow pattern:
 *  1) Each foreach loop is a parallel loop
 *  2) Each transform is ready to run
 *     as soon as all requisite inputs are loaded
 * */

import blob;
import sys;

import cctw;

global const int MAX_X = 3;
global const int MAX_Y = 3;
global const int MAX_Z = 3;

main
{
  blob outputs[][][];

  string data = argv("data");

  int inputTotal = MAX_X*MAX_Y*MAX_Z;
  blob inputs[];
  foreach x in [0:MAX_X-1]
  {
    foreach y in [0:MAX_Y-1]
    {
      foreach z in [0:MAX_Z-1]
      {
        blob t = CctwLoadChunk(MAX_X, MAX_Y, MAX_Z, x, y, z, data);
        int r = CctwXYZToID(MAX_X, MAX_Y, MAX_Z, x, y, z);
        inputs[r] = t;
      }
    }
  }
}
