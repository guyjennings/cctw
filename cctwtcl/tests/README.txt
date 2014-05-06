
CCTW TCL TESTS

To run all the tests, just run: 

tests/run-tests.sh

To run under valgrind, first set 

export VALGRIND="valgrind <args>"

TESTS:

0. Tests that a basic call to the CCTW QScript interface works. 
1. Tests that a complete transform may be performed. 
2. Tests that a chunk may be read from a TIF file.
3. Tests that data dependencies may be obtained for a chunk. 
4. A sketch of the whole workflow. 
