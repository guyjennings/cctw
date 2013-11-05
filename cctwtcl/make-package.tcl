
# Run "tclsh make-package.tcl > pkgIndex.tcl" to create the package

set name     "cctw"
set version  "0.1"
set leaf_so  "libCctwTcl.so"
set leaf_tcl "cctw.tcl"

puts [ ::pkg::create -name $name -version $version \
           -load $leaf_so -source $leaf_tcl ]
