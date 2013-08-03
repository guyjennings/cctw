#include "cctwqtoutputslicedata.h"

CctwqtOutputSliceData::CctwqtOutputSliceData(QObject *parent) :
  QObject(parent),
  CctwOutputDataInterface(CctwIntVector3D(100,100,1), CctwIntVector3D(10,10,1), CctwDoubleVector3D(0,0,0), CctwDoubleVector3D(1,1,1))
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
