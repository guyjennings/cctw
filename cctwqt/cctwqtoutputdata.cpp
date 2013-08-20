#include "cctwqtoutputdata.h"

CctwqtOutputData::CctwqtOutputData(CctwqtDataFrameManager *manager, QObject *parent) :
  CctwqtChunkedData(this, manager, parent),
  CctwOutputDataInterface(CctwIntVector3D(2048,2048,2048), CctwIntVector3D(128,128,128), CctwDoubleVector3D(-5,-5,-5), CctwDoubleVector3D(10.0/2048.0,10.0/2048.0,10.0/2048.0))
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
