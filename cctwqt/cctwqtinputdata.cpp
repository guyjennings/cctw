#include "cctwqtinputdata.h"

CctwqtInputData::CctwqtInputData(CctwqtDataFrameManager *manager, QObject *parent) :
  CctwqtChunkedData(this, manager, parent),
  CctwInputDataInterface(CctwIntVector3D(2048,2048,2048), CctwIntVector3D(128, 128, 128), CctwDoubleVector3D(-5,-5,-5), CctwDoubleVector3D(10.0/2048.0,10.0/2048.0,10.0/2048.0))
{
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
