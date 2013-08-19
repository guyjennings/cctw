#include "cctwchunkeddatainterface.h"
#include <math.h>

CctwChunkedDataInterface::CctwChunkedDataInterface(CctwIntVector3D dim, CctwIntVector3D chunkSize, CctwDoubleVector3D origin, CctwDoubleVector3D scale)
  : m_Dimensions(dim),
    m_ChunkSize(chunkSize),
    m_Origin(origin),
    m_Scale(scale),
    m_ChunkCount((dim-CctwIntVector3D(1,1,1))/chunkSize+CctwIntVector3D(1,1,1))
{
}

CctwChunkedDataInterface::CctwChunkedDataInterface()
{
}

void                CctwChunkedDataInterface::initialize(void *buffer)
{
}

CctwDoubleVector3D  CctwChunkedDataInterface::toReal(CctwIntVector3D vec)
{
  return m_Origin+m_Scale*CctwDoubleVector3D(vec.x(), vec.y(), vec.z());
}

CctwIntVector3D     CctwChunkedDataInterface::toPixel(CctwDoubleVector3D vec)
{
  CctwDoubleVector3D pix = (vec - m_Origin)/m_Scale;

  return CctwIntVector3D(floor(pix.x()), floor(pix.y()), floor(pix.z()));
}

bool                CctwChunkedDataInterface::contains(CctwIntVector3D pixelCoord)
{
  return pixelCoord.x() >= 0 &&
      pixelCoord.y() >= 0 &&
      pixelCoord.z() >= 0 &&
      pixelCoord.x() < m_Dimensions.x() &&
      pixelCoord.y() < m_Dimensions.y() &&
      pixelCoord.z() < m_Dimensions.z();
}

CctwIntVector3D     CctwChunkedDataInterface::chunkStart(CctwIntVector3D chunkIdx)
{
  // Return pixel coords of start of chunk chunkIdx

  return m_ChunkSize*chunkIdx;
}

CctwIntVector3D     CctwChunkedDataInterface::chunkIndex(CctwIntVector3D pixelCoord)
{
  // Return index of chunk containing given pixel

  return pixelCoord / m_ChunkSize;
}

CctwIntVector3D     CctwChunkedDataInterface::chunkIndex(CctwDoubleVector3D fracPixelCoord)
{
  CctwDoubleVector3D fchk = fracPixelCoord / CctwDoubleVector3D(m_ChunkSize.x(), m_ChunkSize.y(), m_ChunkSize.z());

  return CctwIntVector3D(fchk.x(), fchk.y(), fchk.z());
}

CctwIntVector3D     CctwChunkedDataInterface::chunkCount() const
{
  return m_ChunkCount;
}

CctwIntVector3D     CctwChunkedDataInterface::findChunkIndexContaining(CctwDoubleVector3D coords)
{
  CctwIntVector3D pxlCoords = toPixel(coords);

  return chunkIndex(pxlCoords);
}

int CctwChunkedDataInterface::chunkNumberFromIndex(CctwIntVector3D chunkIdx)
{
  int xstride = 1;
  int ystride = m_ChunkCount.x();
  int zstride = m_ChunkCount.x()*m_ChunkCount.y();

  return chunkIdx.x()*xstride + chunkIdx.y()*ystride + chunkIdx.z()*zstride;
}

CctwIntVector3D CctwChunkedDataInterface::chunkIndexFromNumber(int n)
{
  int xstride = 1;
  int ystride = m_ChunkCount.x();
  int zstride = m_ChunkCount.x()*m_ChunkCount.y();

  int z = n / zstride;

  n %= zstride;

  int y = n / ystride;

  n %= ystride;

  int x = n / xstride;

  return CctwIntVector3D(x,y,z);
}
