#include "cctwchunkeddata.h"
#include "cctwdatachunk.h"
#include "cctwapplication.h"
#include <QDir>

#include "lzf_filter.h"


#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#endif

#include "cctwdebug.h"
#include "qcepmutexlocker.h"

QMutex CctwChunkedData::m_FileAccessMutex;

CctwChunkedData::CctwChunkedData
  (CctwApplication *application,
   CctwIntVector3D  dim,
   CctwIntVector3D  chunkSize,
   bool             isInput,
   QString          name,
   QcepObjectWPtr   parent)
  :CctwObject(name, parent),
    m_Application(application),
    m_IsNeXus(false),
    m_DimensionsCache(dim),
    m_ChunkSizeCache(chunkSize),
    m_ChunkCountCache((dim-CctwIntVector3D(1,1,1))/chunkSize + CctwIntVector3D(1,1,1)),
    m_DataFileName(this, "dataFileName", "input.h5", "HDF5 Data File Name"),
    m_DataSetName(this, "dataSetName", "data", "HDF5 Dataset name"),
    m_MaskDataFileName(this, "maskDataFileName", "", "Mask Data File Name"),
    m_MaskDataSetName(this, "maskDataSetName", "", "Mask Dataset Name"),
    m_Mask(this, "mask", QcepIntVector(), "Mask Image"),
    m_Mask3DDataFileName(this, "mask3DDataFileName", "", "3D-Mask Data File Name"),
    m_Mask3DDataSetName(this, "mask3DDataSetName", "", "3D-Mask Dataset Name"),
    m_AnglesDataFileName(this, "anglesDataFileName", "", "Angles Data File Name"),
    m_AnglesDataSetName(this, "anglesDataSetName", "", "Angles Dataset Name"),
    m_Angles(this, "angles", QcepDoubleVector(), "Angles"),
    m_WeightsDataFileName(this, "weightsDataFileName", "", "Weights Data File Name"),
    m_WeightsDataSetName(this, "weightsDataSetName", "", "Weights Dataset Name"),
    m_Weights(this, "weights", QcepDoubleVector(), "Weights"),
    m_Dimensions(this, "dimensions", m_DimensionsCache, "Dataset Dimensions"),
    m_ChunkSize(this, "chunkSize", m_ChunkSizeCache, "Chunk Size"),
    m_ChunkCount(this, "chunkCount", m_ChunkCountCache, "Chunk Count"),
    m_Compression(this, "compression", 0, "Compression Level"),
    m_Normalization(this, "normalization", 1, "Normalization"),
    m_HDFChunkSize(this, "hdfChunkSize", CctwIntVector3D(0,0,0), "HDF File Chunk Size"),
    m_ChunksRead(this, "chunksRead", 0, "Chunks read from input"),
    m_ChunksWritten(this, "chunksWritten", 0, "Chunks written to output"),
    m_ChunksHeld(this, "chunksHeld", 0, "Chunks held on output"),
    m_ChunksHeldMax(this, "chunksHeldMax", 0, "Max Chunks held on output"),
    m_Mask3DDimensions(this, "mask3DDimensions", CctwIntVector3D(0,0,0), "3D Mask Dimensions"),
    m_IsInput(isInput),
    m_TransformOptions(0),
    m_FileId(-1),
    m_DatasetId(-1),
    m_DataspaceId(-1),
    m_Dataset2Id(-1),
    m_Dataspace2Id(-1),
    m_MaskSameFile(false),
    m_MaskFileId(-1),
    m_MaskDatasetId(-1),
    m_MaskDataspaceId(-1),
    m_Mask3DFileId(-1),
    m_Mask3DDatasetId(-1),
    m_Mask3DDataspaceId(-1),
    m_AnglesSameFile(false),
    m_AnglesFileId(-1),
    m_AnglesDatasetId(-1),
    m_AnglesDataspaceId(-1),
    m_WeightsSameFile(false),
    m_WeightsFileId(-1),
    m_WeightsDatasetId(-1),
    m_WeightsDataspaceId(-1)
{
  allocateChunks();
//  connect(prop_DataFileName(), SIGNAL(valueChanged(QString,int)), this, SLOT(onDataFileNameChanged()));
//  connect(prop_DataSetName(), SIGNAL(valueChanged(QString,int)), this, SLOT(onDataFileNameChanged()));
//  connect(prop_MaskDataFileName(), SIGNAL(valueChanged(QString,int)), this, SLOT(onMaskFileNameChanged()));
//  connect(prop_MaskDataSetName(), SIGNAL(valueChanged(QString,int)), this, SLOT(onMaskFileNameChanged()));
//  connect(prop_AnglesDataFileName(), SIGNAL(valueChanged(QString,int)), this, SLOT(onAnglesChanged()));
//  connect(prop_AnglesDataSetName(), SIGNAL(valueChanged(QString,int)), this, SLOT(onAnglesChanged()));
  connect(this, SIGNAL(dimensionsChanged(CctwIntVector3D)),
          prop_Dimensions(), SLOT(setValue(CctwIntVector3D)), Qt::DirectConnection);
  connect(this, SIGNAL(chunkSizeChanged(CctwIntVector3D)),
          prop_ChunkSize(), SLOT(setValue(CctwIntVector3D)), Qt::DirectConnection);
  connect(this, SIGNAL(chunkCountChanged(CctwIntVector3D)),
          prop_ChunkCount(), SLOT(setValue(CctwIntVector3D)), Qt::DirectConnection);

  connect(prop_Dimensions(), SIGNAL(valueChanged(CctwIntVector3D,int)),
          this, SLOT(sizingChanged()), Qt::DirectConnection);
  connect(prop_ChunkSize(), SIGNAL(valueChanged(CctwIntVector3D,int)),
          this, SLOT(sizingChanged()), Qt::DirectConnection);
}

void CctwChunkedData::allocateChunks()
{
//  printMessage("Allocate chunks");
  QcepMutexLocker lock(__FILE__, __LINE__, &m_ChunkLock);

  int n = chunkCount().volume();

  for(int i=0; i<m_DataChunks.count(); i++) {
    delete m_DataChunks[i];
    m_DataChunks[i] = NULL;
  }

  m_DataChunks.resize(n);

//  for (int i=0; i<n; i++) {
//    m_DataChunks[i] = new CctwDataChunk(this, i, tr("chunk-%1").arg(i), parent());
//  }
}

int CctwChunkedData::allocatedChunkCount()
{
  return CctwDataChunk::allocatedChunkCount();
}

void CctwChunkedData::setDimensions(CctwIntVector3D dim)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("Dimensions set to %1").arg(dim.toString()));
  }

  if (m_DimensionsCache != dim) {
    m_DimensionsCache = dim;

    m_ChunkCountCache = ((m_DimensionsCache-CctwIntVector3D(1,1,1))/m_ChunkSizeCache+CctwIntVector3D(1,1,1));

    allocateChunks();

    set_Dimensions(m_DimensionsCache);
    set_ChunkCount(m_ChunkCountCache);
  }
}

void CctwChunkedData::setChunkSize(CctwIntVector3D cksz)
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage(tr("Chunk Size set to %1").arg(cksz.toString()));
  }

  if (m_ChunkSizeCache != cksz) {
    m_ChunkSizeCache = cksz;

    m_ChunkCountCache = ((m_DimensionsCache-CctwIntVector3D(1,1,1))/m_ChunkSizeCache+CctwIntVector3D(1,1,1));

    allocateChunks();

    set_ChunkSize(m_ChunkSizeCache);
    set_ChunkCount(m_ChunkCountCache);
  }
}

void CctwChunkedData::setMaskDimensions(int mx, int my)
{
  QcepIntVector m = get_Mask();

  m.resize(mx*my);

  set_Mask(m);
}

void CctwChunkedData::setAnglesDimensions(int n)
{
  QcepDoubleVector a = get_Angles();

  a.resize(n);

  set_Angles(a);
}

void CctwChunkedData::setWeightsDimensions(int n)
{
  QcepDoubleVector a = get_Weights();

  a.resize(n);

  set_Weights(a);
}

void CctwChunkedData::setAngle(int n, double v)
{
  QcepDoubleVector a = get_Angles();

  if (n >= 0 && n < a.size()) {
    a.replace(n, v);
  }

  set_Angles(a);
}

double CctwChunkedData::angle(int n)
{
  return get_Angles().value(n);
}

double CctwChunkedData::weight(int n)
{
  return get_Weights().value(n);
}


void CctwChunkedData::setWeight(int n, double v)
{
  QcepDoubleVector a = get_Weights();

  if (n >= 0 && n < a.size()) {
    a.replace(n, v);
  }

  set_Weights(a);
}

void CctwChunkedData::sizingChanged()
{
  if (qcepDebug(DEBUG_PROPERTIES)) {
    printMessage("Sizing changed");
  }

  if (m_DimensionsCache != get_Dimensions()) {
    setDimensions(get_Dimensions());
  }

  if (m_ChunkSizeCache != get_ChunkSize()) {
    setChunkSize(get_ChunkSize());
  }

  set_ChunkCount(m_ChunkCountCache);
}

