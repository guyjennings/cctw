
# Flex Tcl cctw_dependencies

package require cctw

set x 1
set y 1
set z 1

foreach i [ list 0 1 2 ] {
    set d [ cctw "inputData.dimensions\[$i\]" ]
    puts "d: $d"
}

set deps [ cctw_dependencies $x $y $z ]

puts "deps: $deps"
