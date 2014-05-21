
# TCL TEST 3
# Test transform

package require cctw

set chunks 2
for { set chunk 0 } { $chunk < $chunks } { incr chunk } {
    # set L [ cctw_input $chunk ]
    # set ptr [ lindex $L 0 ]
    # cctw { script engine load preferences }
    cctw_transform $chunk
}
