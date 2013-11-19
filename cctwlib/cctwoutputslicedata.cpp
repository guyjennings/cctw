#include "cctwoutputslicedata.h"

CctwOutputSliceData::CctwOutputSliceData(CctwIntVector3D dim,        // Data set dimension
                                             CctwIntVector3D chunkSize,  // Chunk size
//                                             CctwDoubleVector3D origin,
//                                             CctwDoubleVector3D scale,
                                             CctwOutputDataFrameManager *manager,
                                             QObject *parent) :
  CctwOutputDataInterface(dim, chunkSize, /*origin, scale,*/ manager, parent)
{
}

int CctwOutputSliceData::useChunk(int nx, int ny, int nz)
{
  return -1;
}

void CctwOutputSliceData::writeData(int chunkId, int dx, int dy, int dz, double value)
{
}

void CctwOutputSliceData::releaseChunk(int chunkId)
{
}

void CctwOutputSliceData::beginTransform()
{
}

void CctwOutputSliceData::endTransform()
{
}
