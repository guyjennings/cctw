#include "cctwchunkeddata.h"
#include "cctwqtdatachunk.h"

CctwChunkedData::CctwChunkedData(CctwIntVector3D dim,
                                     CctwIntVector3D chunkSize,
//                                     CctwDoubleVector3D origin,
//                                     CctwDoubleVector3D scale,
                                     CctwDataFrameManager *manager,
                                     QObject *parent) :
  CctwChunkedDataInterface(dim, chunkSize, /*origin, scale,*/ parent),
  m_Manager(manager)
{
}

void CctwChunkedData::allocateChunks()
{
//  printMessage("Allocate chunks");

  int n = chunkCount().volume();

  for(int i=0; i<m_DataChunks.count(); i++) {
    delete m_DataChunks[i];
    m_DataChunks[i] = NULL;
  }

  m_DataChunks.resize(n);

  for (int i=0; i<n; i++) {
    m_DataChunks[i] = new CctwqtDataChunk(this,
                                          chunkIndexFromNumber(i),
                                          m_Manager, parent());
  }
}

void CctwChunkedData::setDimensions(CctwIntVector3D dim)
{
  CctwChunkedDataInterface::setDimensions(dim);

  allocateChunks();
}

void CctwChunkedData::setChunkSize(CctwIntVector3D cksz)
{
  CctwChunkedDataInterface::setChunkSize(cksz);

  allocateChunks();
}

void CctwChunkedData::clearDependencies()
{
  foreach(CctwqtDataChunk* p, m_DataChunks) {
    if (p) {
      p->clearDependencies();
    }
  }
}

void CctwChunkedData::addDependency(CctwIntVector3D f, CctwIntVector3D t)
{
  int n = chunkNumberFromIndex(f);

  if (n >= 0 && n < m_DataChunks.count()) {
    m_DataChunks[n] ->addDependency(t);
  }
}

CctwqtDataChunk *CctwChunkedData::chunk(CctwIntVector3D idx)
{
  int n = chunkNumberFromIndex(idx);

  if (n >= 0 && n < m_DataChunks.count()) {
    CctwqtDataChunk *chunk = m_DataChunks[n];

    if (chunk && chunk->index() != idx) {
      printMessage(tr("Chunk anomaly"));
    }
    return chunk;
  } else {
    return NULL;
  }
}

void CctwChunkedData::mergeChunk(CctwqtDataChunk *chunk)
{
  if (chunk) {
    CctwIntVector3D idx = chunk->index();

    CctwqtDataChunk *outchunk = this->chunk(idx);

    if (outchunk) {
      outchunk -> mergeChunk(chunk);
    }
  }
}

void CctwChunkedData::clearMergeCounters()
{
  foreach (CctwqtDataChunk *dc, m_DataChunks) {
    if (dc) {
      dc -> clearMergeCounters();
    }
  }
}
