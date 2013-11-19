#include "cctwswiftinputdata.h"
#include <stdlib.h>

CctwSwiftInputData::CctwSwiftInputData(CctwIntVector3D dim,        // Data set dimension
                                       CctwIntVector3D chunkSize,  // Chunk size
                                       CctwqtInputDataFrameManager *manager,
                                       QObject *parent)
  : CctwInputDataInterface(dim, chunkSize, manager, parent)
{
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
