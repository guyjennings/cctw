#!/bin/zsh -eu

# RUN-TESTS.ZSH (Tcl)

# usage: run-tests.zsh [-m] <tests>*
#  -m:    Runs make first
#  tests: Runs each given test file.  If none, run all tests.

TEST_DIR=$(    cd $( dirname $0 )   ; /bin/pwd )
CCTWTCL_DIR=$( cd ${TEST_DIR}/..    ; /bin/pwd )
CCTW_DIR=$(    cd ${CCTWTCL_DIR}/.. ; /bin/pwd )

setopt PUSHD_SILENT

MAKE=""
zparseopts -D m=MAKE
if [[ ${MAKE} != "" ]] 
then
  pushd ${CCTW_DIR}
  make 
  popd
fi

pushd ${CCTWTCL_DIR} 
if [[ ! -e libCctwTcl.so ]] 
then 
  ln -s ../bin/libCctwTcl.so.0.1.1 
  ln -s libCctwTcl.so.0.1.1 libCctwTcl.so
fi
popd

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
  TEST=${TEST%.tcl}
  echo running test: ${TEST}
  ${VALGRIND} tclsh ${TEST}.tcl |& tee ${TEST}.out
  echo
done
