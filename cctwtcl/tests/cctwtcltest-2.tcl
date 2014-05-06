
package require cctw

set L [ cctw_input "pznpt4_0070.tif" 1 ] 
set pointer [ lindex $L 0 ] 
set length  [ lindex $L 1 ] 

puts "pointer: $pointer length: $length"

