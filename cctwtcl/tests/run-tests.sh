#!/bin/bash -eu

# Run from cctw/cctwtcl/tests

TEST_DIR=$( cd $( dirname $0 ) ; /bin/pwd )
CCTWTCL_DIR=$( cd ${TEST_DIR}/.. ; /bin/pwd )

pushd ${CCTWTCL_DIR} >& /dev/null
if [[ ! -e libCctwTcl.so ]] 
then 
  ln -s libCctwTcl.so.0.1.0 libCctwTcl.so
fi
popd >& /dev/null

export TCLLIBPATH=${CCTWTCL_DIR}
export LD_LIBRARY_PATH=${CCTWTCL_DIR}

cd ${TEST_DIR}

if [[ ${#*} == 0 ]]
then
  TESTS=( *.tcl )
else 
  TESTS=( ${*} )
fi

VALGRIND=${VALGRIND:-}

for TEST in ${TESTS[@]}
do
  TEST=$( basename ${TEST} )
  echo running test: ${TEST}
  ${VALGRIND} tclsh ${TEST}
  echo
done
