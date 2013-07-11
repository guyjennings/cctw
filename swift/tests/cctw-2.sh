#!/bin/sh -e

make swift_package

cd swift

CCTW_SWIFT_PKG=$(/bin/pwd)

cd tests

stc -O0 -I $CCTW_SWIFT_PKG -r $CCTW_SWIFT_PKG cctw-2.swift cctw-2.tcl
turbine -l cctw-2.tcl --data=$CCTW_SWIFT_PKG/data
