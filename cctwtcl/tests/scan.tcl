
# SCAN.TCL

# Runs as a Tcl test
# Simply reports all known CCTW Tcl functions

package require cctw

set cmds [ info commands ] 
foreach cmd $cmds { 
    if { [ string first "cctw_" $cmd ] == 0 } { 
        puts $cmd
    }
}

