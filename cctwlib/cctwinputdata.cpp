#include "cctwinputdata.h"
#include "cctwinputdataframemanager.h"

CctwInputData::CctwInputData(CctwIntVector3D dim, CctwIntVector3D chunkSize, /*CctwDoubleVector3D origin, CctwDoubleVector3D scale,*/ CctwInputDataFrameManager *manager, QObject *parent) :
  CctwInputDataInterface(dim, chunkSize, /*origin, scale,*/ manager, parent)
{
}

int CctwInputData::useChunk(int nx, int ny, int nz)
{
  return m_Manager->loadChunk(nx, ny, nz);
}

double CctwInputData::readData(int chunkId, int dx, int dy, int dz)
{
  return 0;
}

void CctwInputData::releaseChunk(int chunkId)
{
  return m_Manager->releaseChunk(chunkId);
}