void CctwChunkedData::setDataSource(QString desc)
{
  QUrl url(desc);

  if (qcepDebug(DEBUG_APP)) {
    printMessage(tr("%1.setDataSource(\"%2\")").arg(get_Name()).arg(CctwApplication::addSlashes(desc)));

    printMessage(tr("scheme:   %1").arg(url.scheme()));
    printMessage(tr("host:     %1").arg(url.host()));
    printMessage(tr("path:     %1").arg(url.path()));
    printMessage(tr("filename: %1").arg(url.fileName()));
#if QT_VERSION >= 0x050000
    printMessage(tr("query:    %1").arg(url.query()));
#endif
    printMessage(tr("fragment: %1").arg(url.fragment()));
  }

  set_DataFileName(url.path());
  set_DataSetName(url.fragment());

#if QT_VERSION >= 0x050000
  if (url.hasQuery()) {
    QUrlQuery qry(url);

    QList <QPair <QString, QString> > l = qry.queryItems();

    QPair<QString, QString> v;
    foreach (v, l) {
      if (qcepDebug(DEBUG_APP)) {
        printMessage(tr(" key:     %1").arg(v.first));
        printMessage(tr(" val:     %1").arg(v.second));
      }
    }

    if (qry.hasQueryItem("size")) {
      QString chunkSize = qry.queryItemValue("size");
      setChunks(chunkSize);
    }

    if (qry.hasQueryItem("dim")) {
      QString dims      = qry.queryItemValue("dim");
      setDims(dims);
    }

    if (qry.hasQueryItem("dataset")) {
      QString dset      = qry.queryItemValue("dataset");
      setDataset(dset);
    }
  }
#endif
}

void CctwChunkedData::onDataFileNameChanged()
{
  if (m_IsInput) {
    checkInputFile();
  }
}

void CctwChunkedData::onMaskFileNameChanged()
{
  if (m_IsInput) {
    checkMaskFile();
  }
}

void CctwChunkedData::onAnglesChanged()
{
  if (m_IsInput) {
    checkAnglesFile();
  }
}

void CctwChunkedData::setDims(QString desc)
{
  QRegExp r("(\\d+)[Xx,](\\d+)[Xx,](\\d+)", Qt::CaseInsensitive);

  if (r.exactMatch(desc)) {
    CctwIntVector3D d(r.cap(1).toInt(), r.cap(2).toInt(), r.cap(3).toInt());

    setDimensions(d);

    printMessage(tr("Dataset dimensions set to [%1,%2,%3]").arg(d.x()).arg(d.y()).arg(d.z()));
  } else {
    bool ok;
    int n = desc.toInt(&ok);

    if (ok) {
      CctwIntVector3D d(n,n,n);

      setDimensions(d);

      printMessage(tr("Dataset dimensions set to [%1,%2,%3]").arg(d.x()).arg(d.y()).arg(d.z()));
    }
  }
}

void CctwChunkedData::setChunks(QString desc)
{
  QRegExp r("(\\d+)[Xx,](\\d+)[Xx,](\\d+)", Qt::CaseInsensitive);

  if (r.exactMatch(desc)) {
    CctwIntVector3D cksz(r.cap(1).toInt(), r.cap(2).toInt(), r.cap(3).toInt());

    setChunkSize(cksz);

    printMessage(tr("Chunk size set to [%1,%2,%3]").arg(cksz.x()).arg(cksz.y()).arg(cksz.z()));
  } else {
    bool ok;
    int n = desc.toInt(&ok);

    if (ok) {
      CctwIntVector3D cksz(n,n,n);

      setChunkSize(cksz);

      printMessage(tr("Chunk size set to [%1,%2,%3]").arg(cksz.x()).arg(cksz.y()).arg(cksz.z()));
    }
  }
}

void CctwChunkedData::setDataset(QString desc)
{
  set_DataSetName(desc);
}

void CctwChunkedData::setMaskSource(QString desc)
{
  QUrl url(desc);

  if (qcepDebug(DEBUG_APP)) {
    printMessage(tr("%1.setMaskSource(\"%2\")").arg(get_Name()).arg(CctwApplication::addSlashes(desc)));

    printMessage(tr("scheme:   %1").arg(url.scheme()));
    printMessage(tr("host:     %1").arg(url.host()));
    printMessage(tr("path:     %1").arg(url.path()));
    printMessage(tr("filename: %1").arg(url.fileName()));
#if QT_VERSION >= 0x050000
    printMessage(tr("query:    %1").arg(url.query()));
#endif
    printMessage(tr("fragment: %1").arg(url.fragment()));
  }

  set_MaskDataFileName(url.path());
  set_MaskDataSetName(url.fragment());

#if QT_VERSION >= 0x050000
  if (url.hasQuery()) {
    QUrlQuery qry(url);

    QList <QPair <QString, QString> > l = qry.queryItems();

    QPair<QString, QString> v;
    foreach (v, l) {
      if (qcepDebug(DEBUG_APP)) {
        printMessage(tr(" key:     %1").arg(v.first));
        printMessage(tr(" val:     %1").arg(v.second));
      }
    }

//    if (qry.hasQueryItem("size")) {
//      QString chunkSize = qry.queryItemValue("size");
//      setChunks(chunkSize);
//    }

//    if (qry.hasQueryItem("dim")) {
//      QString dims      = qry.queryItemValue("dim");
//      setDims(dims);
//    }

    if (qry.hasQueryItem("dataset")) {
      QString dset      = qry.queryItemValue("dataset");
      setMaskDataset(dset);
    }
  }
#endif
}

void CctwChunkedData::setMask3DSource(QString desc)
{
  QUrl url(desc);

  if (qcepDebug(DEBUG_APP)) {
    printMessage(tr("%1.setMask3DSource(\"%2\")").arg(get_Name()).arg(CctwApplication::addSlashes(desc)));

    printMessage(tr("scheme:   %1").arg(url.scheme()));
    printMessage(tr("host:     %1").arg(url.host()));
    printMessage(tr("path:     %1").arg(url.path()));
    printMessage(tr("filename: %1").arg(url.fileName()));
#if QT_VERSION >= 0x050000
    printMessage(tr("query:    %1").arg(url.query()));
#endif
    printMessage(tr("fragment: %1").arg(url.fragment()));
  }

  set_Mask3DDataFileName(url.path());
  set_Mask3DDataSetName(url.fragment());

#if QT_VERSION >= 0x050000
  if (url.hasQuery()) {
    QUrlQuery qry(url);

    QList <QPair <QString, QString> > l = qry.queryItems();

    QPair<QString, QString> v;
    foreach (v, l) {
      if (qcepDebug(DEBUG_APP)) {
        printMessage(tr(" key:     %1").arg(v.first));
        printMessage(tr(" val:     %1").arg(v.second));
      }
    }

//    if (qry.hasQueryItem("size")) {
//      QString chunkSize = qry.queryItemValue("size");
//      setChunks(chunkSize);
//    }

//    if (qry.hasQueryItem("dim")) {
//      QString dims      = qry.queryItemValue("dim");
//      setDims(dims);
//    }

    if (qry.hasQueryItem("dataset")) {
      QString dset      = qry.queryItemValue("dataset");
      set_Mask3DDataSetName(dset);
    }
  }
#endif
}

void CctwChunkedData::setMaskDataset(QString desc)
{
  set_MaskDataSetName(desc);
}

void CctwChunkedData::setAnglesSource(QString desc)
{
  QUrl url(desc);

  if (qcepDebug(DEBUG_APP)) {
    printMessage(tr("%1.setAnglesSource(\"%2\")").arg(get_Name()).arg(CctwApplication::addSlashes(desc)));

    printMessage(tr("scheme:   %1").arg(url.scheme()));
    printMessage(tr("host:     %1").arg(url.host()));
    printMessage(tr("path:     %1").arg(url.path()));
    printMessage(tr("filename: %1").arg(url.fileName()));
#if QT_VERSION >= 0x050000
    printMessage(tr("query:    %1").arg(url.query()));
#endif
    printMessage(tr("fragment: %1").arg(url.fragment()));
  }

  set_AnglesDataFileName(url.path());
  set_AnglesDataSetName(url.fragment());

#if QT_VERSION >= 0x050000
  if (url.hasQuery()) {
    QUrlQuery qry(url);

    QList <QPair <QString, QString> > l = qry.queryItems();

    QPair<QString, QString> v;
    foreach (v, l) {
      if (qcepDebug(DEBUG_APP)) {
        printMessage(tr(" key:     %1").arg(v.first));
        printMessage(tr(" val:     %1").arg(v.second));
      }
    }

//    if (qry.hasQueryItem("size")) {
//      QString chunkSize = qry.queryItemValue("size");
//      setChunks(chunkSize);
//    }

//    if (qry.hasQueryItem("dim")) {
//      QString dims      = qry.queryItemValue("dim");
//      setDims(dims);
//    }

    if (qry.hasQueryItem("dataset")) {
      QString dset      = qry.queryItemValue("dataset");
      setAnglesDataset(dset);
    }
  }
#endif
}

void CctwChunkedData::setAnglesDataset(QString desc)
{
  set_AnglesDataSetName(desc);
}

void CctwChunkedData::setWeightsSource(QString desc)
{
  QUrl url(desc);

  if (qcepDebug(DEBUG_APP)) {
    printMessage(tr("%1.setWeightsSource(\"%2\")").arg(get_Name()).arg(CctwApplication::addSlashes(desc)));

    printMessage(tr("scheme:   %1").arg(url.scheme()));
    printMessage(tr("host:     %1").arg(url.host()));
    printMessage(tr("path:     %1").arg(url.path()));
    printMessage(tr("filename: %1").arg(url.fileName()));
#if QT_VERSION >= 0x050000
    printMessage(tr("query:    %1").arg(url.query()));
#endif
    printMessage(tr("fragment: %1").arg(url.fragment()));
  }

  set_WeightsDataFileName(url.path());
  set_WeightsDataSetName(url.fragment());

#if QT_VERSION >= 0x050000
  if (url.hasQuery()) {
    QUrlQuery qry(url);

    QList <QPair <QString, QString> > l = qry.queryItems();

    QPair<QString, QString> v;
    foreach (v, l) {
      if (qcepDebug(DEBUG_APP)) {
        printMessage(tr(" key:     %1").arg(v.first));
        printMessage(tr(" val:     %1").arg(v.second));
      }
    }

//    if (qry.hasQueryItem("size")) {
//      QString chunkSize = qry.queryItemValue("size");
//      setChunks(chunkSize);
//    }

//    if (qry.hasQueryItem("dim")) {
//      QString dims      = qry.queryItemValue("dim");
//      setDims(dims);
//    }

    if (qry.hasQueryItem("dataset")) {
      QString dset      = qry.queryItemValue("dataset");
      setWeightsDataset(dset);
    }
  }
#endif
}

