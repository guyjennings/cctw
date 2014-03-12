#include "cctwchunkeddatainterface.h"
#include <math.h>
#include "cctwapplication.h"

CctwChunkedDataInterface::CctwChunkedDataInterface
  (CctwApplication *application, CctwIntVector3D dim,
   CctwIntVector3D chunkSize, QString name,
   QObject *parent)
  : CctwObject(name, parent),
    m_Dimensions(dim),
    m_ChunkSize(chunkSize),
    m_ChunkCount((dim-CctwIntVector3D(1,1,1))/chunkSize + CctwIntVector3D(1,1,1)),
    m_DataFileName(application->saver(), this, "dataFileName", "input.h5", "HDF5 Data File Name"),
    m_DataSetName(application->saver(), this, "dataSetName", "data", "HDF5 Dataset name"),
//    m_Dimensions(application->saver(), this, "dimensions", dim, "Dataset Dimensions"),
//    m_ChunkSize(application->saver(), this, "chunkSize", chunkSize, "Chunk Size"),
//    m_ChunkCount(application->saver(), this, "chunkCount", (dim-CctwIntVector3D(1,1,1))/chunkSize + CctwIntVector3D(1,1,1), "Chunk Count"),
    m_Compression(application->saver(), this, "compression", 0, "Compression Level")
{
}

void                CctwChunkedDataInterface::initialize(void *buffer)
{
}

bool                CctwChunkedDataInterface::containsPixel(CctwIntVector3D pixelCoord)
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

bool CctwChunkedDataInterface::containsChunk(CctwIntVector3D chunkIdx)
{
  return chunkIdx.x()>=0 && chunkIdx.x()<m_ChunkCount.x() &&
         chunkIdx.y()>=0 && chunkIdx.y()<m_ChunkCount.y() &&
         chunkIdx.z()>=0 && chunkIdx.z()<m_ChunkCount.z();
}

int CctwChunkedDataInterface::chunkNumberFromIndex(CctwIntVector3D chunkIdx)
{
  if (containsChunk(chunkIdx)) {
    int xstride = 1;
    int ystride = m_ChunkCount.x();
    int zstride = m_ChunkCount.x()*m_ChunkCount.y();

    return chunkIdx.x()*xstride + chunkIdx.y()*ystride + chunkIdx.z()*zstride;
  } else {
    return -1;
  }
}

CctwIntVector3D CctwChunkedDataInterface::chunkIndexFromNumber(int n)
{
  if (n>=0 && n<m_ChunkCount.volume()) {
    int xstride = 1;
    int ystride = m_ChunkCount.x();
    int zstride = m_ChunkCount.x()*m_ChunkCount.y();

    int z = n / zstride;

    n %= zstride;

    int y = n / ystride;

    n %= ystride;

    int x = n / xstride;

    return CctwIntVector3D(x,y,z);
  } else {
    return CctwIntVector3D(-1,-1,-1);
  }
}

void CctwChunkedDataInterface::setDimensions(CctwIntVector3D dim)
{
  m_Dimensions = dim;

  m_ChunkCount = ((m_Dimensions-CctwIntVector3D(1,1,1))/m_ChunkSize+CctwIntVector3D(1,1,1));
}

void CctwChunkedDataInterface::setChunkSize(CctwIntVector3D cksz)
{
  m_ChunkSize = cksz;

  m_ChunkCount = ((m_Dimensions-CctwIntVector3D(1,1,1))/m_ChunkSize+CctwIntVector3D(1,1,1));
}
