
/**
   THIS IS JUST A SKETCH
*/

import blob;
import io;

import cctw;

global const int MAX_X = ...;
global const int MAX_Y = ...;
global const int MAX_Z = ...;

main
{
  string file_list = [ "pznpt4_0070.tif", "pznpt4_0071.tif", ... ];
  blob tiff_fragments[];
  foreach f in file_list {
    // For file pznpt4_0070.tif, creates entries (say) 007000-007099
    // (100 fragments, at 10x10 partitioning of the TIFF)
    blob tmp[] = cctw_chunk_read_fragments();
    foreach b, i in tmp {
      tiff_fragments[i] = b;
    }
  }

  foreach i in [0:MAX_X-1] {
    foreach j in [0:MAX_Y-1] {
      foreach k in [0:MAX_Z-1] {
        // Requisite IDs 007123, 007124, etc.
        int requisiteIDs[] = CctwRequisites(i, j, k);
        int count = size(requisites);
        blob requisites[];
        foreach r, m in requisiteIDs {
          requisites[m] = tiff_fragments[r];
        }
        // All requisite raster buffers are available to this function
        // (because requisites is an array of blobs):
        outputs[i][j][k] = CctwTransform(i, j, k, requisites, count);
      }
    }
  }

  // Merge/sum output buffers?
}
