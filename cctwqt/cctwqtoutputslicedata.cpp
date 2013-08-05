#include "cctwqtoutputslicedata.h"

CctwqtOutputSliceData::CctwqtOutputSliceData(CctwqtDataFrameManager *manager) :
  CctwqtObject(),
  CctwOutputDataInterface(CctwIntVector3D(100,100,1), CctwIntVector3D(10,10,1), CctwDoubleVector3D(0,0,0), CctwDoubleVector3D(1,1,1)),
  m_Manager(manager)
{
  int n = chunkCount().volume();

  m_DataChunks.resize(n);

  for (int i=0; i<n; i++) {
    m_DataChunks[i] = new CctwqtDataChunk(this, manager, this);
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
