#include "cctwdatachunk.h"
#include "cctwchunkeddatainterface.h"
#include <stdio.h>

#ifdef USE_QT

CctwDataChunk::CctwDataChunk(CctwChunkedDataInterface *data, CctwIntVector3D index, QObject *parent) :
  CctwqtObject(parent),
  m_Data(data),
  m_ChunkIndex(index),
  m_ChunkData(NULL),
  m_ChunkWeights(NULL),
  m_Normalized(0),
  m_DataWritten(0),
  m_WeightsWritten(0)
{
}

#else

CctwDataChunk::CctwDataChunk(CctwChunkedDataInterface *data, CctwIntVector3D index) :
  m_Data(data),
  m_ChunkIndex(index),
  m_ChunkData(NULL),
  m_ChunkWeights(NULL)
{
}

#endif

CctwDataChunk::~CctwDataChunk()
{
  if (m_ChunkData || m_ChunkWeights) {
    printf("Anomaly\n");
  }

  delete [] m_ChunkData;
  delete [] m_ChunkWeights;
}

double *CctwDataChunk::dataPointer()
{
  return m_ChunkData;
}

double *CctwDataChunk::weightsPointer()
{
  return m_ChunkWeights;
}

bool CctwDataChunk::dataAllocated() const
{
  return m_ChunkData;
}

bool CctwDataChunk::weightsAllocated() const
{
  return m_ChunkWeights;
}

CctwIntVector3D CctwDataChunk::chunkSize()
{
  return m_Data->chunkSize();
}

int CctwDataChunk::readData()
{
  return allocateData();
}

int CctwDataChunk::readWeights()
{
  return allocateWeights();
}

int CctwDataChunk::normalize()
{
  if (m_Normalized) {
    printf("Already normalized\n");
  } else if (m_ChunkData && m_ChunkWeights) {
    int cksz = m_Data->chunkSize().volume();

    for (int i=0; i<cksz; i++) {
      if (m_ChunkWeights[i] != 0) {
        m_ChunkData[i] /= m_ChunkWeights[i];
      }
    }
  }

  m_Normalized = true;

  return 0;
}

int CctwDataChunk::writeData()
{
  return chunkSize().volume()*sizeof(double);
}

int CctwDataChunk::writeWeights()
{
  return chunkSize().volume()*sizeof(double);
}

int CctwDataChunk::allocateData()
{
  int cksz = m_Data->chunkSize().volume();

  double *newData = allocateBuffer();

  releaseBuffer(m_ChunkData);

  m_ChunkData = newData;

  return cksz*sizeof(double);
}

int CctwDataChunk::allocateWeights()
{
  int cksz = m_Data->chunkSize().volume();

  double *newWeights = allocateBuffer();

  releaseBuffer(m_ChunkWeights);

  m_ChunkWeights = newWeights;

  return cksz*sizeof(double);
}

int CctwDataChunk::deallocateData()
{
  int res = m_ChunkData ? m_Data->chunkSize().volume()*sizeof(double) : 0;

  releaseBuffer(m_ChunkData);

  m_ChunkData = NULL;

  return res;
}

int CctwDataChunk::deallocateWeights()
{
  int res = m_ChunkWeights ? m_Data->chunkSize().volume()*sizeof(double) : 0;

  releaseBuffer(m_ChunkWeights);

  m_ChunkWeights = NULL;

  return res;
}

int CctwDataChunk::pixelOffset(CctwIntVector3D localcoords)
{
  CctwIntVector3D cksz = m_Data->chunkSize();

  if (localcoords >= CctwIntVector3D(0,0,0) && localcoords < cksz) {
    int offset = (localcoords.z() * cksz.y() + localcoords.y())*cksz.x() + localcoords.x();
    return offset;
  } else {
    return -1;
  }
}

double CctwDataChunk::data(CctwIntVector3D localcoords)
{
  int offset = pixelOffset(localcoords);

  if (offset >= 0 && m_ChunkData) {
    return m_ChunkData[offset];
  } else {
    return 0;
  }
}

double CctwDataChunk::weight(CctwIntVector3D localcoords)
{
  int offset = pixelOffset(localcoords);

  if (offset >= 0 && m_ChunkWeights) {
    return m_ChunkWeights[offset];
  } else {
    return 1;
  }
}

void CctwDataChunk::setData(CctwIntVector3D localcoords, double val)
{
  int offset = pixelOffset(localcoords);

  if (offset >= 0 && m_ChunkData) {
    m_ChunkData[offset] = val;
  }
}

void CctwDataChunk::setWeight(CctwIntVector3D localcoords, double val)
{
  int offset = pixelOffset(localcoords);

  if (offset >= 0 && m_ChunkWeights) {
    m_ChunkWeights[offset] = val;
  }
}
