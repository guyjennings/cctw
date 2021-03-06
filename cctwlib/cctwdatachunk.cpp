#include "cctwdatachunk.h"
#include <stdio.h>
#include <QSemaphore>
#include "cctwthread.h"

static QAtomicInt g_ChunkCount;

CctwDataChunk::CctwDataChunk(CctwChunkedDataWPtr data, int index, QString name, QcepObjectWPtr parent) :
  CctwObject(name, parent),
  m_Data(data),
  m_ChunkIndex(index),
  m_ChunkStart(calculateChunkStart()),
  m_ChunkSize(calculateChunkSize()),
  m_ChunkData(NULL),
  m_ChunkWeights(NULL),
  m_Normalized(0),
  m_DataWritten(0),
  m_WeightsWritten(0),
  m_MergeCounter(0),
  m_OwnData(true),
  m_SkippedPixels(0)
{
  g_ChunkCount.fetchAndAddOrdered(1);
}

CctwIntVector3D CctwDataChunk::calculateChunkStart()
{
//  printMessage(tr("calculateChunkStart(): %1").arg(m_ChunkIndex));
  CctwChunkedDataPtr data(m_Data);

  if (data) {
    return data -> chunkStart(m_ChunkIndex);
  } else {
    printMessage(tr("Chunk %1, m_Data == NULL").arg(m_ChunkIndex));
    return CctwIntVector3D(0,0,0);
  }
}

CctwIntVector3D CctwDataChunk::calculateChunkSize()
{
  CctwChunkedDataPtr data(m_Data);

  if (data) {
    CctwIntVector3D chksize = data->chunkSize();
    CctwIntVector3D chkend  = m_ChunkStart + chksize;
    CctwIntVector3D dim     = data->dimensions();

    if (chkend.x() > dim.x()) {
      chksize.x() = dim.x() - m_ChunkStart.x();
    }

    if (chkend.y() > dim.y()) {
      chksize.y() = dim.y() - m_ChunkStart.y();
    }

    if (chkend.z() > dim.z()) {
      chksize.z() = dim.z() - m_ChunkStart.z();
    }

    return chksize;
  } else {
    printMessage(tr("Chunk %1, m_Data == NULL").arg(m_ChunkIndex));
    return CctwIntVector3D(0,0,0);
  }
}

CctwDataChunk::~CctwDataChunk()
{
  if (m_OwnData) {
    releaseBuffer(m_ChunkData);
    releaseBuffer(m_ChunkWeights);
  }

  m_ChunkData = NULL;
  m_ChunkWeights = NULL;

  g_ChunkCount.fetchAndAddOrdered(-1);
}

int CctwDataChunk::allocatedChunkCount()
{
  return g_ChunkCount.fetchAndAddOrdered(0);
}

static QAtomicInt g_MaxAllocated(0);
static QAtomicInt g_Allocated(0);
static QAtomicInt g_AllocationLimit(1000);
static QSemaphore g_Available(1000);

CctwChunkedData::MergeDataType *CctwDataChunk::dataPointer()
{
  return m_ChunkData;
}

CctwChunkedData::MergeDataType *CctwDataChunk::weightsPointer()
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

CctwIntVector3D CctwDataChunk::chunkStart()
{
  return m_ChunkStart;
}

void CctwDataChunk::resetChunkStart()
{
  m_ChunkStart = calculateChunkStart();
  qDebug("CctwDataChunk::resetChunkStart(): %s", qPrintable(m_ChunkStart.toString()));
}

CctwIntVector3D CctwDataChunk::chunkSize()
{
  return m_ChunkSize;
}

void CctwDataChunk::setChunkSize(CctwIntVector3D size)
{
  m_ChunkSize = size;
}

int CctwDataChunk::allocateData()
{
  int cksz = m_ChunkSize.volume();

  CctwChunkedData::MergeDataType *newData = allocateBuffer();

  releaseBuffer(m_ChunkData);

  m_ChunkData = newData;

  return cksz*sizeof(CctwChunkedData::MergeDataType);
}

int CctwDataChunk::allocateWeights()
{
  int cksz = m_ChunkSize.volume();

  CctwChunkedData::MergeDataType *newWeights = allocateBuffer();

  releaseBuffer(m_ChunkWeights);

  m_ChunkWeights = newWeights;

  return cksz*sizeof(CctwChunkedData::MergeDataType);
}

int CctwDataChunk::deallocateData()
{
  int res = m_ChunkData ? m_ChunkSize.volume()*sizeof(CctwChunkedData::MergeDataType) : 0;

  releaseBuffer(m_ChunkData);

  m_ChunkData = NULL;

  return res;
}

