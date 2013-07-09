#include "cctwchunkeddatainterface.h"

CctwChunkedDataInterface::CctwChunkedDataInterface(CctwIntVector3D dim, CctwIntVector3D chunkSize, CctwDoubleVector3D origin, CctwDoubleVector3D scale)
  : m_Dimensions(dim),
    m_ChunkSize(chunkSize),
    m_Origin(origin),
    m_Scale(scale)
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

  return CctwIntVector3D(pix.x(), pix.y(), pix.z());
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
