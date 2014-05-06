
package require cctw

set cmds [ info commands ] 
foreach cmd $cmds { 
    if { [ string first "cctw_" $cmd ] == 0 } { 
        puts $cmd
    }
}

