#include "cctwqtoutputdata.h"

CctwqtOutputData::CctwqtOutputData(CctwIntVector3D dim,        // Data set dimension
                                   CctwIntVector3D chunkSize,  // Chunk size
//                                   CctwDoubleVector3D origin,
//                                   CctwDoubleVector3D scale,
                                   CctwqtOutputDataFrameManager *manager,
                                   QObject *parent) :
  CctwOutputDataInterface(dim, chunkSize, /*origin, scale,*/ manager, parent)
{
}

int CctwqtOutputData::useChunk(int nx, int ny, int nz)
{
  return -1;
}

void CctwqtOutputData::writeData(int chunkId, int dx, int dy, int dz, double value)
{
}

void CctwqtOutputData::releaseChunk(int chunkId)
{
}

void CctwqtOutputData::beginTransform()
{
  m_Manager -> beginTransform();
}

void CctwqtOutputData::endTransform()
{
  m_Manager -> endTransform();
}
