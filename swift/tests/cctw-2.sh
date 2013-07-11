#!/bin/sh -e

make swift_package

cd swift

CCTW_SWIFT_PKG=$(/bin/pwd)

cd tests

N=${N:-3}

stc -O0 -I $CCTW_SWIFT_PKG -r $CCTW_SWIFT_PKG cctw-2.swift cctw-2.tcl
time turbine -l -n $N cctw-2.tcl --data=$CCTW_SWIFT_PKG/data
