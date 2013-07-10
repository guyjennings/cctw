#!/bin/sh -e

make swift_package

cd swift

stc -r $PWD test-cctw-1.swift test-cctw-1.tcl
turbine test-cctw-1.tcl
