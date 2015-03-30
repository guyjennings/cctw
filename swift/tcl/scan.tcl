#!/usr/bin/env tclsh

# Simple script to report CCTW functions available from Tcl

lappend auto_path .
lappend auto_path swift
package require cctw 0.0

set commands [ list ]

foreach cmd [ info commands ] {
    if { [ string match *Cctw* $cmd ] } {
        lappend commands $cmd
    }
}
set commands [ lsort $commands ]

puts "CCTW functions:"
foreach cmd $commands {
    puts $cmd
}
