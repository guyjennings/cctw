#include "cctwqtoutputslicedata.h"

CctwqtOutputSliceData::CctwqtOutputSliceData(CctwqtDataFrameManager *manager) :
  CctwqtObject(),
  CctwOutputDataInterface(CctwIntVector3D(100,100,1), CctwIntVector3D(10,10,1), CctwDoubleVector3D(0,0,0), CctwDoubleVector3D(1,1,1)),
  m_Manager(manager)
{
}

void CctwqtOutputSliceData::initialize()
{
  int n = chunkCount().volume();

  m_DataChunks.resize(n);

  for (int i=0; i<n; i++) {
    m_DataChunks[i] = new CctwqtDataChunk(chunkIndexFromNumber(i), this, m_Manager, this);
  }
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
