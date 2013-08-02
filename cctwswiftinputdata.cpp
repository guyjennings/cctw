#include "cctwswiftinputdata.h"
#include <stdlib.h>

CctwSwiftInputData::CctwSwiftInputData()
  : CctwInputDataInterface(CctwIntVector3D(2048,2048,3600),
                           CctwIntVector3D(100,100,100),
                           CctwDoubleVector3D(-102.4, -102.4, 0.0),
                           CctwDoubleVector3D(0.1, 0.1, 0.1))
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

  return 0;
}

void CctwSwiftInputData::releaseChunk(int chunkId)
{
}
