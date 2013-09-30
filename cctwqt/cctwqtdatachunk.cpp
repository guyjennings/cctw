#include "cctwqtdatachunk.h"

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
  if (c) {
    double *d, *w, *id, *iw;

    while (popMergeData(&d, &w))  {
      c -> popMergeData(&id, &iw);

      CctwIntVector3D cks = chunkSize();
      CctwIntVector3D icks = c->chunkSize();

      if (cks == icks) {
        int n = cks.volume();

        if (d && id) {
          for (int i=0; i<n; i++) {
            d[i] += id[i];
          }
        } else if (id) {
          d = id;
          id = NULL;
        }

        if (w && iw) {
          for (int i=0; i<n; i++) {
            w[i] += iw[i];
          }
        } else if (iw) {
          w = iw;
          iw = NULL;
        }
      }

      pushMergeData(d, w);

      delete[] id;
      delete[] iw;

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
