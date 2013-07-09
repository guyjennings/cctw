#!/bin/sh -e

cd swift

stc -r $PWD test-cctw-1.swift test-cctw-1.tcl
turbine test-cctw-1.tcl
