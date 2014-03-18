#include "cctwinputdata.h"

CctwInputData::CctwInputData(CctwApplication *application, CctwIntVector3D dim, CctwIntVector3D chunkSize, QString name, QObject *parent) :
  CctwChunkedData(application, dim, chunkSize, name, parent)
{
}

int CctwInputData::useChunk(int nx, int ny, int nz)
{
//  return m_Manager->loadChunk(nx, ny, nz);
  printMessage(tr("CctwInputData::useChunk(%1,%2,%3)").arg(nx).arg(ny).arg(nz));

  return 0;
}

double CctwInputData::readData(int chunkId, int dx, int dy, int dz)
{
  return 0;
}

//void CctwInputData::releaseChunk(int chunkId)
//{
////  return m_Manager->releaseChunk(chunkId);
//}
