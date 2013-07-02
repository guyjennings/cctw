#include "cctwswiftinputdata.h"
#include <stdlib.h>

CctwSwiftInputData::CctwSwiftInputData()
{
}

int CctwSwiftInputData::useChunk(int nx, int ny, int nz)
{
  return -1;
}

double CctwSwiftInputData::readData(int chunkId, int dx, int dy, int dz)
{
  ::abort();
}

void CctwSwiftInputData::releaseChunk(int chunkId)
{
}