void CctwChunkedData::setWeightsDataset(QString desc)
{
  set_WeightsDataSetName(desc);
}

bool                CctwChunkedData::containsPixel(CctwIntVector3D pixelCoord)
{
  return pixelCoord.x() >= 0 &&
      pixelCoord.y() >= 0 &&
      pixelCoord.z() >= 0 &&
      pixelCoord.x() < m_DimensionsCache.x() &&
      pixelCoord.y() < m_DimensionsCache.y() &&
      pixelCoord.z() < m_DimensionsCache.z();
}

CctwIntVector3D     CctwChunkedData::chunkStart(int n)
{
  // Return pixel coords of start of chunk chunkIdx

  CctwIntVector3D chunkIdx = chunkIndexFromNumber(n);

  return m_ChunkSizeCache*chunkIdx;
}

int     CctwChunkedData::chunkContaining(CctwIntVector3D pixelCoord)
{
  // Return index of chunk containing given pixel

  CctwIntVector3D res = pixelCoord / m_ChunkSizeCache;

  return chunkNumberFromIndex(res);
}

int     CctwChunkedData::chunkContaining(CctwDoubleVector3D fracPixelCoord)
{
  CctwDoubleVector3D fchk = fracPixelCoord / CctwDoubleVector3D(m_ChunkSizeCache.x(), m_ChunkSizeCache.y(), m_ChunkSizeCache.z());

  return chunkNumberFromIndex(CctwIntVector3D(fchk.x(), fchk.y(), fchk.z()));
}

CctwIntVector3D     CctwChunkedData::chunkCount() const
{
  return m_ChunkCountCache;
}

bool CctwChunkedData::containsChunk(int ix, int iy, int iz)
{
  return ix>=0 && ix<m_ChunkCountCache.x() &&
         iy>=0 && iy<m_ChunkCountCache.y() &&
         iz>=0 && iz<m_ChunkCountCache.z();
}

int CctwChunkedData::chunkNumberFromIndex(CctwIntVector3D chunkIdx)
{
  if (containsChunk(chunkIdx.x(), chunkIdx.y(), chunkIdx.z())) {
    int xstride = 1;
    int ystride = m_ChunkCountCache.x();
    int zstride = m_ChunkCountCache.x()*m_ChunkCountCache.y();

    return chunkIdx.x()*xstride + chunkIdx.y()*ystride + chunkIdx.z()*zstride;
  } else {
    return -1;
  }
}

