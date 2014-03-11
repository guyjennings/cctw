#include "cctwdatachunk.h"
#include "cctwchunkeddatainterface.h"
#include <stdio.h>
#include <QSemaphore>
#include "cctwthread.h"

CctwDataChunk::CctwDataChunk(CctwChunkedData *data, CctwIntVector3D index, CctwDataFrameManager *manager, QString name, QObject *parent) :
  CctwObject(name, parent),
  m_Data(data),
  m_Manager(manager),
  m_ChunkIndex(index),
  m_ChunkData(NULL),
  m_ChunkWeights(NULL),
  m_Normalized(0),
  m_DataWritten(0),
  m_WeightsWritten(0),
  m_MergeCounter(0)
{
  int n = m_Data->chunkCount().z();

  m_DataFrames.resize(n);
}

CctwDataChunk::~CctwDataChunk()
{
  if (m_ChunkData || m_ChunkWeights) {
    printf("Anomaly\n");
  }

//  delete [] m_ChunkData;
//  delete [] m_ChunkWeights;

  releaseBuffer(m_ChunkData);
  releaseBuffer(m_ChunkWeights);

  m_ChunkData = NULL;
  m_ChunkWeights = NULL;
}

static QAtomicInt g_MaxAllocated(0);
static QAtomicInt g_Allocated(0);
static QAtomicInt g_AllocationLimit(1000);
static QSemaphore g_Available(1000);

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
  int res = allocateData();

  CctwIntVector3D size = m_Data->chunkSize();
  CctwIntVector3D start = m_ChunkIndex * size;

  for (int k=0; k<size.z(); k++) {
    for (int j=0; j<size.y(); j++) {
      for (int i=0; i<size.x(); i++) {
        CctwIntVector3D coords(i,j,k);

        int val = ((start.x()+i)/16 & 1) ^ ((start.y()+j)/16 & 1) ^ ((start.z()+k)/16 & 1);

        setData(coords, val+0.75);
      }
    }
  }

  return res;
}

