
(string result) cctw(string code) "cctw" "0.1"
[ "set <<result>> [ cctw <<code>> ]" ];

/*

 Probably obsolete-  

(int r) CctwXYZToID(int max_x, int max_y, int max_z,
                    int x, int y, int z)
"cctw" "0.0"
[ "set <<r>> [ CctwXYZtoID <<max_x>> <<max_y>> <<max_z>> <<x>> <<y>> <<z>> ]" ];

@dispatch=WORKER
(blob r) CctwLoadChunk(int max_x, int max_y, int max_z,
                       int x, int y, int z, string directory)
"cctw" "0.0" "load_chunk";

(int r[]) CctwRequisites(int max_x, int max_y, int max_z,
                         int osx, int osy, int osz)
"cctw" "0.0" "requisites";

(blob o) CctwTransform(int osx, int osy, int osz, blob i, int count)
"cctw" "0.0"
[ "set <<o>> [ cctw::transform <<osx>> <<osy>> <<osz>> <<i>> <<count>> ]" ];

*/
