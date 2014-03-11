#include "cctwoutputdata.h"

CctwOutputData::CctwOutputData(CctwApplication *application, CctwIntVector3D dim,        // Data set dimension
                                   CctwIntVector3D chunkSize,  // Chunk size
//                                   CctwDoubleVector3D origin,
//                                   CctwDoubleVector3D scale,
                                   CctwOutputDataFrameManager *manager, QString name,
                                   QObject *parent) :
  CctwOutputDataInterface(application, dim, chunkSize, /*origin, scale,*/ manager, name, parent)
{
}

int CctwOutputData::useChunk(int nx, int ny, int nz)
{
  return -1;
}

void CctwOutputData::writeData(int chunkId, int dx, int dy, int dz, double value)
{
}

void CctwOutputData::releaseChunk(int chunkId)
{
}

void CctwOutputData::beginTransform()
{
  m_Manager -> beginTransform();
}

void CctwOutputData::endTransform()
{
  m_Manager -> endTransform();
}
