
/**
   THIS IS JUST A SKETCH
*/

import blob;
import io;

import cctw;

main
{
  string hdf_file = "big.hdf";
  // QList<void*> = hdf_read(string name)
  (blob fragments[],   max_input_x, max_input_y, max_input_z)
    = hdf_read(hdf_file);

  // E.g. fragment[(1,1,1)] = <binary data>

  foreach i,j,k {
    fragment[(i,j,k]] = read_hdf_fragment(hdf_file, i,j,k);
  }

  output_chunks_x = 16; // toint(argv("chunks_x"));
  output_chunks_y = 16; // toint(argv("chunks_y"));
  output_chunks_z = 16; //

  // Total pixels in each dimension
  output_dimension_x = 2048;
  output_dimension_y = 2048;
  output_dimension_z = 2048;

  // input chunk IDs needed for output chunk (index)
  int requisite_input_chunk_IDs[][];

  // (1,1,1) -> (0,3,0), (0,3,1), (1,3,1)
  // (2,2,2) -> (0,5,0), (0,7,3), (1,3,1)
  // requisite_input_chunk_IDs[(1,3,1)] = (1,1,1), (2,2,2)

  output_products[][];

  // foreach i in [0:max_input_x-1] {
  //   foreach j in [0:max_input_y-1] {
  //     foreach k in [0:max_input_z-1] {

  //       int output_chunk_IDs[] = forward_dependencies(i,j,k);
  //       foreach id in output_chunk_IDs {
  //         requisite_input_chunk_IDs[id] += (i,j,k);
  //       }
  //       // if (condition)
  //       //   input_chunks_needed += (i,j,k);
  //     }}}


  // C++:
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
}
