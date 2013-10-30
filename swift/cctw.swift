
@dispatch=WORKER
(string result) cctw(string code) "cctw" "0.1"
[ "set <<result>> [ cctw <<code>> ]" ];

@dispatch=WORKER
(blob result) cctw_chunk_read(string filename) "cctw" "0.1"
[ "set <<result>> [ cctw::cctwswift_chunk_read <<filename>> ]" ];