CctwIntVector3D CctwChunkedData::chunkIndexFromNumber(int n)
{
  if (n>=0 && n<m_ChunkCountCache.volume()) {
    int xstride = 1;
    int ystride = m_ChunkCountCache.x();
    int zstride = m_ChunkCountCache.x()*m_ChunkCountCache.y();

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

CctwDataChunk *CctwChunkedData::chunk(int n)
{
  if (n >= 0 && n < m_DataChunks.count()) {
    QcepMutexLocker lock(__FILE__, __LINE__, &m_ChunkLock);

    CctwDataChunk *chunk = m_DataChunks[n];

    if (chunk == NULL) {
      CctwChunkedDataWPtr myself = qSharedPointerDynamicCast<CctwChunkedData>(sharedFromThis());

      chunk = new CctwDataChunk(myself, n, tr("Chunk-%1").arg(n), sharedFromThis());

//      if (parent()) {
//        chunk->moveToThread(parent()->thread());
//        chunk->setParent(parent());
//      }

      m_DataChunks[n] = chunk;
    }

    if (chunk && chunk->index() != n) {
      printMessage(tr("Chunk anomaly"));
    }
    return chunk;
  } else {
    return NULL;
  }
}

void CctwChunkedData::releaseChunk(int n)
{
  if (n >= 0 && n < m_DataChunks.count()) {
    QcepMutexLocker lock(__FILE__, __LINE__, &m_ChunkLock);

    CctwDataChunk *chunk = m_DataChunks[n];

    if (chunk) {
      delete chunk;
      m_DataChunks[n] = NULL;
    }
  }
}

void CctwChunkedData::releaseChunkData(int n)
{
  CctwDataChunk *chk = chunk(n);

  if (chk) {
    chk->deallocateData();
    chk->deallocateWeights();
  }
}

CctwDataChunk *CctwChunkedData::chunk(CctwIntVector3D idx)
{
  return chunk(chunkNumberFromIndex(idx));
}

void CctwChunkedData::mergeChunk(CctwDataChunk *chunk)
{
  if (chunk) {
    int idx = chunk->index();

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

  set_ChunksRead(0);
  set_ChunksWritten(0);
  set_ChunksHeld(0);
  set_ChunksHeldMax(0);
}

void CctwChunkedData::normalizeChunk(int n)
{
  CctwDataChunk *chunk = this->chunk(n);

  if (chunk) {
    chunk->normalizeChunk();
  }
}

bool CctwChunkedData::checkInputFile()
{
  /* Save old error handler */
  herr_t (*old_func)(hid_t, void*);
  void *old_client_data;
  H5Eget_auto(H5E_DEFAULT, &old_func, &old_client_data);

  /* Turn off error handling */
  H5Eset_auto(H5E_DEFAULT, NULL, NULL);

  bool res = openInputFile(true);
  closeInputFile();

  /* Restore previous error handler */
  H5Eset_auto(H5E_DEFAULT, old_func, old_client_data);

  return res;
}

bool CctwChunkedData::openInputFile(bool quietly)
{
  set_Normalization(1);

  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  if (m_FileId >= 0) {
    return true;
  }

  QString fileName    = get_DataFileName();
  QString dataSetName = get_DataSetName();

  if (fileName.length() == 0) {
    printMessage("Input file name is empty");
    return false;
  }

  if (dataSetName.length() == 0) {
    printMessage("Input dataset name is empty");
    return false;
  }

  if (!quietly) {
    printMessage(tr("About to open input file: %1 dataset: %2")
                  .arg(fileName).arg(dataSetName));
  }

  QFileInfo f(fileName);

  hid_t fileId = -1;
  hid_t objId  = -1;
  hid_t dsetId = -1;
  hid_t dspcId = -1;
  hid_t plist  = -1;
  hid_t dset2Id = -1;
  hid_t dspc2Id = -1;

  try
  {
    if (!f.exists()) {
      throw tr("File %1 does not exist").arg(fileName);
    }

    if (H5Fis_hdf5(qPrintable(fileName)) <= 0) {
      throw tr("File %1 exists but is not an hdf file").arg(fileName);
    }

    fileId = H5Fopen(qPrintable(fileName), H5F_ACC_RDONLY, H5P_DEFAULT);
    if (fileId < 0) {
      throw tr("File %1 could not be opened").arg(fileName);
    }

    objId = H5Oopen(fileId, qPrintable(dataSetName), H5P_DEFAULT);
    if (objId <= 0) {
      throw tr("Could not open data object!");
    }

    H5O_info_t info;
    if (H5Oget_info(objId, &info) < 0) {
      throw tr("Could not get object info");
    }

    H5Oclose(objId);
    objId = -1;

    if (info.type == H5O_TYPE_DATASET) {
      // Input descriptor names a dataset - normalized data...

      dsetId = H5Dopen(fileId, qPrintable(dataSetName), H5P_DEFAULT);
      if (dsetId <= 0) {
        throw tr("Could not open dataset!");
      }

      dspcId = H5Dget_space(dsetId);
      if (dspcId < 0) {
        throw tr("Could not get dataspace of existing dataset");
      }

      hsize_t dims[3];
      int ndims = H5Sget_simple_extent_ndims(dspcId);
      if (ndims != 3) {
        throw tr("Dataspace is not 3-dimensional");
      }

      int ndims2 = H5Sget_simple_extent_dims(dspcId, dims, NULL);
      if (ndims2 != 3) {
        throw tr("Could not get dataspace dimensions");
      }

      setDimensions(CctwIntVector3D(dims[2], dims[1], dims[0]));
      plist = H5Dget_create_plist(dsetId);

      if (plist < 0) {
        throw tr("Could not get dataset create plist");
      }

      hsize_t cksz[3];
      if (H5Pget_chunk(plist, 3, cksz) < 0) {
        printMessage(("Could not get dataset chunk size"));
        set_HDFChunkSize(CctwIntVector3D(0,0,0));
        setChunkSize(CctwIntVector3D(32,32,32));
      } else {
        set_HDFChunkSize(CctwIntVector3D(cksz[2], cksz[1], cksz[0]));
        setChunkSize(get_HDFChunkSize());
      }

      H5Pclose(plist); plist = -1;
    } else if (info.type == H5O_TYPE_GROUP) {
      // Input descriptor names a group - intermediate data...

      dsetId = H5Dopen(fileId, qPrintable(dataSetName+"/v"), H5P_DEFAULT);
      if (dsetId <= 0)
        throw tr("Could not open 'V' dataset!");

      dspcId = H5Dget_space(dsetId);
      if (dspcId < 0)
        throw tr("Could not get dataspace of existing dataset");

      hsize_t dims[3];
      int ndims = H5Sget_simple_extent_ndims(dspcId);
      if (ndims != 3)
        throw tr("Dataspace is not 3-dimensional");

      int ndims2 = H5Sget_simple_extent_dims(dspcId, dims, NULL);
      if (ndims2 != 3)
        throw tr("Could not get dataspace dimensions");

      setDimensions(CctwIntVector3D(dims[2], dims[1], dims[0]));
      plist = H5Dget_create_plist(dsetId);

      if (plist < 0)
        throw tr("Could not get dataset create plist");

      hsize_t cksz[3];
      if (H5Pget_chunk(plist, 3, cksz) < 0) {
        printMessage(("Could not get dataset chunk size"));
        set_HDFChunkSize(CctwIntVector3D(0,0,0));
        setChunkSize(CctwIntVector3D(32,32,32));
      } else {
        set_HDFChunkSize(CctwIntVector3D(cksz[2], cksz[1], cksz[0]));
        setChunkSize(get_HDFChunkSize());
      }

      H5Pclose(plist); plist = -1;

      dset2Id = H5Dopen(fileId, qPrintable(dataSetName+"/n"), H5P_DEFAULT);
      if (dset2Id < 0) {
        printMessage(tr("Could not open 'N' dataset, assuming normalized"));
      } else {
        dspc2Id = H5Dget_space(dsetId);
        if (dspc2Id < 0)
          throw tr("Could not get dataspace of existing 'N' dataset");

        hsize_t dims[3];
        int ndims = H5Sget_simple_extent_ndims(dspc2Id);
        if (ndims != 3)
          throw tr("Dataspace is not 3-dimensional");

        int ndims2 = H5Sget_simple_extent_dims(dspc2Id, dims, NULL);
        if (ndims2 != 3)
          throw tr("Could not get dataspace dimensions");

        if (dimensions() != CctwIntVector3D(dims[2],dims[1],dims[0])) {
          throw tr("Dimension mismatch between 'V' and 'N' datasets");
        }

        set_Normalization(0);
      }
    } else {
      throw tr("Input dataset is wrong type");
    }

    m_FileId      = fileId;
    m_DatasetId   = dsetId;
    m_DataspaceId = dspcId;
    m_Dataset2Id  = dset2Id;
    m_Dataspace2Id= dspc2Id;

    if (!quietly) {
      printMessage(tr("Opened input file \"%1\" OK, DS \"%2\", Norm %3, DSID %4, DS2ID %5, CMPRS %6")
                   .arg(get_DataFileName()).arg(get_DataSetName())
                   .arg(get_Normalization()).arg(m_DatasetId).arg(m_Dataset2Id)
                   .arg(get_Compression()));

      CctwIntVector3D hdfChunkSize = get_HDFChunkSize();

      printMessage(tr("Input dimensions %1, HDF Chunk size %2, Input chunk size %3, Chunk counts %4")
                   .arg(get_Dimensions().toString())
                   .arg(hdfChunkSize.toString())
                   .arg(get_ChunkSize().toString())
                   .arg(get_ChunkCount().toString()));
    }
  }
  catch (QString &msg )
  {
    if (!quietly) {
      printMessage(tr("Anomaly in CctwChunkedData::openInputFile fileId=%1, dsetId=%2, dspcId=%3")
                   .arg(fileId).arg(dsetId).arg(dspcId));
      printMessage(msg);
    }

    if (dspcId >= 0) H5Sclose(dspcId);
    if (dspc2Id >= 0) H5Sclose(dspc2Id);
    if (dsetId >= 0) H5Dclose(dsetId);
    if (dset2Id >= 0) H5Dclose(dset2Id);
    if (objId >= 0) H5Oclose(objId);
    if (fileId >= 0) H5Fclose(fileId);
    if (plist >= 0) H5Pclose(plist);

    return false;
  }
  return true;
}

void CctwChunkedData::closeInputFile()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  if (m_IsNeXus)
    closeInputNeXusFile();

  if (m_DataspaceId >= 0) {
    H5Sclose(m_DataspaceId);
    m_DataspaceId = -1;
  }

  if (m_Dataspace2Id >= 0) {
    H5Sclose(m_Dataspace2Id);
    m_Dataspace2Id = -1;
  }

  if (m_DatasetId >= 0) {
    H5Dclose(m_DatasetId);
    m_DatasetId = -1;
  }

  if (m_Dataset2Id >= 0) {
    H5Dclose(m_Dataset2Id);
    m_Dataset2Id = -1;
  }

  if (m_FileId >= 0) {
    H5Fclose(m_FileId);
    m_FileId = -1;
  }

  printMessage("Closed input file");
}

#if NEXUS_ENABLED == 1

bool CctwChunkedData::openInputNeXusFile()
{
  bool res = true;

  m_IsNeXus = true;

  QString fileName = get_DataFileName();
  QFileInfo f(fileName);

  if (!f.exists()) {
    printMessage(tr("File %1 does not exist").arg(fileName));
    res = false;
  } else if (H5Fis_hdf5(qPrintable(fileName)) <= 0) {
    printMessage(tr("File %1 exists but is not an hdf file").arg(fileName));
    res = false;
  }

  m_NeXusFile = new NeXus::File(qPrintable(fileName));
  printMessage(tr("NeXus file opened successfully: %1").arg(fileName));

  return res;
}

void CctwChunkedData::closeInputNeXusFile()
{
  m_NeXusFile->close();
}

#else
bool CctwChunkedData::openInputNeXusFile()
{
  printMessage(tr("NeXus was not enabled at build time!"));
  return false;
}
void CctwChunkedData::closeInputNeXusFile()
{
  printMessage(tr("NeXus was not enabled at build time!"));
}
#endif

bool CctwChunkedData::openOutputFile()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  if (m_FileId >= 0) {
    return true;
  }

  QString fileName = get_DataFileName();
  QString dataSetName = get_DataSetName();

  if (fileName.length() == 0) {
    printMessage("Output file name is empty");
    return false;
  }

  if (dataSetName.length() == 0) {
    printMessage("Output dataset name is empty");
    return false;
  }

  printMessage(tr("About to open output file: %1 dataset: %2")
                .arg(fileName).arg(dataSetName));

  int res = true;

  QFileInfo f(fileName);

  hid_t fileId = -1;
  hid_t gplist2 = -1;
  hid_t gplist = -1;
  hid_t dsetId = -1;
  hid_t dspcId = -1;
  hid_t dset2Id = -1;
  hid_t dspc2Id = -1;
  hid_t plist  = -1;
  hid_t plist2  = -1;

  if (!f.exists()) {
    fileId = H5Fcreate(qPrintable(fileName), H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);

    if (fileId < 0) {
      printMessage(tr("File %1 could not be created").arg(fileName));
      res = false;
    }
  } else if (H5Fis_hdf5(qPrintable(fileName)) <= 0) {
    printMessage(tr("File %1 exists but is not an hdf file").arg(fileName));
    res = false;
  } else {
    fileId = H5Fopen(qPrintable(fileName), H5F_ACC_RDWR, H5P_DEFAULT);

    if (fileId < 0) {
      printMessage(tr("File %1 could not be opened").arg(fileName));
      res = false;
    }
  }

  if (fileId >= 0) {
    herr_t (*old_func)(hid_t, void*);
    void *old_client_data;
    H5Eget_auto(H5E_DEFAULT, &old_func, &old_client_data);

    /* Turn off error handling */
    H5Eset_auto(H5E_DEFAULT, NULL, NULL);

    if (get_Normalization()) {
      dsetId = H5Dopen(fileId, qPrintable(get_DataSetName()), H5P_DEFAULT);
    } else {
      dsetId = H5Dopen(fileId, qPrintable(get_DataSetName()+"/v"), H5P_DEFAULT);
      dset2Id = H5Dopen(fileId, qPrintable(get_DataSetName()+"/n"), H5P_DEFAULT);
    }

    /* Restore previous error handler */
    H5Eset_auto(H5E_DEFAULT, old_func, old_client_data);

    if (dsetId >= 0) {
      dspcId = H5Dget_space(dsetId);

      if (dspcId < 0) {
        printMessage("Couldn't get dataspace of existing dataset");
        res = false;
      } else {
        hsize_t dims[3];
        int ndims = H5Sget_simple_extent_ndims(dspcId);

        if (ndims != 3) {
          printMessage("Dataspace is not 3-dimensional");
          res = false;
        } else {
          int ndims2 = H5Sget_simple_extent_dims(dspcId, dims, NULL);

          if (ndims2 != 3) {
            printMessage("Couldnt get dataspace dimensions");
            res = false;
          } else {
            if (dims[0] != (hsize_t) dimensions().z() ||
                dims[1] != (hsize_t) dimensions().y() ||
                dims[2] != (hsize_t) dimensions().x()) {
              printMessage("Dataspace dimensions do not match output data");
              res = false;
            }
          }
        }
      }
    } else {
      hsize_t dims[3];
      dims[0] = dimensions().z();
      dims[1] = dimensions().y();
      dims[2] = dimensions().x();

      dspcId = H5Screate_simple(3, dims, NULL);

      CctwIntVector3D cksz = get_HDFChunkSize();
      int cmprs = get_Compression();

      plist = H5Pcreate(H5P_DATASET_CREATE);

      if (cksz.volume() > 0) {
        hsize_t c[3];

        c[0] = cksz.z();
        c[1] = cksz.y();
        c[2] = cksz.x();

        float zero = 0.0;

        if (H5Pset_chunk(plist, 3, c) < 0) {
          printMessage("Failed to set chunk size");
          res = false;
        } else if (H5Pset_fill_value(plist, H5T_NATIVE_FLOAT, &zero)) {
          printMessage("Failed to set fill value");
          res = false;
        } else if (cmprs > 0) {
          if (H5Pset_deflate(plist, cmprs) < 0) {
            printMessage("Set deflate failed");
            res = false;
          }
        } else if (cmprs) {
          if (H5Pset_filter(plist, H5PY_FILTER_LZF, H5Z_FLAG_OPTIONAL, 0, NULL) < 0) {
            printMessage("Set LZF Compress failed");
            res = false;
          }
        }
      }

      gplist = H5Pcreate(H5P_LINK_CREATE);

      if (H5Pset_create_intermediate_group(gplist, 1) < 0) {
        printMessage(tr("Failed to set create intermediate groups"));
        res = false;
      } else {
        if (get_Normalization()) {
          dsetId = H5Dcreate(fileId,
                             qPrintable(get_DataSetName()),
                             H5T_NATIVE_FLOAT,
                             dspcId,
                             gplist,
                             plist,
                             H5P_DEFAULT);
        } else {
          dsetId = H5Dcreate(fileId,
                             qPrintable(get_DataSetName()+"/v"),
                             H5T_NATIVE_FLOAT,
                             dspcId,
                             gplist,
                             plist,
                             H5P_DEFAULT);
        }
      }
    }

    if (dsetId < 0) {
      printMessage(tr("Could not create or find dataset %1").arg(get_DataSetName()));
      res = false;
    } else if (!get_Normalization()) {
      if (dset2Id >= 0) {
        dspc2Id = H5Dget_space(dset2Id);

        if (dspc2Id < 0) {
          printMessage("Couldn't get dataspace of existing 'N' dataset");
          res = false;
        } else {
          hsize_t dims[3];
          int ndims = H5Sget_simple_extent_ndims(dspc2Id);

          if (ndims != 3) {
            printMessage("'N' Dataspace is not 3-dimensional");
            res = false;
          } else {
            int ndims2 = H5Sget_simple_extent_dims(dspc2Id, dims, NULL);

            if (ndims2 != 3) {
              printMessage("Couldnt get 'N' dataspace dimensions");
              res = false;
            } else {
              if (dims[0] != (hsize_t) dimensions().z() ||
                  dims[1] != (hsize_t) dimensions().y() ||
                  dims[2] != (hsize_t) dimensions().x()) {
                printMessage("'N' Dataspace dimensions do not match output data");
                res = false;
              }
            }
          }
        }
      } else {
        hsize_t dims[3];
        dims[0] = dimensions().z();
        dims[1] = dimensions().y();
        dims[2] = dimensions().x();

        dspc2Id = H5Screate_simple(3, dims, NULL);

        CctwIntVector3D cksz = get_HDFChunkSize();
        int cmprs = get_Compression();

        plist2 = H5Pcreate(H5P_DATASET_CREATE);

        if (cksz.volume() > 0) {
          hsize_t c[3];

          c[0] = cksz.z();
          c[1] = cksz.y();
          c[2] = cksz.x();

          float zero = 0.0;

          if (H5Pset_chunk(plist2, 3, c) < 0) {
            printMessage("Failed to set chunk size");
            res = false;
          } else if (H5Pset_fill_value(plist2, H5T_NATIVE_FLOAT, &zero)) {
            printMessage("Failed to set fill value");
            res = false;
          } else if (cmprs > 0) {
            if (H5Pset_deflate(plist2, cmprs) < 0) {
              printMessage("Set deflate failed");
              res = false;
            }
          } else if (cmprs) {
            if (H5Pset_filter(plist2, H5PY_FILTER_LZF, H5Z_FLAG_OPTIONAL, 0, NULL) < 0) {
              printMessage("Set LZF Compress failed");
              res = false;
            }
          }
        }

        gplist2 = H5Pcreate(H5P_LINK_CREATE);

        if (H5Pset_create_intermediate_group(gplist2, 1) < 0) {
          printMessage(tr("Failed to set create intermediate groups"));
          res = false;
        } else {
          dset2Id = H5Dcreate(fileId,
                             qPrintable(get_DataSetName()+"/n"),
                             H5T_NATIVE_FLOAT,
                             dspc2Id,
                             gplist2,
                             plist2,
                             H5P_DEFAULT);
        }
      }
    }
  }

  if (plist  >= 0) H5Pclose(plist);
  if (gplist >= 0) H5Pclose(gplist);
  if (plist2  >= 0) H5Pclose(plist2);
  if (gplist2 >= 0) H5Pclose(gplist2);

  if (res == false) {
    if (dspcId >= 0) H5Sclose(dspcId);
    if (dsetId >= 0) H5Dclose(dsetId);
    if (dspc2Id >= 0) H5Sclose(dspc2Id);
    if (dset2Id >= 0) H5Dclose(dset2Id);
    if (fileId >= 0) H5Fclose(fileId);
  } else {
    if (fileId < 0 || dsetId < 0 || dspcId < 0) {
      printMessage(tr("Error in CctwChunkedData::openOutputFile fileId=%1, dsetId=%2, dspcId=%3")
                   .arg(fileId).arg(dsetId).arg(dspcId));
    }

    if (!get_Normalization() && (dset2Id < 0 || dspc2Id < 0)) {
      printMessage(tr("Error in CctwChunkedData::openOutputFile fileId=%1, dset2Id=%2, dspc2Id=%3")
                   .arg(fileId).arg(dset2Id).arg(dspc2Id));
    }

    m_FileId      = fileId;
    m_DatasetId   = dsetId;
    m_DataspaceId = dspcId;
    m_Dataset2Id  = dset2Id;
    m_Dataspace2Id= dspc2Id;

    printMessage(tr("Opened output file \"%1\" OK, DS \"%2\", Norm %3, DSID %4, DS2ID %5, CMPRS %6")
                 .arg(get_DataFileName()).arg(get_DataSetName())
                 .arg(get_Normalization()).arg(m_DatasetId).arg(m_Dataset2Id)
                 .arg(get_Compression()));

    CctwIntVector3D hdfChunkSize = get_HDFChunkSize();

    printMessage(tr("Output dimensions %1, HDF Chunk size %2, Output chunk size %3, Output chunk count %4")
                 .arg(get_Dimensions().toString())
                 .arg(hdfChunkSize.toString())
                 .arg(get_ChunkSize().toString())
                 .arg(get_ChunkCount().toString()));
  }

  return res;
}

void CctwChunkedData::closeOutputFile()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  if (m_DataspaceId >= 0) {
    H5Sclose(m_DataspaceId);
    m_DataspaceId = -1;
  }

  if (m_DatasetId >= 0) {
    H5Dclose(m_DatasetId);
    m_DatasetId = -1;
  }

  if (m_Dataspace2Id >= 0) {
    H5Sclose(m_Dataspace2Id);
    m_Dataspace2Id = -1;
  }

  if (m_Dataset2Id >= 0) {
    H5Dclose(m_Dataset2Id);
    m_Dataset2Id = -1;
  }

  if (m_FileId >= 0) {
    H5Fclose(m_FileId);
    m_FileId = -1;
  }

  printMessage("Closed output file");
}

