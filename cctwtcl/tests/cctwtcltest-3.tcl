
# TCL TEST 3
# Test transform

package require cctw

set chunk 1
set L [ cctw_input "/home/wozniak/nexus-data/pznpt.nxs" "entry/data/v" $chunk ] 
set ptr [ lindex $L 0 ] 
cctw_transform $ptr $chunk
