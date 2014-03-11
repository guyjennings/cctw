#include "cctwinputdata.h"
#include "cctwinputdataframemanager.h"

CctwInputData::CctwInputData(CctwApplication *application, CctwIntVector3D dim, CctwIntVector3D chunkSize, /*CctwDoubleVector3D origin, CctwDoubleVector3D scale,*/ CctwInputDataFrameManager *manager, QString name, QObject *parent) :
  CctwInputDataInterface(application, dim, chunkSize, /*origin, scale,*/ manager, name, parent)
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
