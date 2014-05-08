#!/bin/zsh -eu

# RUN-TESTS.ZSH (Swift)

# usage: run-tests.zsh [-m] <tests>*
#  -m:    Runs make first
#  tests: Runs each given test file.  If none, run all tests.

TEST_DIR=$( cd $( dirname $0 ) ; /bin/pwd )
CCTWTCL_DIR=$( cd ${TEST_DIR}/../../cctwtcl ; /bin/pwd )
CCTW_DIR=$(    cd ${CCTWTCL_DIR}/.. ; /bin/pwd )

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
  @ stc -u -I .. ${TEST}
  @ turbine -n ${PROCS} ${TEST%.swift}.tcl
done
