
# Stub of Tcl wrapper library

namespace eval cctw {
    proc cctw_input_blob { filename id } {
        set L [ cctw_input $filename $id ]
        set pointer [ lindex $L 2 ]
        set length [ lindex $L 1 ]
        return [ list $pointer $length ]
    }

    proc cctw_transform_tcl { outputs inputs } { 
        puts "cctw_transform_tcl: $outputs $inputs"
        rule $inputs "cctw::cctw_transform_tcl_body $outputs $inputs"
    }
    
    proc cctw_transform_tcl_body { output_ids output_blobs input_id input_blob } { 
        set input_id_value   [ retrieve_integer $input_id   ] 
        set input_blob_value [ retrieve_blob    $input_blob ] 
        show input_id_value
        set ptr [ lindex $input_blob_value 0 ]
        set products [ cctw_transform $ptr $input_id_value ]
        show products
    }
}
