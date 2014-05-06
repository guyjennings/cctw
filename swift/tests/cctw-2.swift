
/**
   Use turbine -n 4 to get 2 workers
*/

import blob;
import io;
import sys;

import cctw;

main
{
  foreach i in [0:9]
  {
    blob b = cctw_input(getenv("HOME")/"nexus-data/pznpt.nxs",
                        "entry/data/v", i);
    printf("blob_size: %i", blob_size(b));
  }
}
