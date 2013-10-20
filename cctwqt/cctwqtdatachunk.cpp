#include "cctwqtdatachunk.h"
#include <QSemaphore>
#include "cctwqtthread.h"

CctwqtDataChunk::CctwqtDataChunk(CctwqtChunkedData *data, CctwIntVector3D idx,
                                 CctwqtDataFrameManager *manager,
                                 QObject *parent) :
  CctwDataChunk(data, idx, parent),
  m_ChunkIndex(idx),
  m_Data(data),
  m_Manager(manager),
  m_MergeCounter(0)
{
  int n = m_Data->chunkCount().z();

  m_DataFrames.resize(n);
}

static QAtomicInt g_MaxAllocated(0);
static QAtomicInt g_Allocated(0);
static QAtomicInt g_AllocationLimit(1000);
static QSemaphore g_Available(1000);

CctwqtDataChunk::~CctwqtDataChunk()
{
  releaseBuffer(m_ChunkData);
  releaseBuffer(m_ChunkWeights);

  m_ChunkData = NULL;
  m_ChunkWeights = NULL;
}

void CctwqtDataChunk::resetAllocationLimits(int nmax)
{
  g_Allocated.fetchAndStoreOrdered(0);
  g_MaxAllocated.fetchAndStoreOrdered(0);
  g_AllocationLimit.fetchAndStoreOrdered(nmax);
  g_Available.acquire(g_Available.available());
  g_Available.release(nmax);
}

int CctwqtDataChunk::maxAllocated()
{
  return g_MaxAllocated.fetchAndAddOrdered(0);
}

double *CctwqtDataChunk::allocateBuffer()
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

void CctwqtDataChunk::releaseBuffer(double *buffer)
{
  if (buffer) {
    int nalloc = g_Allocated.fetchAndAddOrdered(-1);

//    printMessage(tr("Releasing 1 blocks, %1 allocated").arg(nalloc));
  }

  delete [] buffer;
}

int CctwqtDataChunk::readData()
{
//  printMessage(tr("Chunk[%1,%2,%3] read data")
//               .arg(index().x()).arg(index().y()).arg(index().z()));

  int res = CctwDataChunk::readData();

  CctwIntVector3D size = m_Data->chunkSize();

  for (int k=0; k<size.z(); k++) {
    for (int j=0; j<size.y(); j++) {
      for (int i=0; i<size.x(); i++) {
        CctwIntVector3D coords(i,j,k);

        int val = (i/16 & 1) ^ (j/16 & 1) ^ (k/16 & 1);

        setData(coords, val+0.75);
      }
    }
  }

  return res;
}

int CctwqtDataChunk::readWeights()
{
//  printMessage(tr("Chunk[%1,%2,%3] read weights")
//               .arg(index().x()).arg(index().y()).arg(index().z()));

  int res = CctwDataChunk::readWeights();

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

void CctwqtDataChunk::waitForData()
{
  int nbuff = dependencyCount();

  if (nbuff > g_AllocationLimit.fetchAndAddOrdered(0)) {
    printMessage(tr("Trying to allocate too many blocks - will sleep 5 secs then proceed anyway"));

    CctwqtThread::sleep(5);
  } else {
//    printMessage(tr("Trying to acquire %1 blocks, %2 available").arg(nbuff).arg(g_Available.available()));

    if (!g_Available.tryAcquire(nbuff)) {
      printMessage(tr("Failed to acquire %1 blocks").arg(nbuff));

      g_Available.acquire(nbuff);
    }
  }
}

void CctwqtDataChunk::finishedWithData()
{
  int nbuff = dependencyCount();

  if (nbuff > g_AllocationLimit.fetchAndAddOrdered(0)) {
    printMessage(tr("Skipped release"));
  } else {
    g_Available.release(nbuff);

//    printMessage(tr("Releasing %1 blocks, %2 available").arg(nbuff).arg(g_Available.available()));
  }
}

CctwIntVector3D CctwqtDataChunk::index() const
{
  return m_ChunkIndex;
}

void CctwqtDataChunk::clearDependencies()
{
  QMutexLocker lock(&m_DependenciesLock);

  m_Dependencies.resize(0);
}

void CctwqtDataChunk::sortDependencies()
{
  qSort(m_Dependencies.begin(), m_Dependencies.end());
}

int  CctwqtDataChunk::dependencyCount() const
{
  return m_Dependencies.count();
}

CctwIntVector3D CctwqtDataChunk::dependency(int n) const
{
  return m_Dependencies.value(n);
}

void CctwqtDataChunk::addDependency(CctwIntVector3D dep)
{
  QMutexLocker lock(&m_DependenciesLock);

  if (!m_Dependencies.contains(dep)) {
//    printMessage(tr("Added dependency from chunk [%1,%2,%3] to chunk [%4,%5,%6]")
//                 .arg(m_ChunkIndex.x()).arg(m_ChunkIndex.y()).arg(m_ChunkIndex.z())
//                 .arg(dep.x()).arg(dep.y()).arg(dep.z()));

    m_Dependencies.append(dep);
  }
}

void CctwqtDataChunk::reportDependencies()
{
  QMutexLocker lock(&m_DependenciesLock);

  QString msg(tr("[%1,%2,%3] ->").arg(m_ChunkIndex.x()).arg(m_ChunkIndex.y()).arg(m_ChunkIndex.z()));

  sortDependencies();

  foreach (CctwIntVector3D dep, m_Dependencies) {
    msg += tr(" [%4,%5,%6]").arg(dep.x()).arg(dep.y()).arg(dep.z());
  }

  printMessage(msg);
}

void CctwqtDataChunk::mergeChunk(CctwqtDataChunk *c)
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

void CctwqtDataChunk::clearMergeCounters()
{
  m_MergeCounter = 0;
}

void CctwqtDataChunk::incMergeCounters()
{
  m_MergeCounter++;
}

int CctwqtDataChunk::mergeCount()
{
  return m_MergeCounter;
}

bool CctwqtDataChunk::popMergeData(double **data, double **weights)
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

void CctwqtDataChunk::pushMergeData(double *data, double *weights)
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

int CctwqtDataChunk::writeData()
{
  normalize();

  if (m_Manager) {
    m_Manager->writeChunk(this);
  }

  return CctwDataChunk::writeData();
}

int CctwqtDataChunk::writeWeights()
{
  return CctwDataChunk::writeWeights();
}
