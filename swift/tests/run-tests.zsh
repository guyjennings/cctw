#!/bin/zsh -eu

TURBINE_WORKERS=${TURBINE_WORKERS:-2}

declare TURBINE_WORKERS
print

PROCS=$(( TURBINE_WORKERS + 2 ))

@()
# Verbose operation
{
  print
  print ${*}
  print
  ${*}
}

for TEST in *.swift
do
  @ stc -I .. ${TEST}
  @ turbine -n ${PROCS} ${TEST%.swift}.tcl
done
