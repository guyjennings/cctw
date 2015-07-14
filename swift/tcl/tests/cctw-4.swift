
/**
   THIS IS JUST A SKETCH
*/

import blob;
import io;

import cctw;

main
{
  blob chunks[];

  int input_chunks_i = 2;
  int input_chunks_j = 2;
  int input_chunks_k = 2;

  foreach i in [0:input_chunks_i-1] {
    foreach j in [0:input_chunks_j-1] {
      foreach k in [0:input_chunks_k-1] {
        string hdf_file = sprintf("fragment-%i-%i-%i.hdf", i, j, k);
        int id = cctw_ijk2id(input_chunks_i,input_chunks_j,
                             input_chunks_k,i,j,k);
        printf("opening: %s for chunk id: %i", hdf_file, id);
        chunks[id] = cctw_input(hdf_file, id);
        printf("size(%s) = %i", hdf_file, blob_size(chunks[id]));
      }}}

  output_chunks_x = 16; // toint(argv("chunks_x"));
  output_chunks_y = 16; // toint(argv("chunks_y"));
  output_chunks_z = 16; //

  // Total pixels in each dimension
  output_dimension_x = 2048;
  output_dimension_y = 2048;
  output_dimension_z = 2048;

  // input chunk IDs needed for output chunk (index)
  int requisite_input_chunk_IDs[][];

  blob output_products[][];
  /*  // C++:
  // List<(i,j,k,double* intensity, double* weight)> =
  // forward_transform(i,j,k, double* intensity (IN), double* weight (IN),
  foreach id in input_chunks {
    (output_ids[], intensity[], weights[]) = forward_transform(id, );
    // E.g.: [(0,3,0), (0,3,1), (1,3,1)] = forward_transform((1,1,1));
    foreach output_data, id in output_ids {
      output_products[id] += output_data;
      output_weights[id] += output_weight;
    }

    // E.g.:
    //// output_products((0,3,0)) += data;
    //// output_products((0,3,1)) += data;
    //// output_products((1,3,1)) += data;
  }

  // Merge/sum output buffers?
  outputs[id] = cctwMerge(output_products[id]);
*/
}
