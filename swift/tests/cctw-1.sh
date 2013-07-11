#!/bin/sh -e

make swift_package

cd swift

CCTW_SWIFT_PKG=$(/bin/pwd)

cd tests

stc -I $CCTW_SWIFT_PKG -r $CCTW_SWIFT_PKG cctw-1.swift cctw-1.tcl
turbine cctw-1.tcl
