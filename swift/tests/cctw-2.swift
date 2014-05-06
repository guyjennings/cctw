
/**
   Use turbine -n 4 to get 2 workers
*/

import blob;
import io;
import sys;

import cctw;

main
{
  
  blob tiff = cctw_input(getenv("HOME")/"nexus-data/pznpt.nxs",
                         "entry/data/v", 1);
  printf("blob_size: %i", blob_size(tiff));
}
