
/**
   CCTW TEST 3
*/

import blob;
import io;
import sys;

import cctw;

main
{
  bag<blob> M[];
  
  foreach i in [1:1]
  {
    blob b1= cctw_input(getenv("HOME")/"nexus-data/pznpt.nxs",
                        "entry/data/v", i);
    printf("blob_size: %i", blob_size(b1));
      
    blob b2[];
    int outputId[];
    // Each transform produces a variable number of indexed blobs
    (outputId, b2) = cctw_transform(i, b1);
    foreach b, j in b2
    {
      int slot = outputId[j];
      M[slot] += b;
    }
  }

  foreach g in M
  {
    blob b = cctw_merge(g);
    blob_write(b);
  }
}
