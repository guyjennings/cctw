#include "cctwswiftoutputdata.h"
#include <stdlib.h>

CctwSwiftOutputData::CctwSwiftOutputData(CctwIntVector3D dim,        // Data set dimension
                                         CctwIntVector3D chunkSize,  // Chunk size
                                         CctwqtOutputDataFrameManager *manager,
                                         QObject *parent)
  : CctwOutputDataInterface(dim, chunkSize, manager, parent)
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
