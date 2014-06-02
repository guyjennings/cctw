
# TCL TEST 3
# Test transform

package require cctw

set chunks [ list 0 4 65 130 ]
foreach chunk $chunks { 
    # set L [ cctw_input $chunk ]
    # set ptr [ lindex $L 0 ]
    # cctw { script engine load preferences }
    puts "chunk: $chunk"
    set output [ cctw_transform $chunk ]
    puts $output
    puts ""
}
