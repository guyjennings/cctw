#include "cctwinputchunkindex.h"
#include "cctwcommandline.h"

CctwInputChunkIndex::CctwInputChunkIndex(int nx, int ny, int nz)
  : CctwChunkIndex(nx, ny, nz)
{
}

CctwInputChunkIndex CctwInputChunkIndex::createNew(int argc, char *argv[])
{
  int ocx = CctwCommandLine::integerValue(argc, argv, "--icx", 1);
  int ocy = CctwCommandLine::integerValue(argc, argv, "--icy", 1);
  int ocz = CctwCommandLine::integerValue(argc, argv, "--icz", 1);

  return CctwInputChunkIndex(ocx, ocy, ocz);
}
