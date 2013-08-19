#include "cctwqtinputdata.h"

CctwqtInputData::CctwqtInputData(CctwqtDataFrameManager *manager, QObject *parent) :
  CctwqtChunkedData(this, manager, parent),
  CctwInputDataInterface(CctwIntVector3D(100,100,100), CctwIntVector3D(10, 10, 10), CctwDoubleVector3D(-5,-5,-5), CctwDoubleVector3D(0.1,0.1,0.1))
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
