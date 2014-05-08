
# TCL TEST 3
# Test transform

package require cctw

set chunks 9
for { set chunk 0 } { $chunk < $chunks } { incr chunk } { 
    set L [ cctw_input "/home/wozniak/nexus-data/pznpt.nxs" "entry/data/v" $chunk ] 
    set ptr [ lindex $L 0 ] 
    # cctw { script engine load preferences } 
    cctw_transform $ptr $chunk
}
