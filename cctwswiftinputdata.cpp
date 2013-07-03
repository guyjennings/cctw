#include "cctwswiftinputdata.h"
#include <stdlib.h>

CctwSwiftInputData::CctwSwiftInputData()
  : CctwInputDataInterface(CctwVector3D<int>(2048,2048,3600),
                           CctwVector3D<int>(100,100,100),
                           CctwVector3D<double>(0.1, 0.1, 0.1))
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
