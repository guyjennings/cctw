#include "cctwchunkeddata.h"
#include "cctwdatachunk.h"

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
    m_DataChunks[i] = new CctwDataChunk(this,
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
  foreach(CctwDataChunk* p, m_DataChunks) {
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

CctwDataChunk *CctwChunkedData::chunk(CctwIntVector3D idx)
{
  int n = chunkNumberFromIndex(idx);

  if (n >= 0 && n < m_DataChunks.count()) {
    CctwDataChunk *chunk = m_DataChunks[n];

    if (chunk && chunk->index() != idx) {
      printMessage(tr("Chunk anomaly"));
    }
    return chunk;
  } else {
    return NULL;
  }
}

void CctwChunkedData::mergeChunk(CctwDataChunk *chunk)
{
  if (chunk) {
    CctwIntVector3D idx = chunk->index();

    CctwDataChunk *outchunk = this->chunk(idx);

    if (outchunk) {
      outchunk -> mergeChunk(chunk);
    }
  }
}

void CctwChunkedData::clearMergeCounters()
{
  foreach (CctwDataChunk *dc, m_DataChunks) {
    if (dc) {
      dc -> clearMergeCounters();
    }
  }
}
