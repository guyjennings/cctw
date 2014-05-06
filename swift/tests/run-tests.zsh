#!/bin/zsh -eu

TEST_DIR=$( cd $( dirname $0 ) ; /bin/pwd )
CCTWTCL_DIR=$( cd ${TEST_DIR}/../../cctwtcl ; /bin/pwd )

export TURBINE_USER_LIB=${CCTWTCL_DIR}
export LD_LIBRARY_PATH=${CCTWTCL_DIR}

TURBINE_WORKERS=${TURBINE_WORKERS:-2}

declare CCTWTCL_DIR TURBINE_WORKERS
print

PROCS=$(( TURBINE_WORKERS + 2 ))

if [[ -z ${TURBINE_USER_LIB} ]]
then
  print "Set TURBINE_USER_LIB to the directory containing pkgIndex.tcl"
  exit 1
fi

@()
# Verbose operation
{
  print
  print ${*}
  print
  ${*}
}

if [[ ${#*} == 0 ]] 
then 
  TESTS=( ${TEST_DIR}/*.swift )
else 
  TESTS=${*}
fi

cd ${TEST_DIR}
declare PWD

for TEST in ${TESTS}
do
  TEST=$( basename ${TEST} )
  @ stc -I .. ${TEST}
  @ turbine -n ${PROCS} ${TEST%.swift}.tcl
done
