
(int r[]) CctwRequisites(int max_x, int max_y, int max_z,
                         int osx, int osy, int osz)
"cctw" "0.0" "requisites";

(blob o) CctwTransform(int osx, int osy, int osz, blob i, int count)
"cctw" "0.0"
[ "set <<o>> [ cctw::transform <<osx>> <<osy>> <<osz>> <<i>> <<count>> ]" ];


