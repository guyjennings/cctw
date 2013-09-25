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
  QMutexLocker lock(&m_MergeLock);

  if (c) {
    CctwIntVector3D cks = chunkSize();
    CctwIntVector3D icks = c->chunkSize();

    if (!dataAllocated()) {
      allocateData();
    }

    if (!weightsAllocated()) {
      allocateWeights();
    }

    if (cks == icks) {
      int n = cks.volume();

      double *d = dataPointer();
      double *id = c->dataPointer();

      if (d && id) {
        for (int i=0; i<n; i++) {
          d[i] += id[i];
        }
      }

      double *w = weightsPointer();
      double *iw = c->weightsPointer();

      if (w && iw) {
        for (int i=0; i<n; i++) {
          w[i] += iw[i];
        }
      }
    }

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
