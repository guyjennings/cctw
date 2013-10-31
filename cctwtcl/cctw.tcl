
# Stub of Tcl wrapper library

namespace eval cctw {
    proc cctwswift_chunk_read { filename } {
        cctw_chunk_read $filename pointer length
        return [ list $pointer $length ]
    }
}