int CctwDataChunk::deallocateWeights()
{
  int res = m_ChunkWeights ? m_ChunkSize.volume()*sizeof(CctwChunkedData::MergeDataType) : 0;

  releaseBuffer(m_ChunkWeights);

  m_ChunkWeights = NULL;

  return res;
}

int CctwDataChunk::detachData()
{
  m_ChunkData = NULL;

  return 0;
}

int CctwDataChunk::detachWeights()
{
  m_ChunkWeights = NULL;

  return 0;
}

int CctwDataChunk::pixelOffset(int lx, int ly, int lz)
{
  if (lx < 0 || lx >= m_ChunkSize.x()) {
    return -1;
  } else if (ly < 0 || ly >= m_ChunkSize.y()) {
    return -1;
  } else if (lz < 0 || lz >= m_ChunkSize.z()) {
    return -1;
  } else {
    int offset = (lz * m_ChunkSize.y() + ly)*m_ChunkSize.x() + lx;
    return offset;
  }
}

CctwChunkedData::MergeDataType CctwDataChunk::data(int lx, int ly, int lz)
{
  int offset = pixelOffset(lx, ly, lz);

  if (offset >= 0 && m_ChunkData) {
    return m_ChunkData[offset];
  } else {
    return 0;
  }
}

CctwChunkedData::MergeDataType CctwDataChunk::weight(int lx, int ly, int lz)
{
  int offset = pixelOffset(lx, ly, lz);

  if (offset >= 0 && m_ChunkWeights) {
    return m_ChunkWeights[offset];
  } else {
    return 1;
  }
}

void CctwDataChunk::setData(int lx, int ly, int lz, CctwChunkedData::MergeDataType val)
{
  int offset = pixelOffset(lx, ly, lz);

  if (offset >= 0 && m_ChunkData) {
    m_ChunkData[offset] = val;
  }
}

void CctwDataChunk::setWeight(int lx, int ly, int lz, CctwChunkedData::MergeDataType val)
{
  int offset = pixelOffset(lx, ly, lz);

  if (offset >= 0 && m_ChunkWeights) {
    m_ChunkWeights[offset] = val;
  }
}

void CctwDataChunk::resetAllocationLimits(int nmax)
{
  g_Allocated.fetchAndStoreOrdered(0);
  g_MaxAllocated.fetchAndStoreOrdered(0);
  g_AllocationLimit.fetchAndStoreOrdered(nmax);
  g_Available.acquire(g_Available.available());
  g_Available.release(nmax);
}

int CctwDataChunk::maxAllocated()
{
  return g_MaxAllocated.fetchAndAddOrdered(0);
}

CctwChunkedData::MergeDataType *CctwDataChunk::allocateBuffer()
{
  int cksz = m_ChunkSize.volume();

  int nalloc = g_Allocated.fetchAndAddOrdered(1);

  if (nalloc > g_MaxAllocated.fetchAndAddOrdered(0)) {
    g_MaxAllocated.fetchAndStoreOrdered(nalloc);
  }

//  printMessage(tr("Acquired 1 blocks, %1 allocated, %2 max")
//               .arg(nalloc).arg(g_MaxAllocated.fetchAndAddOrdered(0)));

//  CctwChunkedData::MergeDataType *res = new CctwChunkedData::MergeDataType[cksz];

//  for (int i=0; i<cksz; i++) {
//    res[i] = 0;
//  }

  CctwChunkedData::MergeDataType *res = (CctwChunkedData::MergeDataType*) calloc(sizeof(CctwChunkedData::MergeDataType), cksz);

  return res;
}

void CctwDataChunk::setBuffer(void *buffer)
{
  printMessage(tr("Setting buffer to: %1").arg((long long) buffer));
  m_ChunkData = (CctwChunkedData::MergeDataType*) buffer;
  m_OwnData = false;
}

void CctwDataChunk::releaseBuffer(CctwChunkedData::MergeDataType *buffer)
{
  if (buffer) {
    int nalloc = g_Allocated.fetchAndAddOrdered(-1);

//    printMessage(tr("Releasing 1 blocks, %1 allocated").arg(nalloc));

//    delete [] buffer;

    free(buffer);
  }
}

int CctwDataChunk::index() const
{
  return m_ChunkIndex;
}

