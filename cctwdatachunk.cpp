#include "cctwdatachunk.h"
#include "cctwchunkeddatainterface.h"

#ifdef USE_QT

CctwDataChunk::CctwDataChunk(CctwChunkedDataInterface *data, CctwChunkIndex index, QObject *parent) :
  CctwqtObject(parent),
  m_Data(data),
  m_ChunkIndex(index),
  m_ChunkData(NULL),
  m_ChunkWeights(NULL)
{
}

#else

CctwDataChunk::CctwDataChunk() :
  m_Data(data),
  m_ChunkIndex(index),
  m_ChunkData(NULL),
  m_ChunkWeights(NULL)
{
}

#endif

CctwDataChunk::~CctwDataChunk()
{
  delete [] m_ChunkData;
  delete [] m_ChunkWeights;
}

int CctwDataChunk::allocateData()
{
  double *newData = NULL;
  int cksz = m_Data->chunkSize().volume();

  newData = new double[cksz];

  for (int i=0; i<cksz; i++) {
    newData[i] = 0;
  }

  delete [] m_ChunkData;

  m_ChunkData = newData;

  return cksz*sizeof(double);
}

int CctwDataChunk::allocateWeights()
{
  double *newWeights = NULL;
  int cksz = m_Data->chunkSize().volume();

  newWeights = new double[cksz];

  for (int i=0; i<cksz; i++) {
    newWeights[i] = 0;
  }

  delete [] m_ChunkWeights;

  m_ChunkWeights = newWeights;

  return cksz*sizeof(double);
}

int CctwDataChunk::deallocateData()
{
  int res = m_ChunkData ? m_Data->chunkSize().volume()*sizeof(double) : 0;

  delete [] m_ChunkData;

  m_ChunkData = NULL;

  return res;
}

int CctwDataChunk::deallocateWeights()
{
  int res = m_ChunkWeights ? m_Data->chunkSize().volume()*sizeof(double) : 0;

  delete [] m_ChunkWeights;

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