void CctwChunkedData::flushOutputFile()
{
  printMessage("\rWriting output file...");

  m_Application->set_Progress(0);
  m_Application->set_ProgressLimit(m_DataChunks.count());

  for (int i=0; i<m_DataChunks.count(); i++) {
    writeChunk(i);
    m_Application->prop_Progress()->incValue(1);
  }
}

bool CctwChunkedData::checkMaskFile()
{
  /* Save old error handler */
  herr_t (*old_func)(hid_t, void*);
  void *old_client_data;
  H5Eget_auto(H5E_DEFAULT, &old_func, &old_client_data);

  /* Turn off error handling */
  H5Eset_auto(H5E_DEFAULT, NULL, NULL);

  bool res = openMaskFile(true);
  closeMaskFile();

  /* Restore previous error handler */
  H5Eset_auto(H5E_DEFAULT, old_func, old_client_data);

  return res;
}

bool CctwChunkedData::openMaskFile(bool quietly)
{
  bool sameFile = false;

  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  if (m_MaskFileId >= 0) {
    return true;
  }

  QString fileName    = get_MaskDataFileName();
  QString dataSetName = get_MaskDataSetName();
  QString inputFile   = get_DataFileName();

  if (dataSetName.length()==0) {
    printMessage(tr("No dataset name given for mask file, skipping"));
    return true;
  }

  if (!quietly) {
    printMessage(tr("About to open mask input file: %1 dataset: %2")
                  .arg(fileName).arg(dataSetName));
  }

  QFileInfo f(fileName);

  hid_t fileId = -1;
  hid_t dsetId = -1;
  hid_t dspcId = -1;

  try
  {
    if (fileName.length() > 0) {
      if (!f.exists())
        throw tr("Mask file %1 does not exist").arg(fileName);
      if (H5Fis_hdf5(qPrintable(fileName)) <= 0)
        throw tr("Mask file %1 exists but is not an hdf file").arg(fileName);
    }

    if (fileName.length() == 0) { // No file name given?
      printMessage("No mask file name");
      fileId = m_FileId;
      sameFile = true;
    } else if (fileName == inputFile) { // In same file as input data?
      printMessage("Mask file same as input file");
      fileId = m_FileId;
      sameFile = true;
    } else {
      fileId = H5Fopen(qPrintable(fileName), H5F_ACC_RDWR, H5P_DEFAULT);
    }

    if (fileId < 0)
      throw tr("Mask file %1 could not be opened").arg(fileName);

    dsetId = H5Dopen(fileId, qPrintable(dataSetName), H5P_DEFAULT);
    if (dsetId <= 0)
      throw tr("Could not open mask dataset!");

    dspcId = H5Dget_space(dsetId);
    if (dspcId < 0)
      throw tr("Could not get dataspace of existing mask dataset");

    hsize_t dims[3];
    int ndims = H5Sget_simple_extent_ndims(dspcId);
    if (ndims != 2)
      throw tr("Mask dataspace is not 2-dimensional");

    int ndims2 = H5Sget_simple_extent_dims(dspcId, dims, NULL);
    if (ndims2 != 2)
      throw tr("Could not get mask dataspace dimensions");

    setMaskDimensions(dims[1], dims[0]);

    m_MaskSameFile    = sameFile;
    m_MaskFileId      = fileId;
    m_MaskDatasetId   = dsetId;
    m_MaskDataspaceId = dspcId;

    if (!quietly) {
      printMessage(tr("Opened mask input file OK"));
    }
  }
  catch (QString &msg )
  {
    if (!quietly) {
      printMessage(tr("Error in CctwChunkedData::openMaskFile fileId=%1, dsetId=%2, dspcId=%3")
                   .arg(fileId).arg(dsetId).arg(dspcId));
      printMessage(msg);
    }

    if (dspcId >= 0) H5Sclose(dspcId);
    if (dsetId >= 0) H5Dclose(dsetId);
    if ((fileId >= 0) && (sameFile == false)) H5Fclose(fileId);
    return false;
  }
  return true;
}

