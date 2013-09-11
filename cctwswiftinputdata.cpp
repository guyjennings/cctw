#include "cctwswiftinputdata.h"
#include <stdlib.h>

#ifdef USE_QT
CctwSwiftInputData::CctwSwiftInputData(CctwIntVector3D dim,        // Data set dimension
                                       CctwIntVector3D chunkSize,  // Chunk size
                                       CctwDoubleVector3D origin,  // Real coords of pixel 0,0,0
                                       CctwDoubleVector3D scale,   // Real offset of pixel 1,1,1
                                       CctwqtDataFrameManager *manager,
                                       QObject *parent)
  : CctwInputDataInterface(dim, chunkSize, origin, scale, manager, parent)
{
}
#else
CctwSwiftInputData::CctwSwiftInputData(CctwIntVector3D dim,        // Data set dimension
                                       CctwIntVector3D chunkSize,  // Chunk size
                                       CctwDoubleVector3D origin,  // Real coords of pixel 0,0,0
                                       CctwDoubleVector3D scale)   // Real offset of pixel 1,1,1
  : CctwInputDataInterface(dim, chunkSize, origin, scale)
{
}
#endif

//CctwSwiftInputData *CctwSwiftInputData::createNew(int argc, char *argv[])
//{
//  return new CctwSwiftInputData();
//}

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
