#include "cctwqtchunkeddata.h"
#include "cctwqtdatachunk.h"

CctwqtChunkedData::CctwqtChunkedData(CctwIntVector3D dim,
                                     CctwIntVector3D chunkSize,
                                     CctwDoubleVector3D origin,
                                     CctwDoubleVector3D scale,
                                     CctwqtDataFrameManager *manager,
                                     QObject *parent) :
  CctwChunkedDataInterface(dim, chunkSize, origin, scale, parent),
  m_Manager(manager)
{
}

void CctwqtChunkedData::allocateChunks()
{
  int n = chunkCount().volume();

  m_DataChunks.resize(n);

  for (int i=0; i<n; i++) {
    m_DataChunks[i] = new CctwqtDataChunk(chunkIndexFromNumber(i),
                                          this,
                                          m_Manager, NULL /*this*/);
  }
}

void CctwqtChunkedData::clearDependencies()
{
  foreach(CctwqtDataChunk* p, m_DataChunks) {
    if (p) {
      p->clearDependencies();
    }
  }
}

void CctwqtChunkedData::addDependency(CctwIntVector3D f, CctwIntVector3D t)
{
  int n = chunkNumberFromIndex(f);

  if (n >= 0 && n < m_DataChunks.count()) {
    m_DataChunks[n] ->addDependency(t);
  }
}

CctwqtDataChunk *CctwqtChunkedData::chunk(CctwIntVector3D idx)
{
  int n = chunkNumberFromIndex(idx);

  if (n >= 0 && n < m_DataChunks.count()) {
    return m_DataChunks[n];
  } else {
    return NULL;
  }
}
