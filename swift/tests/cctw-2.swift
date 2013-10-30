
/**
   Use turbine -n 4 to get 2 workers
*/

import blob;
import io;

import cctw;

main
{
  blob tiff = cctw_chunk_read("pznpt4_0070.tif");
  printf("blob_size: %i", blob_size(tiff));
}
