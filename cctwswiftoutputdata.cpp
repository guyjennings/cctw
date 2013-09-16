#include "cctwswiftoutputdata.h"
#include <stdlib.h>

#ifdef USE_QT

CctwSwiftOutputData::CctwSwiftOutputData(CctwIntVector3D dim,        // Data set dimension
                                         CctwIntVector3D chunkSize,  // Chunk size
//                                         CctwDoubleVector3D origin,
//                                         CctwDoubleVector3D scale,
                                         CctwqtOutputDataFrameManager *manager,
                                         QObject *parent)
  : CctwOutputDataInterface(dim, chunkSize, /*origin, scale,*/ manager, parent)
{
}

#else

CctwSwiftOutputData::CctwSwiftOutputData(CctwIntVector3D dim,        // Data set dimension
                                         CctwIntVector3D chunkSize/*,  // Chunk size
                                         CctwDoubleVector3D origin,
                                         CctwDoubleVector3D scale*/)
  : CctwOutputDataInterface(dim, chunkSize/*, origin, scale*/)
{
}

#endif

//CctwSwiftOutputData *CctwSwiftOutputData::createNew(int argc, char *argv[])
//{
//  return new CctwSwiftOutputData();
//}

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