void CctwChunkedData::closeMaskFile()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  if (m_MaskDataspaceId >= 0) {
    H5Sclose(m_MaskDataspaceId);
    m_MaskDataspaceId = -1;
  }

  if (m_MaskDatasetId >= 0) {
    H5Dclose(m_MaskDatasetId);
    m_MaskDatasetId = -1;
  }

  if ((m_MaskFileId >= 0) && (m_MaskSameFile == false)) {
    H5Fclose(m_MaskFileId);
    m_MaskFileId = -1;
    m_MaskSameFile = false;
  }

  if (qcepDebug(DEBUG_APP)) {
    printMessage("Closed mask input file");
  }
}

bool CctwChunkedData::readMaskFile()
{
  bool res = true;

  if (openMaskFile()) {

    if (m_MaskFileId >= 0) {
      res = false;

      QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

      hid_t memspace_id = -1;
      hsize_t offset[2], count[2], stride[2], block[2];

      QcepIntVector a= get_Mask();

      offset[0] = 0;
      offset[1] = 0;

      count[0]  = get_Dimensions().y();
      count[1]  = get_Dimensions().x();

      stride[0] = 1;
      stride[1] = 1;

      block[0]  = 1;
      block[1]  = 1;

      int *msk = a.data();

      memspace_id = H5Screate_simple(2, count, NULL);
      herr_t selerr = H5Sselect_hyperslab(m_MaskDataspaceId, H5S_SELECT_SET, offset, stride, count, block);

      if (selerr < 0) {
        printMessage(tr("ERROR: select_hyperslab failed!"));
      }

      herr_t rderr = H5Dread(m_MaskDatasetId, H5T_NATIVE_INT, memspace_id, m_MaskDataspaceId, H5P_DEFAULT, msk);

      if (selerr || rderr) {
        printMessage(tr("Error reading mask, selerr = %1, rderr = %2").arg(selerr).arg(rderr));
      } else {
        res = true;

        if (msk) {
          int sum=0;
          for (int i=0; i<a.size(); i++) {
            if (msk[i]) {
              sum += 1;
            }
          }

          printMessage(tr("Mask has %1 of %2 bits set").arg(sum).arg(a.size()));
        }

        set_Mask(a);
      }

      H5Sclose(memspace_id);
    }

    closeMaskFile();
  } else {
    res = false;
  }

  if (qcepDebug(DEBUG_APP)) {
    printMessage(tr("CctwChunkedData::readMaskFile returns %1").arg(res));
  }

  return res;
}

bool CctwChunkedData::openMask3DFile(bool quietly)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  if (m_Mask3DFileId >= 0) {
    return true;
  }

  QString fileName = get_Mask3DDataFileName();
  QString dataSetName = get_Mask3DDataSetName();

  if (fileName.length() == 0 && dataSetName.length() == 0) {
    return true;
  }

  if (fileName.length() == 0) {
    fileName = get_DataFileName();
  }

  if (fileName.length() == 0) {
    printMessage("3D Mask File name is empty");
    return false;
  }

  if (dataSetName.length() == 0) {
    printMessage("3D Mask dataset name is empty");
    return false;
  }

  if (!quietly) {
    printMessage(tr("About to open 3D mask file: %1 dataset: %2")
                  .arg(fileName).arg(dataSetName));
  }

  QFileInfo f(fileName);

  hid_t fileId = -1;
  hid_t objId  = -1;
  hid_t dsetId = -1;
  hid_t dspcId = -1;
  hid_t plist  = -1;

  try
  {
    if (!f.exists()) {
      throw tr("3D Mask File %1 does not exist").arg(fileName);
    }

    if (H5Fis_hdf5(qPrintable(fileName)) <= 0) {
      throw tr("3D Mask File %1 exists but is not an hdf file").arg(fileName);
    }

    fileId = H5Fopen(qPrintable(fileName), H5F_ACC_RDONLY, H5P_DEFAULT);
    if (fileId < 0) {
      throw tr("3D Mask File %1 could not be opened").arg(fileName);
    }

    objId = H5Oopen(fileId, qPrintable(dataSetName), H5P_DEFAULT);
    if (objId <= 0) {
      throw tr("Could not open 3D mask data object");
    }

    H5O_info_t info;
    if (H5Oget_info(objId, &info) < 0) {
      throw tr("Could not get 3d mask object info");
    }

    H5Oclose(objId);
    objId = -1;

    if (info.type == H5O_TYPE_DATASET) {
      dsetId = H5Dopen(fileId, qPrintable(dataSetName), H5P_DEFAULT);
      if (dsetId <= 0) {
        throw tr("Could not open 3d mask dataset");
      }

      dspcId = H5Dget_space(dsetId);
      if (dspcId < 0) {
        throw tr("Could not get dataspace of existing 3d mask dataset");
      }

      int ndims = H5Sget_simple_extent_ndims(dspcId);
      if (ndims != 3) {
        throw tr("3D Mask dataspace is not 3-dimensional");
      }

      hsize_t dims[3];
      int ndims2 = H5Sget_simple_extent_dims(dspcId, dims, NULL);
      if (ndims2 != 3) {
        throw tr("Could not get 3d mask dataspace dimensions");
      }

      set_Mask3DDimensions(CctwIntVector3D(dims[2], dims[1], dims[0]));
    } else {
      throw tr("3D mask dataset is wrong type");
    }

    m_Mask3DFileId      = fileId;
    m_Mask3DDatasetId   = dsetId;
    m_Mask3DDataspaceId = dspcId;

    if (!quietly) {
      printMessage(tr("Opened 3D mask file \"%1\" OK, DS \"%2\", DSID %3")
                   .arg(fileName).arg(dataSetName).arg(m_Mask3DDatasetId));
    }
  }

  catch (QString &msg)
  {
    if (!quietly) {
      printMessage(tr("CctwChunkedData::openMask3DFile failed fileId=%1, dsetId=%2, dspcId=%3")
                   .arg(fileId).arg(dsetId).arg(dspcId));
      printMessage(msg);
    }

    if (dspcId >= 0) H5Sclose(dspcId);
    if (dsetId >= 0) H5Dclose(dsetId);
    if (fileId >= 0) H5Fclose(fileId);

    return false;
  }

  return true;
}

void CctwChunkedData::closeMask3DFile()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  if (m_Mask3DDataspaceId >= 0) {
    H5Sclose(m_Mask3DDataspaceId);
    m_Mask3DDataspaceId = -1;
  }

  if (m_Mask3DDatasetId >= 0) {
    H5Dclose(m_Mask3DDatasetId);
    m_Mask3DDatasetId = -1;
  }

  if (m_Mask3DFileId >= 0) {
    H5Fclose(m_Mask3DFileId);
    m_Mask3DFileId = -1;
  }
}

bool CctwChunkedData::checkAnglesFile()
{
  /* Save old error handler */
  herr_t (*old_func)(hid_t, void*);
  void *old_client_data;
  H5Eget_auto(H5E_DEFAULT, &old_func, &old_client_data);

  /* Turn off error handling */
  H5Eset_auto(H5E_DEFAULT, NULL, NULL);

  bool res = openAnglesFile(true);
  closeAnglesFile();

  /* Restore previous error handler */
  H5Eset_auto(H5E_DEFAULT, old_func, old_client_data);

  return res;
}

bool CctwChunkedData::openAnglesFile(bool quietly)
{
  bool sameFile = false;

  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  if (m_AnglesFileId >= 0) {
    return true;
  }

  QString fileName    = get_AnglesDataFileName();
  QString dataSetName = get_AnglesDataSetName();
  QString inputFile   = get_DataFileName();

  if (dataSetName.length()==0) {
    printMessage(tr("No dataset name given for angles file, skipping"));
    return true;
  }

  if (!quietly) {
    printMessage(tr("About to open angles input file: %1 dataset: %2")
                  .arg(fileName).arg(dataSetName));
  }

  QFileInfo f(fileName);

  hid_t fileId = -1;
  hid_t dsetId = -1;
  hid_t dspcId = -1;

  try
  {
    if (fileName.length() > 0) {
      if (!f.exists())
        throw tr("Angles file %1 does not exist").arg(fileName);
      if (H5Fis_hdf5(qPrintable(fileName)) <= 0)
        throw tr("Angles file %1 exists but is not an hdf file").arg(fileName);
    }

    if (fileName.length() == 0) { // No file name given?
      fileId = m_FileId;
      sameFile = true;
    } else if (fileName == inputFile) { // In same file as input data?
      fileId = m_FileId;
      sameFile = true;
    } else {
      fileId = H5Fopen(qPrintable(fileName), H5F_ACC_RDWR, H5P_DEFAULT);
    }

    if (fileId < 0)
      throw tr("Angles file %1 could not be opened").arg(fileName);

    dsetId = H5Dopen(fileId, qPrintable(dataSetName), H5P_DEFAULT);
    if (dsetId <= 0)
      throw tr("Could not open angles dataset!");

    dspcId = H5Dget_space(dsetId);
    if (dspcId < 0)
      throw tr("Could not get dataspace of existing angles dataset");

    hsize_t dims[3];
    int ndims = H5Sget_simple_extent_ndims(dspcId);
    if (ndims != 1)
      throw tr("Angles Dataspace is not 1-dimensional");

    int ndims2 = H5Sget_simple_extent_dims(dspcId, dims, NULL);
    if (ndims2 != 1)
      throw tr("Could not get angles dataspace dimensions");

    setAnglesDimensions(dims[0]);

    m_AnglesSameFile    = sameFile;
    m_AnglesFileId      = fileId;
    m_AnglesDatasetId   = dsetId;
    m_AnglesDataspaceId = dspcId;

    if (!quietly) {
      printMessage(tr("Opened Angles input file OK"));
    }
  }
  catch (QString &msg )
  {
    if (!quietly) {
      printMessage(tr("Error in CctwChunkedData::openAnglesFile fileId=%1, dsetId=%2, dspcId=%3")
                   .arg(fileId).arg(dsetId).arg(dspcId));
      printMessage(msg);
    }

    if (dspcId >= 0) H5Sclose(dspcId);
    if (dsetId >= 0) H5Dclose(dsetId);
    if ((fileId >= 0) && (sameFile == false)) H5Fclose(fileId);
    return false;
  }
  return true;
}