void CctwDataChunk::mergeData(CctwChunkedData::MergeDataType *id,
                              CctwChunkedData::MergeDataType *iw,
                              int n)
{
  QMutexLocker lock(&m_MergeLock);

  if (m_ChunkData == NULL) {
    allocateData();
  }

  if (m_ChunkWeights == NULL) {
    allocateWeights();
  }

  CctwChunkedData::MergeDataType *d = m_ChunkData, *w = m_ChunkWeights;

  if (d && id) {
    for (int i=0; i<n; i++) {
      if ((iw && iw[i] != 0 && iw[i] == iw[i]) || !iw) {
        d[i] += id[i];
      }
    }
  } else {
    printMessage(tr("Couldn't merge data for chunk [%1]").arg(index()));
  }

  if (w && iw) {
    for (int i=0; i<n; i++) {
      if ((iw && iw[i] != 0 && iw[i] == iw[i]) || !iw) {
        w[i] += iw[i];
      }
    }
  }
}

void CctwDataChunk::mergeChunk(CctwDataChunk *c)
{
//  printMessage(tr("Merging chunk [%1]")
//               .arg(index()));

  if (c) {
    if (mergeCount() == 0) {
//      printMessage(tr("Output chunk [%1] started")
//                   .arg(index()));
    }

    if (c->index() != index()) {
      printMessage(tr("Merging anomaly [%1] != [%2]")
                   .arg(index()).arg(c->index()));
    }

    CctwChunkedData::MergeDataType *d, *w, *id, *iw;
    c -> popMergeData(&id, &iw);

    while (popMergeData(&d, &w))  {
//      printMessage(tr("Pop Merge"));

      CctwIntVector3D cks = chunkSize();
      CctwIntVector3D icks = c->chunkSize();

      if (cks == icks) {
        int n = cks.volume();

        if (d && id) {
          for (int i=0; i<n; i++) {
            if ((w && w[i] != 0) || !w) {
              id[i] += d[i];
            }
          }
        } else if (d) {
          id = d;
          d = NULL;
        }

        if (w && iw) {
          for (int i=0; i<n; i++) {
            iw[i] += w[i];
          }
        } else if (w) {
          iw = w;
          w = NULL;
        }
      } else {
        printMessage(tr("Anomaly merging data chunk %1").arg(m_ChunkIndex));
      }

      releaseBuffer(d);
      releaseBuffer(w);
    }

    pushMergeData(id, iw);

    incMergeCounters();

    if (mergeCount() == 1) {
      CctwChunkedDataPtr data(m_Data);

      if (data) {
        data->incChunksHeld(1);
      }
    }
  }
}

void CctwDataChunk::clearMergeCounters()
{
  m_MergeCounter = 0;
  m_Normalized   = 0;
}

void CctwDataChunk::incMergeCounters()
{
  m_MergeCounter++;
}

int CctwDataChunk::mergeCount()
{
  return m_MergeCounter;
}

bool CctwDataChunk::popMergeData(CctwChunkedData::MergeDataType **data,
                                 CctwChunkedData::MergeDataType **weights)
{
  QMutexLocker lock(&m_MergeLock);

  if (data && weights) {
    *data = m_ChunkData;

    if (m_MergeData.isEmpty()) {
      m_ChunkData = NULL;
    } else {
      m_ChunkData = m_MergeData.takeLast();
    }

    *weights = m_ChunkWeights;

    if (m_MergeWeights.isEmpty()) {
      m_ChunkWeights = NULL;
    } else {
      m_ChunkWeights = m_MergeWeights.takeLast();
    }

    return *data || *weights;
  } else {
    return false;
  }
}

void CctwDataChunk::pushMergeData(CctwChunkedData::MergeDataType *data,
                                  CctwChunkedData::MergeDataType *weights)
{
  QMutexLocker lock(&m_MergeLock);

  if (m_ChunkData == NULL) {
    m_ChunkData = data;
  } else {
    m_MergeData.push_back(data);
  }

  if (m_ChunkWeights == NULL) {
    m_ChunkWeights = weights;
  } else {
    m_MergeWeights.push_back(weights);
  }
}

void CctwDataChunk::normalizeChunk()
{
  if (m_Normalized) {
    printMessage(tr("Chunk %1 - Already normalized").arg(index()));
  } else if (m_ChunkData && m_ChunkWeights) {
    int cksz = m_ChunkSize.volume();

    for (int i=0; i<cksz; i++) {
      if (m_ChunkWeights[i] != 0.0) {
        m_ChunkData[i] /= m_ChunkWeights[i];
      }
    }

    deallocateWeights();
  }

  m_Normalized = true;
}

int CctwDataChunk::getSkippedPixels()
{
  return m_SkippedPixels;
}

void CctwDataChunk::setSkippedPixels(int n)
{
  m_SkippedPixels = n;
}