int CctwDataChunk::readWeights()
{
  int res = allocateWeights();

  CctwIntVector3D size = m_Data->chunkSize();

  for (int k=0; k<size.z(); k++) {
    for (int j=0; j<size.y(); j++) {
      for (int i=0; i<size.x(); i++) {
        CctwIntVector3D coords(i,j,k);

        int val = 1;

        setWeight(coords, val);
      }
    }
  }

  return res;
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
  normalize();

  if (m_Manager) {
    m_Manager->writeChunk(this);
  }

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

double *CctwDataChunk::allocateBuffer()
{
  int cksz = m_Data->chunkSize().volume();

  int nalloc = g_Allocated.fetchAndAddOrdered(1);

  if (nalloc > g_MaxAllocated.fetchAndAddOrdered(0)) {
    g_MaxAllocated.fetchAndStoreOrdered(nalloc);
  }

//  printMessage(tr("Acquired 1 blocks, %1 allocated, %2 max")
//               .arg(nalloc).arg(g_MaxAllocated.fetchAndAddOrdered(0)));

  double *res = new double[cksz];

  for (int i=0; i<cksz; i++) {
    res[i] = 0;
  }

  return res;
}

void CctwDataChunk::releaseBuffer(double *buffer)
{
  if (buffer) {
    int nalloc = g_Allocated.fetchAndAddOrdered(-1);

//    printMessage(tr("Releasing 1 blocks, %1 allocated").arg(nalloc));

    delete [] buffer;
  }
}

void CctwDataChunk::waitForData()
{
  int nbuff = dependencyCount();

  if (nbuff > g_AllocationLimit.fetchAndAddOrdered(0)) {
    printMessage(tr("Trying to allocate too many blocks - will sleep 5 secs then proceed anyway"));

    CctwThread::sleep(5);
  } else {
//    printMessage(tr("Trying to acquire %1 blocks, %2 available").arg(nbuff).arg(g_Available.available()));

    if (!g_Available.tryAcquire(nbuff)) {
      printMessage(tr("Failed to acquire %1 blocks").arg(nbuff));

      g_Available.acquire(nbuff);
    }
  }
}

void CctwDataChunk::finishedWithData()
{
  int nbuff = dependencyCount();

  if (nbuff > g_AllocationLimit.fetchAndAddOrdered(0)) {
    printMessage(tr("Skipped release"));
  } else {
    g_Available.release(nbuff);

//    printMessage(tr("Releasing %1 blocks, %2 available").arg(nbuff).arg(g_Available.available()));
  }
}

CctwIntVector3D CctwDataChunk::index() const
{
  return m_ChunkIndex;
}

void CctwDataChunk::clearDependencies()
{
  QMutexLocker lock(&m_DependenciesLock);

  m_Dependencies.resize(0);
}

void CctwDataChunk::sortDependencies()
{
  qSort(m_Dependencies.begin(), m_Dependencies.end());
}

int  CctwDataChunk::dependencyCount() const
{
  return m_Dependencies.count();
}

CctwIntVector3D CctwDataChunk::dependency(int n) const
{
  return m_Dependencies.value(n);
}

void CctwDataChunk::addDependency(CctwIntVector3D dep)
{
  QMutexLocker lock(&m_DependenciesLock);

  if (!m_Dependencies.contains(dep)) {
//    printMessage(tr("Added dependency from chunk [%1,%2,%3] to chunk [%4,%5,%6]")
//                 .arg(m_ChunkIndex.x()).arg(m_ChunkIndex.y()).arg(m_ChunkIndex.z())
//                 .arg(dep.x()).arg(dep.y()).arg(dep.z()));

    m_Dependencies.append(dep);
  }
}

void CctwDataChunk::reportDependencies()
{
  QMutexLocker lock(&m_DependenciesLock);

  QString msg(tr("[%1,%2,%3] ->").arg(m_ChunkIndex.x()).arg(m_ChunkIndex.y()).arg(m_ChunkIndex.z()));

  sortDependencies();

  foreach (CctwIntVector3D dep, m_Dependencies) {
    msg += tr(" [%4,%5,%6]").arg(dep.x()).arg(dep.y()).arg(dep.z());
  }

  printMessage(msg);
}

void CctwDataChunk::mergeChunk(CctwDataChunk *c)
{
//  printMessage(tr("Merging chunk [%1,%2,%3]")
//               .arg(index().x()).arg(index().y()).arg(index().z()));

  if (c) {
    if (c->index() != index()) {
      printMessage(tr("Merging anomaly [%1,%2,%3] != [%4,%5,%6]")
                   .arg(index().x()).arg(index().y()).arg(index().z())
                   .arg(c->index().x()).arg(c->index().y()).arg(c->index().z()));
    }

    double *d, *w, *id, *iw;
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
      }

      releaseBuffer(d);
      releaseBuffer(w);
    }

    pushMergeData(id, iw);

    incMergeCounters();

    if (mergeCount() == dependencyCount()) {
      printMessage(tr("Output chunk [%1,%2,%3] completed")
                   .arg(index().x()).arg(index().y()).arg(index().z()));
      writeData();
      writeWeights();
      deallocateData();
      deallocateWeights();
    } else if (mergeCount() > dependencyCount()) {
      printMessage(tr("Exceeded expected number of merges for chunk [%1,%2,%3] %4 > %5")
                   .arg(index().x()).arg(index().y()).arg(index().z())
                   .arg(mergeCount()).arg(dependencyCount()));
      deallocateData();
      deallocateWeights();
    }
  }
}

void CctwDataChunk::clearMergeCounters()
{
  m_MergeCounter = 0;
}

void CctwDataChunk::incMergeCounters()
{
  m_MergeCounter++;
}

int CctwDataChunk::mergeCount()
{
  return m_MergeCounter;
}

bool CctwDataChunk::popMergeData(double **data, double **weights)
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

void CctwDataChunk::pushMergeData(double *data, double *weights)
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
