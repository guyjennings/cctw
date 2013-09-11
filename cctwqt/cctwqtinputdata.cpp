#include "cctwqtinputdata.h"
#include "cctwqtdataframemanager.h"

CctwqtInputData::CctwqtInputData(CctwIntVector3D dim, CctwIntVector3D chunkSize, CctwDoubleVector3D origin, CctwDoubleVector3D scale, CctwqtDataFrameManager *manager, QObject *parent) :
  CctwInputDataInterface(dim, chunkSize, origin, scale, manager, parent)
{
}

int CctwqtInputData::useChunk(int nx, int ny, int nz)
{
  return m_Manager->loadChunk(nx, ny, nz);
}

double CctwqtInputData::readData(int chunkId, int dx, int dy, int dz)
{
  return 0;
}

void CctwqtInputData::releaseChunk(int chunkId)
{
  return m_Manager->releaseChunk(chunkId);
}
