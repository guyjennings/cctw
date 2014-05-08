
import string;

@dispatch=WORKER
(string result) cctw(string code) "cctw" "0.1"
[ "set <<result>> [ cctw <<code>> ]" ];

@dispatch=WORKER
(blob result) cctw_input(string filename, string dataset, int chunk) "cctw" "0.1"
[ "set <<result>> [ cctw_input <<filename>> <<dataset>> <<chunk>> ]" ];

@dispatch=WORKER
  (int index[], blob result[]) cctw_transform(int chunk, blob i)
  "cctw" "0.1" "cctw_transform_tcl";

@dispatch=WORKER
  (blob result) cctw_merge(bag<blob> b)
  "cctw" "0.1" "cctw_merge_tcl";
