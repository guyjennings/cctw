#include "cctwqtoutputdata.h"

CctwqtOutputData::CctwqtOutputData(CctwqtDataFrameManager *manager, QObject *parent) :
  CctwqtChunkedData(this, manager, parent),
  CctwOutputDataInterface(CctwIntVector3D(100,100,100), CctwIntVector3D(10,10,10), CctwDoubleVector3D(-5,-5,-5), CctwDoubleVector3D(0.1,0.1,0.1))
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
