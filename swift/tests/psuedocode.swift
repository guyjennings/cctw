
/**
   Pseudocode from in-person meeting 9/24/2013.
*/

blob INPUT[][][];
blob dependencies[][][]; // address[]
blob OUTPUT[][][];

// Input dimensions
I_X = 16;
I_Y = 16;
I_Z = 16;

// Output dimensions
O_X = 16;
O_Y = 16;
O_Z = 1 or 16;

// Loop over input dimensions
foreach x in I_X {
  foreach y in I_Y {
    foreach z in I_Z {

      dependencies[x][y][z] = compute_dependencies(x,y,z);
    }}}

//
foreach x in I_X {
  foreach y in I_Y {
    foreach z in I_Z {
      if (relevant(x,y,z,dependencies)) {
        INPUT[x,y,z] = load_chunk(x,y,z);
      }
    }}}

bag <blob> INTERMEDIATES[];

foreach x in I_X {
  foreach y in I_Y {
    foreach z in I_Z {

      if (relevant(x,y,z,dependencies)) {

        blob t[];
        int address;
        (t, address) = compute_intermediates(INPUT[x][y][z]);

        INTERMEDIATES[address] += t;
      }

    }}}

foreach b,address in INTERMEDIATES {
  blob chunk = reduction(b);
  OUTPUT[address] = chunk;
}

foreach address in OUTPUT {

  (x,y,z) = address2outputXYZ(address);
  write_output_fragment(OUTPUT[address], x,y,z, "file.nexus");
}
// OR
write_output(OUTPUT);

