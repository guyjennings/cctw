#include "cctwqtinputdata.h"

CctwqtInputData::CctwqtInputData(CctwqtDataFrameManager *manager) :
  QObject(),
  m_Manager(manager),
  CctwInputDataInterface(CctwIntVector3D(100,100,100), CctwIntVector3D(10, 10, 10), CctwDoubleVector3D(0,0,0), CctwDoubleVector3D(1,1,1))
{
  int n = chunkCount().volume();

  m_DataChunks.resize(n);

  for (int i=0; i<n; i++) {
    m_DataChunks[i] = new CctwqtDataChunk(this, manager);
  }
}

int CctwqtInputData::useChunk(int nx, int ny, int nz)
{
  return -1;
}

double CctwqtInputData::readData(int chunkId, int dx, int dy, int dz)
{
  return 0;
}

void CctwqtInputData::releaseChunk(int chunkId)
{
}
