
import string;

@dispatch=WORKER
(string result) cctw(string code) "cctw" "0.1"
[ "set <<result>> [ cctw <<code>> ]" ];

@dispatch=WORKER
  (blob result) cctw_input(string filename, string dataset, int chunk) "cctw" "0.1"
[ "set <<result>> [ cctw_input <<filename>> <<dataset>> <<chunk>> ]" ];
