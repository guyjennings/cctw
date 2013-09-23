#include "cctwqtdatachunk.h"

CctwqtDataChunk::CctwqtDataChunk(CctwqtChunkedData *data, CctwIntVector3D idx,
                                 CctwqtDataFrameManager *manager,
                                 QObject *parent) :
  CctwDataChunk(data, idx, parent),
  m_ChunkIndex(idx),
  m_Data(data),
  m_Manager(manager)
{
  int n = m_Data->chunkCount().z();

  m_DataFrames.resize(n);
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
