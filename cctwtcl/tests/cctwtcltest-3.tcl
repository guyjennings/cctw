
# TCL TEST 3
# Test transform

package require cctw

set L [ cctw_input "/home/wozniak/nexus-data/pznpt.nxs" "entry/data/v" 1 ] 
set ptr [ lindex $L 0 ] 
cctw_transform $ptr
