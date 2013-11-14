
/**
 * TEST 3
 * Currently a sketch of dataflow pattern
 * Dataflow pattern:
 *  1) Each foreach loop is a parallel loop
 *  2) Each transform is ready to run
 *     as soon as all requisite inputs are loaded
 * */

import blob;

import cctw;

main
{
  int max_i;
  int max_j;
  int max_k;
  blob outputs[][][];

  int inputTotal = max_i*max_j*max_k;
  blob inputs[];
  foreach i in [0:max_i-1]
  {
    foreach j in [0:max_j-1]
    {
      foreach k in [0:max_k-1]
      {
        blob t = CctwLoadFragment(i, j, k);
        int r = CctwXYZToID(i, j, k);
        inputs[r] = t;
      }
    }
  }

  foreach i in [0:max_i-1]
  {
    foreach j in [0:max_j-1]
    {
      foreach k in [0:max_k-1]
      {
        int requisiteIDs[] = CctwRequisites(i, j, k);
        int count = size(requisites);
        blob requisites[];
        foreach r, m in requisiteIDs
        {
          requisites[m] = r;
        }
        outputs[i][j][k] = CctwTransform(i, j, k, requisites, count);
      }
    }
  }
  CctwStoreOutputs(outputs);
}