void CctwChunkedData::closeAnglesFile()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  if (m_AnglesDataspaceId >= 0) {
    H5Sclose(m_AnglesDataspaceId);
    m_AnglesDataspaceId = -1;
  }

  if (m_AnglesDatasetId >= 0) {
    H5Dclose(m_AnglesDatasetId);
    m_AnglesDatasetId = -1;
  }

  if ((m_AnglesFileId >= 0) && (m_AnglesSameFile == false)) {
    H5Fclose(m_AnglesFileId);
    m_AnglesFileId = -1;
    m_AnglesSameFile = false;
  }

  if (qcepDebug(DEBUG_APP)) {
    printMessage("Closed Angles file");
  }
}

bool CctwChunkedData::readAnglesFile()
{
  bool res = true;

  if (openAnglesFile()) {

    if (m_AnglesFileId >= 0) {
      res = false;

      QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

      hid_t memspace_id = -1;
      hsize_t offset[1], count[1], stride[1], block[1];

      QcepDoubleVector a= get_Angles();

      offset[0] = 0;
      count[0]  = a.size();
      stride[0] = 1;
      block[0]  = 1;

      double *angles = a.data();

      memspace_id = H5Screate_simple(1, count, NULL);
      herr_t selerr = H5Sselect_hyperslab(m_AnglesDataspaceId, H5S_SELECT_SET, offset, stride, count, block);

      if (selerr < 0) {
        printMessage(tr("ERROR: select_hyperslab failed!"));
      }

      herr_t rderr = H5Dread(m_AnglesDatasetId, H5T_NATIVE_DOUBLE, memspace_id, m_AnglesDataspaceId, H5P_DEFAULT, angles);

      if (selerr || rderr) {
        printMessage(tr("Error reading angles, selerr = %1, rderr = %2").arg(selerr).arg(rderr));
      } else {
        res = true;

        set_Angles(a);
      }

      H5Sclose(memspace_id);
    }

    closeAnglesFile();
  } else {
    res = false;
  }

  if (qcepDebug(DEBUG_APP)) {
    printMessage(tr("CctwChunkedData::readAnglesFile returns %1").arg(res));
  }

  return res;
}

bool CctwChunkedData::checkWeightsFile()
{
  /* Save old error handler */
  herr_t (*old_func)(hid_t, void*);
  void *old_client_data;
  H5Eget_auto(H5E_DEFAULT, &old_func, &old_client_data);

  /* Turn off error handling */
  H5Eset_auto(H5E_DEFAULT, NULL, NULL);

  bool res = openWeightsFile(true);
  closeWeightsFile();

  /* Restore previous error handler */
  H5Eset_auto(H5E_DEFAULT, old_func, old_client_data);

  return res;
}

bool CctwChunkedData::openWeightsFile(bool quietly)
{
  bool sameFile = false;

  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  if (m_WeightsFileId >= 0) {
    return true;
  }

  QString fileName    = get_WeightsDataFileName();
  QString dataSetName = get_WeightsDataSetName();
  QString inputFile   = get_DataFileName();

  if (dataSetName.length()==0) {
    printMessage(tr("No dataset name given for weights file, skipping"));
    return true;
  }

  if (!quietly) {
    printMessage(tr("About to open Weights input file: %1 dataset: %2")
                  .arg(fileName).arg(dataSetName));
  }

  QFileInfo f(fileName);

  hid_t fileId = -1;
  hid_t dsetId = -1;
  hid_t dspcId = -1;

  try
  {
    if (fileName.length() > 0) {
      if (!f.exists())
        throw tr("Weights File %1 does not exist").arg(fileName);
      if (H5Fis_hdf5(qPrintable(fileName)) <= 0)
        throw tr("Weights File %1 exists but is not an hdf file").arg(fileName);
    }

    if (fileName.length() == 0) { // No file name given?
      fileId = m_FileId;
      sameFile = true;
    } else if (fileName == inputFile) { // In same file as input data?
      fileId = m_FileId;
      sameFile = true;
    } else {
      fileId = H5Fopen(qPrintable(fileName), H5F_ACC_RDWR, H5P_DEFAULT);
    }

    if (fileId < 0)
      throw tr("Weights File %1 could not be opened").arg(fileName);

    dsetId = H5Dopen(fileId, qPrintable(dataSetName), H5P_DEFAULT);
    if (dsetId <= 0)
      throw tr("Could not open weights dataset!");

    dspcId = H5Dget_space(dsetId);
    if (dspcId < 0)
      throw tr("Could not get dataspace of existing weights dataset");

    hsize_t dims[3];
    int ndims = H5Sget_simple_extent_ndims(dspcId);
    if (ndims != 1)
      throw tr("Weights Dataspace is not 1-dimensional");

    int ndims2 = H5Sget_simple_extent_dims(dspcId, dims, NULL);
    if (ndims2 != 1)
      throw tr("Could not get weights dataspace dimensions");

    setWeightsDimensions(dims[0]);

    m_WeightsSameFile    = sameFile;
    m_WeightsFileId      = fileId;
    m_WeightsDatasetId   = dsetId;
    m_WeightsDataspaceId = dspcId;

    if (!quietly) {
      printMessage(tr("Opened Weights input file OK"));
    }
  }
  catch (QString &msg )
  {
    if (!quietly) {
      printMessage(tr("Error in CctwChunkedData::openWeightsFile fileId=%1, dsetId=%2, dspcId=%3")
                   .arg(fileId).arg(dsetId).arg(dspcId));
      printMessage(msg);
    }

    if (dspcId >= 0) H5Sclose(dspcId);
    if (dsetId >= 0) H5Dclose(dsetId);
    if ((fileId >= 0) && (sameFile == false)) H5Fclose(fileId);
    return false;
  }
  return true;
}

void CctwChunkedData::closeWeightsFile()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  if (m_WeightsDataspaceId >= 0) {
    H5Sclose(m_WeightsDataspaceId);
    m_WeightsDataspaceId = -1;
  }

  if (m_WeightsDatasetId >= 0) {
    H5Dclose(m_WeightsDatasetId);
    m_WeightsDatasetId = -1;
  }

  if ((m_WeightsFileId >= 0) && (m_WeightsSameFile == false)) {
    H5Fclose(m_WeightsFileId);
    m_WeightsFileId = -1;
    m_WeightsSameFile = false;
  }

  if (qcepDebug(DEBUG_APP)) {
    printMessage("Closed Weights file");
  }
}

bool CctwChunkedData::readWeightsFile()
{
  bool res = true;

  if (openWeightsFile()) {

    if (m_WeightsFileId >= 0) {
      res = false;

      QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

      hid_t memspace_id = -1;
      hsize_t offset[1], count[1], stride[1], block[1];

      QcepDoubleVector a= get_Weights();

      offset[0] = 0;
      count[0]  = a.size();
      stride[0] = 1;
      block[0]  = 1;

      double *Weights = a.data();

      memspace_id = H5Screate_simple(1, count, NULL);
      herr_t selerr = H5Sselect_hyperslab(m_WeightsDataspaceId, H5S_SELECT_SET, offset, stride, count, block);

      if (selerr < 0) {
        printMessage(tr("ERROR: select_hyperslab failed!"));
      }

      herr_t rderr = H5Dread(m_WeightsDatasetId, H5T_NATIVE_DOUBLE, memspace_id, m_WeightsDataspaceId, H5P_DEFAULT, Weights);

      if (selerr || rderr) {
        printMessage(tr("Error reading Weights, selerr = %1, rderr = %2").arg(selerr).arg(rderr));
      } else {
        res = true;

        set_Weights(a);
      }

      H5Sclose(memspace_id);
    }

    closeWeightsFile();
  } else {
    res = false;
  }

  if (qcepDebug(DEBUG_APP)) {
    printMessage(tr("CctwChunkedData::readWeightsFile returns %1").arg(res));
  }

  return res;
}

//CctwDataChunk *CctwChunkedData::readChunk(int n)
//{
//  CctwDataChunk *chk = chunk(n);

//  if (chk) {
//    chk->allocateData();
//    chk->allocateWeights();

//    CctwIntVector3D size = chunkSize();
//    CctwIntVector3D start = chunkStart(n);

//    for (int k=0; k<size.z(); k++) {
//      for (int j=0; j<size.y(); j++) {
//        for (int i=0; i<size.x(); i++) {
//          CctwIntVector3D coords(i,j,k);

//          int val = ((start.x()+i)/16 & 1) ^ ((start.y()+j)/16 & 1) ^ ((start.z()+k)/16 & 1);

//          chk -> setData(i, j, k, val+0.75);
//          chk -> setWeight(i, j, k, 1.0);
//        }
//      }
//    }
//  }

