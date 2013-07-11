#!/bin/sh -e

make swift_package

cd swift

stc -r $PWD -O0 test-cctw-2.swift test-cctw-2.tcl
turbine -l test-cctw-2.tcl
