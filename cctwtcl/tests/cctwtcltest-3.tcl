
# TCL TEST 3
# Test transform

package require cctw

set p [ cctw_parameters ]
puts "parameters: \n $p"

set chunks [ list 0 4 65 130 5100 ]
foreach chunk $chunks { 
    # set L [ cctw_input $chunk ]
    # set ptr [ lindex $L 0 ]
    # cctw { script engine load preferences }
    puts "chunk: $chunk"
    set output [ cctw_transform $chunk ]
    puts $output
    puts ""
}
