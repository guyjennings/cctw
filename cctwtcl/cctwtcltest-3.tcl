
# Flex Tcl cctw_dependencies

package require cctw

set x 1
set y 1
set z 1

set deps [ cctw_dependencies $x $y $z ]

puts "deps: $deps"
