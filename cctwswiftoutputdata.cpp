#include "cctwswiftoutputdata.h"
#include <stdlib.h>

CctwSwiftOutputData::CctwSwiftOutputData()
{
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
