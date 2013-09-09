#include "cctwqtchunkeddata.h"

CctwqtChunkedData::CctwqtChunkedData(CctwChunkedDataInterface *interface, CctwqtDataFrameManager *manager, QObject *parent) :
//  CctwqtObject(parent),
  m_Interface(interface),
  m_Manager(manager)
{
}

void CctwqtChunkedData::allocateChunks()
{
  int n = m_Interface->chunkCount().volume();

  m_DataChunks.resize(n);

  for (int i=0; i<n; i++) {
    m_DataChunks[i] = new CctwqtDataChunk(m_Interface -> chunkIndexFromNumber(i),
                                          m_Interface, m_Manager, NULL /*this*/);
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
  int n = m_Interface -> chunkNumberFromIndex(f);

  if (n >= 0 && n < m_DataChunks.count()) {
    m_DataChunks[n] ->addDependency(t);
  }
}

CctwqtDataChunk *CctwqtChunkedData::chunk(CctwIntVector3D idx)
{
  int n = m_Interface -> chunkNumberFromIndex(idx);

  if (n >= 0 && n < m_DataChunks.count()) {
    return m_DataChunks[n];
  } else {
    return NULL;
  }
}
