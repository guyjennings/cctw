
namespace eval cctw {

    proc requisites { outputs inputs } {

        set r   [ lindex $outputs 0 ]
        set osx [ lindex $inputs  0 ]
        set osy [ lindex $inputs  1 ]
        set osz [ lindex $inputs  2 ]

        rule [ list $osx $osy $osz ] \
            "cctw::requisites_body $outputs $osx $osy $osz"
    }

    proc requisites_body { outputs osx osy osz } {

        set osx_value [ retrieve_integer $osx ]
        set osy_value [ retrieve_integer $osy ]
        set osz_value [ retrieve_integer $osz ]

        set MAX_REQUISITES [ CctwMaxRequisites ]

        set p [ blobutils_malloc \
                    [ expr $MAX_REQUISITES * [blobutils_sizeof_int] ] ]
        set p [ blobutils_cast_to_int_ptr $p ]

        set count \
            [ CctwRequisites $osx_value $osy_value $osz_value $p ]

        for { set i 0 } { $i < $count } { incr i } {
            set v [ blobutils_get_int $p $i ]
            literal t integer $v
            container_insert $outputs $i $v
        }
        slot_drop $outputs
    }
}
