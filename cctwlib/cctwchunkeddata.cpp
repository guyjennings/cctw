#include "cctwchunkeddata.h"
#include "cctwdatachunk.h"
#include "cctwapplication.h"
#include <QUrlQuery>
#include "cctwdebug.h"

CctwChunkedData::CctwChunkedData
  (CctwApplication *application,
   CctwIntVector3D  dim,
   CctwIntVector3D  chunkSize,
   QString          name,
   QObject         *parent)
  :CctwObject(name, parent),
    m_Dimensions(dim),
    m_ChunkSize(chunkSize),
    m_ChunkCount((dim-CctwIntVector3D(1,1,1))/chunkSize + CctwIntVector3D(1,1,1)),
    m_DataFileName(application->saver(), this, "dataFileName", "input.h5", "HDF5 Data File Name"),
    m_DataSetName(application->saver(), this, "dataSetName", "data", "HDF5 Dataset name"),
//    m_Dimensions(application->saver(), this, "dimensions", dim, "Dataset Dimensions"),
//    m_ChunkSize(application->saver(), this, "chunkSize", chunkSize, "Chunk Size"),
//    m_ChunkCount(application->saver(), this, "chunkCount", (dim-CctwIntVector3D(1,1,1))/chunkSize + CctwIntVector3D(1,1,1), "Chunk Count"),
    m_Compression(application->saver(), this, "compression", 0, "Compression Level")
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
                                          tr("chunk-%1").arg(i), parent());
  }
}

void CctwChunkedData::setDimensions(CctwIntVector3D dim)
{
  m_Dimensions = dim;

  m_ChunkCount = ((m_Dimensions-CctwIntVector3D(1,1,1))/m_ChunkSize+CctwIntVector3D(1,1,1));

  allocateChunks();
}

void CctwChunkedData::setChunkSize(CctwIntVector3D cksz)
{
  m_ChunkSize = cksz;

  m_ChunkCount = ((m_Dimensions-CctwIntVector3D(1,1,1))/m_ChunkSize+CctwIntVector3D(1,1,1));

  allocateChunks();
}

void                CctwChunkedData::setDataSource(QString desc)
{
  printMessage(tr("%1:setDataSource(\"%2\")").arg(get_Name()).arg(CctwApplication::addSlashes(desc)));

  QUrl url(desc);

//  if (qcepDebug(DEBUG_APP)) {
    printMessage(tr("scheme:   %1").arg(url.scheme()));
    printMessage(tr("host:     %1").arg(url.host()));
    printMessage(tr("path:     %1").arg(url.path()));
    printMessage(tr("filename: %1").arg(url.fileName()));
    printMessage(tr("query:    %1").arg(url.query()));
    printMessage(tr("fragment: %1").arg(url.fragment()));
//  }

  if (url.hasQuery()) {
    QUrlQuery qry(url);

    QList <QPair <QString, QString> > l = qry.queryItems();

    QPair<QString, QString> v;
    foreach (v, l) {
//      if (qcepDebug(DEBUG_APP)) {
        printMessage(tr(" key:     %1").arg(v.first));
        printMessage(tr(" val:     %1").arg(v.second));
//      }
    }
  }
}

bool                CctwChunkedData::containsPixel(CctwIntVector3D pixelCoord)
{
  return pixelCoord.x() >= 0 &&
      pixelCoord.y() >= 0 &&
      pixelCoord.z() >= 0 &&
      pixelCoord.x() < m_Dimensions.x() &&
      pixelCoord.y() < m_Dimensions.y() &&
      pixelCoord.z() < m_Dimensions.z();
}

CctwIntVector3D     CctwChunkedData::chunkStart(CctwIntVector3D chunkIdx)
{
  // Return pixel coords of start of chunk chunkIdx

  return m_ChunkSize*chunkIdx;
}

CctwIntVector3D     CctwChunkedData::chunkIndex(CctwIntVector3D pixelCoord)
{
  // Return index of chunk containing given pixel

  return pixelCoord / m_ChunkSize;
}

CctwIntVector3D     CctwChunkedData::chunkIndex(CctwDoubleVector3D fracPixelCoord)
{
  CctwDoubleVector3D fchk = fracPixelCoord / CctwDoubleVector3D(m_ChunkSize.x(), m_ChunkSize.y(), m_ChunkSize.z());

  return CctwIntVector3D(fchk.x(), fchk.y(), fchk.z());
}

CctwIntVector3D     CctwChunkedData::chunkCount() const
{
  return m_ChunkCount;
}

bool CctwChunkedData::containsChunk(CctwIntVector3D chunkIdx)
{
  return chunkIdx.x()>=0 && chunkIdx.x()<m_ChunkCount.x() &&
         chunkIdx.y()>=0 && chunkIdx.y()<m_ChunkCount.y() &&
         chunkIdx.z()>=0 && chunkIdx.z()<m_ChunkCount.z();
}

int CctwChunkedData::chunkNumberFromIndex(CctwIntVector3D chunkIdx)
{
  if (containsChunk(chunkIdx)) {
    int xstride = 1;
    int ystride = m_ChunkCount.x();
    int zstride = m_ChunkCount.x()*m_ChunkCount.y();

    return chunkIdx.x()*xstride + chunkIdx.y()*ystride + chunkIdx.z()*zstride;
  } else {
    return -1;
  }
}

CctwIntVector3D CctwChunkedData::chunkIndexFromNumber(int n)
{
  if (n>=0 && n<m_ChunkCount.volume()) {
    int xstride = 1;
    int ystride = m_ChunkCount.x();
    int zstride = m_ChunkCount.x()*m_ChunkCount.y();

    int z = n / zstride;

    n %= zstride;

    int y = n / ystride;

    n %= ystride;

    int x = n / xstride;

    return CctwIntVector3D(x,y,z);
  } else {
    return CctwIntVector3D(-1,-1,-1);
  }
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

CctwDataChunk *CctwChunkedData::chunk(int n)
{
  if (n >= 0 && n < m_DataChunks.count()) {
    CctwDataChunk *chunk = m_DataChunks[n];

    if (chunk && chunkNumberFromIndex(chunk->index()) != n) {
      printMessage(tr("Chunk anomaly"));
    }
    return chunk;
  } else {
    return NULL;
  }
}

CctwDataChunk *CctwChunkedData::chunk(CctwIntVector3D idx)
{
  return chunk(chunkNumberFromIndex(idx));
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
