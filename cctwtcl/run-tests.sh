#!/bin/sh -eu

export TCLLIBPATH=${PWD}

for TEST in *test-*.tcl 
do
  printf "\nRunning: %s\n\n" ${TEST}
  tclsh ${TEST}
done
printf "\nSUCCESS.\n"
