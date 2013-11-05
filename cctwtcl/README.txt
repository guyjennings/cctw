
After building CCTW, run: 

tclsh make-package.tcl > pkgIndex.tcl

to create the Tcl package. 

Then, to use it from Tcl, set environment variable TCLLIBPATH to the
cctwtcl/ directory.

To use it from Swift, set environment variable TURBINE_USER_LIB to the
cctwtcl/ directory.

