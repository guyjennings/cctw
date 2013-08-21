#include "cctwqtdatachunk.h"

CctwqtDataChunk::CctwqtDataChunk(CctwIntVector3D idx, CctwChunkedDataInterface *data, CctwqtDataFrameManager *manager, QObject *parent) :
  CctwqtObject(parent),
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

void CctwqtDataChunk::addDependency(CctwIntVector3D dep)
{
  QMutexLocker lock(&m_DependenciesLock);

  if (!m_Dependencies.contains(dep)) {
    printMessage(tr("Added dependency from chunk [%1,%2,%3] to chunk [%4,%5,%6]")
                 .arg(m_ChunkIndex.x()).arg(m_ChunkIndex.y()).arg(m_ChunkIndex.z())
                 .arg(dep.x()).arg(dep.y()).arg(dep.z()));

    m_Dependencies.append(dep);
  }
}

void CctwqtDataChunk::reportDependencies()
{
  QMutexLocker lock(&m_DependenciesLock);

  if (m_Dependencies.count()) {
    qSort(m_Dependencies.begin(), m_Dependencies.end());

    foreach (CctwIntVector3D dep, m_Dependencies) {
      printMessage(tr("[%1,%2,%3] -> [%4,%5,%6]")
                   .arg(m_ChunkIndex.x()).arg(m_ChunkIndex.y()).arg(m_ChunkIndex.z())
                   .arg(dep.x()).arg(dep.y()).arg(dep.z()));
    }
  }
}
