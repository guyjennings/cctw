
namespace eval cctw {

    proc requisites { outputs inputs } {

        set r [ lindex $outputs 0 ]

        set max_x [ lindex $inputs 0 ]
        set max_y [ lindex $inputs 1 ]
        set max_z [ lindex $inputs 2 ]

        set osx [ lindex $inputs 3 ]
        set osy [ lindex $inputs 4 ]
        set osz [ lindex $inputs 5 ]

        rule [ list $max_x $max_y $max_z $osx $osy $osz ] \
            "cctw::requisites_body $outputs $max_x $max_y $max_z $osx $osy $osz"
    }

    proc requisites_body { outputs max_x max_y max_z osx osy osz } {

        set max_x_value [ retrieve_integer $max_x ]
        set max_y_value [ retrieve_integer $max_y ]
        set max_z_value [ retrieve_integer $max_z ]

        set osx_value [ retrieve_integer $osx ]
        set osy_value [ retrieve_integer $osy ]
        set osz_value [ retrieve_integer $osz ]

        set MAX_REQUISITES [ CctwMaxRequisites ]

        set p [ blobutils_malloc \
                    [ expr $MAX_REQUISITES * [blobutils_sizeof_int] ] ]
        set p [ blobutils_cast_to_int_ptr $p ]

        set count \
            [ CctwRequisites $max_x_value $max_y_value $max_z_value \
                             $osx_value $osy_value $osz_value $p ]

        for { set i 0 } { $i < $count } { incr i } {
            set v [ blobutils_get_int $p $i ]
            literal t integer $v
            container_insert $outputs $i $t
        }
        adlb::slot_drop $outputs
    }
}