//  printMessage(tr("CctwChunkedData::readChunk(%1)").arg(n));

//  return chk;
//}

CctwDataChunk *CctwChunkedData::readChunk(int n)
{
  CctwDataChunk *chk = NULL;
  if (openInputFile()) {

    QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

    chk = chunk(n);

    if (chk) {
      prop_ChunksRead()->incValue(1);

      chk->allocateData();
      chk->allocateWeights();

//      printMessage(tr("About to read chunk %1").arg(n));

      if (m_FileId >= 0) {
        hid_t memspace_id = -1;
        hsize_t offset[3], count[3], stride[3], block[3];
        CctwIntVector3D st = chk -> chunkStart();
        CctwIntVector3D sz = chk -> chunkSize();

//        printMessage(tr("Reading chunk %1 [%2..%3, %4..%5, %6..%7]")
//                     .arg(n)
//                     .arg(st.x())
//                     .arg(st.x()+sz.x()-1)
//                     .arg(st.y())
//                     .arg(st.y()+sz.y()-1)
//                     .arg(st.z())
//                     .arg(st.z()+sz.z()-1)
//                     );

        count[0] = sz.z();
        count[1] = sz.y();
        count[2] = sz.x();

        stride[0] = 1;
        stride[1] = 1;
        stride[2] = 1;

        block[0] = 1;
        block[1] = 1;
        block[2] = 1;

        offset[0] = st.z();
        offset[1] = st.y();
        offset[2] = st.x();

        CctwChunkedData::MergeDataType *chunkData = chk->dataPointer();
        CctwChunkedData::MergeDataType *weightData = chk->weightsPointer();
        char *maskData = NULL;

        if (m_Mask3DDataspaceId >= 0) {
          maskData = new char[sz.volume()];
        }

        if (chunkData == NULL) {
          printMessage(tr("Anomaly reading chunk %1, data == NULL").arg(n));
        } else if ((m_TransformOptions & 4) == 0){
          memspace_id   = H5Screate_simple(3, count, NULL);
          herr_t selerr = H5Sselect_hyperslab(m_DataspaceId, H5S_SELECT_SET, offset, stride, count, block);
          if (selerr < 0) {
            printMessage(tr("ERROR: select_hyperslab failed!"));
          }
          herr_t rderr  = H5Dread(m_DatasetId, CCTW_H5T_INTERNAL_TYPE, memspace_id, m_DataspaceId, H5P_DEFAULT, chunkData);

          if (selerr || rderr) {
            printMessage(tr("Error reading chunk %1, selerr = %2, rderr = %3")
                         .arg(n)
                         .arg(selerr)
                         .arg(rderr));
          }

          if ((m_Dataspace2Id >= 0) && (m_Dataset2Id >= 0)) {
            herr_t selerr = H5Sselect_hyperslab(m_Dataspace2Id, H5S_SELECT_SET, offset, stride, count, block);
            if (selerr < 0) {
              printMessage(tr("ERROR: select_hyperslab failed!"));
            }

            herr_t rderr = H5Dread(m_Dataset2Id, CCTW_H5T_INTERNAL_TYPE, memspace_id, m_Dataspace2Id, H5P_DEFAULT, weightData);

            if (selerr || rderr) {
              printMessage(tr("Error reading weight chunk %1, selerr = %2, rderr = %3")
                           .arg(n)
                           .arg(selerr)
                           .arg(rderr));
            }
          }

          if ((m_Mask3DDataspaceId >= 0) && (m_Mask3DDatasetId >= 0)) {
            herr_t selerr = H5Sselect_hyperslab(m_Mask3DDataspaceId, H5S_SELECT_SET, offset, stride,count, block);
            if (selerr < 0) {
              printMessage(tr("ERROR: select_hyperslab failed!"));
            }

            herr_t rderr = H5Dread(m_Mask3DDatasetId, H5T_NATIVE_CHAR, memspace_id, m_Mask3DDataspaceId, H5P_DEFAULT, maskData);

            if (selerr || rderr) {
              printMessage(tr("Error reading mask chunk %1, selerr = %2, rderr = %3")
                           .arg(n)
                           .arg(selerr)
                           .arg(rderr));
            }
          }

          H5Sclose(memspace_id);
        }

        int nskipped=0;
        int s = sz.volume();

        if (maskData && chunkData) {
          for (int i=0; i<s; i++) {
            if (maskData[i]) {
              chunkData[i] = qQNaN();
            }
          }
        }

        if (chunkData) {
          for (int i=0; i<s; i++) {
            if (chunkData[i] != chunkData[i]) { // Test for NaN
              nskipped++;
            }
          }
        }

        if (m_Dataspace2Id < 0) {
          if (weightData && chunkData) {
            for (int i=0; i<s; i++) {
              if (chunkData[i] == chunkData[i]) { // Test for NaN
                weightData[i] = 1;
              } else {
                weightData[i] = 0;
              }
            }
          }
        }

        delete maskData;

        chk->setSkippedPixels(nskipped);
      }
    }
  }

//  printMessage(tr("CctwChunkedData::readChunk(%1)").arg(n));

  return chk;
}

void CctwChunkedData::writeChunk(int n)
{
  if (openOutputFile()) {
    QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

//    printMessage(tr("Writing chunk %1").arg(n));

    CctwDataChunk *chk = chunk(n);

    if (chk) {
//      prop_ChunksWritten()->incValue(1);

      if (get_Normalization()) {
        normalizeChunk(n);
      }

      if (m_FileId >= 0) {

        hsize_t offset[3], count[3], stride[3], block[3];

        CctwIntVector3D st = chk -> chunkStart();
        CctwIntVector3D sz = chk -> chunkSize();

//        printMessage(tr("Writing chunk %1 [%2..%3, %4..%5, %6..%7]")
//                     .arg(n)
//                     .arg(st.x())
//                     .arg(st.x()+sz.x()-1)
//                     .arg(st.y())
//                     .arg(st.y()+sz.y()-1)
//                     .arg(st.z())
//                     .arg(st.z()+sz.z()-1)
//                     );

        count[0] = sz.z();
        count[1] = sz.y();
        count[2] = sz.x();

        stride[0] = 1;
        stride[1] = 1;
        stride[2] = 1;

        block[0] = 1;
        block[1] = 1;
        block[2] = 1;

        offset[0] = st.z();
        offset[1] = st.y();
        offset[2] = st.x();

        CctwChunkedData::MergeDataType *chunkData = chk->dataPointer();
        CctwChunkedData::MergeDataType *weightData = chk->weightsPointer();

        if (chunkData == NULL) {
//          printMessage(tr("Anomaly writing chunk %1, data == NULL").arg(n));
        } else if ((m_TransformOptions & 8) == 0){
          hid_t memspace_id = -1;
          memspace_id   = H5Screate_simple(3, count, NULL);
          herr_t selerr = H5Sselect_hyperslab(m_DataspaceId, H5S_SELECT_SET, offset, stride, count, block);
          herr_t wrterr = H5Dwrite(m_DatasetId, CCTW_H5T_INTERNAL_TYPE, memspace_id, m_DataspaceId, H5P_DEFAULT, chunkData);

          if (selerr || wrterr) {
            printMessage(tr("Error writing chunk %1, selerr = %2, wrterr = %3")
                         .arg(n)
                         .arg(selerr)
                         .arg(wrterr));
          }

          if ((m_Dataspace2Id >= 0) && (m_Dataset2Id >= 0)) {
            // Write out the 'weight' chunk
            herr_t selerr = H5Sselect_hyperslab(m_Dataspace2Id, H5S_SELECT_SET, offset, stride, count, block);
            herr_t wrterr = H5Dwrite(m_Dataset2Id, CCTW_H5T_INTERNAL_TYPE, memspace_id, m_Dataspace2Id, H5P_DEFAULT, weightData);

            if (selerr || wrterr) {
              printMessage(tr("Error writing chunk %1, selerr = %2, wrterr = %3")
                           .arg(n)
                           .arg(selerr)
                           .arg(wrterr));
            }
          }
          H5Sclose(memspace_id);
        }
      }
    }

    releaseChunkData(n);
  }
}

bool CctwChunkedData::beginTransform(bool isInput, int transformOptions)
{
  bool res = false;

  m_IsInput = isInput;
  m_TransformOptions = transformOptions;

//  printMessage("CctwChunkedData::beginTransform()");

  if (m_IsInput) {
    if (openInputFile()) {
      //      printMessage("Reading mask data");
      if (readMaskFile()) {
        //        printMessage("Reading 3D mask data");
        if (openMask3DFile()) {
          //        printMessage("Reading angles data");
          if (readAnglesFile()) {
            //          printMessage("Reading weights data");
            if (readWeightsFile()) {
              res = true;
            }
          }
        }
      }
    }
  } else {
    res = openOutputFile();
  }

  if (qcepDebug(DEBUG_APP)) {
    printMessage(tr("Begin transform: ds %1, ds2 %2, m3d %3")
                 .arg(m_DataspaceId)
                 .arg(m_Dataspace2Id)
                 .arg(m_Mask3DDataspaceId));
  }

  return res;
}

void CctwChunkedData::endTransform()
{
//  printMessage("CctwChunkedData::endTransform()");

  if (m_IsInput) {
    closeInputFile();
    closeMask3DFile();
  } else {
    closeOutputFile();
  }
}

void CctwChunkedData::incChunksRead(int n)
{
  prop_ChunksRead()->incValue(n);
}

void CctwChunkedData::incChunksWritten(int n)
{
  prop_ChunksWritten()->incValue(n);
}

void CctwChunkedData::incChunksHeld(int n)
{
  prop_ChunksHeld()->incValue(n);

  if (get_ChunksHeld() > get_ChunksHeldMax()) {
    set_ChunksHeldMax(get_ChunksHeld());
  }
}
