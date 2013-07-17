#include "cctwswiftoutputdata.h"
#include <stdlib.h>

CctwSwiftOutputData::CctwSwiftOutputData()
  : CctwOutputDataInterface(CctwIntVector3D(2048,2048,2048),
                            CctwIntVector3D(100,100,100),
                            CctwDoubleVector3D(-102.4,-102.4,-102.4),
                            CctwDoubleVector3D(0.1,0.1,0.1))
{
}

CctwSwiftOutputData *CctwSwiftOutputData::createNew(int argc, char *argv[])
{
  return new CctwSwiftOutputData();
}

int CctwSwiftOutputData::useChunk(int nx, int ny, int nz)
{
  return -1;
}

void CctwSwiftOutputData::writeData(int chunkId, int dx, int dy, int dz, double value)
{
  ::abort();
}

void CctwSwiftOutputData::releaseChunk(int chunkId)
{
}
