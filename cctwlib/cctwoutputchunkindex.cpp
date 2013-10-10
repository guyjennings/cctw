#include "cctwoutputchunkindex.h"
#include "cctwcommandline.h"

CctwOutputChunkIndex::CctwOutputChunkIndex(int nx, int ny, int nz)
  : CctwChunkIndex(nx, ny, nz)
{
}

CctwOutputChunkIndex CctwOutputChunkIndex::createNew(int argc, char *argv[])
{
  int ocx = CctwCommandLine::integerValue(argc, argv, "--ocx", 10);
  int ocy = CctwCommandLine::integerValue(argc, argv, "--ocy", 10);
  int ocz = CctwCommandLine::integerValue(argc, argv, "--ocz", 10);

  return CctwOutputChunkIndex(ocx, ocy, ocz);
}
