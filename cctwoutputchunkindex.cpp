#include "cctwoutputchunkindex.h"
#include "cctwcommandline.h"

CctwOutputChunkIndex::CctwOutputChunkIndex(int nx, int ny, int nz)
  : m_Nx(nx),
    m_Ny(ny),
    m_Nz(nz)
{
}

CctwOutputChunkIndex CctwOutputChunkIndex::createNew(int argc, char *argv[])
{
  int ocx = CctwCommandLine::integerValue(argc, argv, "--ocx", 1);
  int ocy = CctwCommandLine::integerValue(argc, argv, "--ocy", 1);
  int ocz = CctwCommandLine::integerValue(argc, argv, "--ocz", 1);

  return CctwOutputChunkIndex(ocx, ocy, ocz);
}
