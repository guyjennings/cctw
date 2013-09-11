#include "cctwqtoutputslicedata.h"

CctwqtOutputSliceData::CctwqtOutputSliceData(CctwIntVector3D dim,        // Data set dimension
                                             CctwIntVector3D chunkSize,  // Chunk size
                                             CctwDoubleVector3D origin,
                                             CctwDoubleVector3D scale,
                                             CctwqtDataFrameManager *manager,
                                             QObject *parent) :
  CctwOutputDataInterface(dim, chunkSize, origin, scale, manager, parent)
{
}

int CctwqtOutputSliceData::useChunk(int nx, int ny, int nz)
{
  return -1;
}

void CctwqtOutputSliceData::writeData(int chunkId, int dx, int dy, int dz, double value)
{
}

void CctwqtOutputSliceData::releaseChunk(int chunkId)
{
}
