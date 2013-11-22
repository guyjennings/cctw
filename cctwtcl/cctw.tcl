
# Stub of Tcl wrapper library

namespace eval cctw {
    proc cctw_input_blob { filename id } {
        set L [ cctw_input $filename $id ]
        set pointer [ lindex $L 2 ]
        set length [ lindex $L 1 ]
        return [ list $pointer $length ]
    }
}
