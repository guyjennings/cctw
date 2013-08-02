#include "cctwqtinputdata.h"

CctwqtInputData::CctwqtInputData(QObject *parent) :
  QObject(parent),
  CctwInputDataInterface(CctwIntVector3D(100,100,100), CctwIntVector3D(10, 10, 10), CctwDoubleVector3D(0,0,0), CctwDoubleVector3D(1,1,1))
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
