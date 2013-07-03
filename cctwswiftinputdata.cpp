#include "cctwswiftinputdata.h"
#include <stdlib.h>

CctwSwiftInputData::CctwSwiftInputData()
{
}

CctwSwiftInputData *CctwSwiftInputData::createNew(int argc, char *argv[])
{
  return new CctwSwiftInputData();
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
