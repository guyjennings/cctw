#include "cctwchunkeddata.h"
#include "cctwdatachunk.h"
#include "cctwapplication.h"
#include <QDir>

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
   QObject         *parent)
  :CctwObject(name, parent),
    m_IsNeXus(false),
    m_DimensionsCache(dim),
    m_ChunkSizeCache(chunkSize),
    m_ChunkCountCache((dim-CctwIntVector3D(1,1,1))/chunkSize + CctwIntVector3D(1,1,1)),
    m_DataFileName(application->saver(), this, "dataFileName", "input.h5", "HDF5 Data File Name"),
    m_DataSetName(application->saver(), this, "dataSetName", "data", "HDF5 Dataset name"),
    m_MaskDataFileName(application->saver(), this, "maskDataFileName", "", "Mask Data File Name"),
    m_MaskDataSetName(application->saver(), this, "maskDataSetName", "", "Mask Dataset Name"),
    m_AnglesDataFileName(application->saver(), this, "anglesDataFileName", "", "Angles Data File Name"),
    m_AnglesDataSetName(application->saver(), this, "anglesDataSetName", "", "Angles Dataset Name"),
    m_Dimensions(application->saver(), this, "dimensions", m_DimensionsCache, "Dataset Dimensions"),
    m_ChunkSize(application->saver(), this, "chunkSize", m_ChunkSizeCache, "Chunk Size"),
    m_ChunkCount(QcepSettingsSaverWPtr(), this, "chunkCount", m_ChunkCountCache, "Chunk Count"),
    m_Compression(application->saver(), this, "compression", 0, "Compression Level"),
    m_HDFChunkSize(application->saver(), this, "hdfChunkSize", CctwIntVector3D(0,0,0), "HDF File Chunk Size"),
    m_ChunksRead(QcepSettingsSaverWPtr(), this, "chunksRead", 0, "Chunks read from input"),
    m_ChunksWritten(QcepSettingsSaverWPtr(), this, "chunksWritten", 0, "Chunks written to output"),
    m_ChunksHeld(QcepSettingsSaverWPtr(), this, "chunksHeld", 0, "Chunks held on output"),
    m_ChunksHeldMax(QcepSettingsSaverWPtr(), this, "chunksHeldMax", 0, "Max Chunks held on output"),
    m_IsInput(isInput),
    m_TransformOptions(0),
    m_FileId(-1),
    m_DatasetId(-1),
    m_DataspaceId(-1),
    m_MaskFileId(-1),
    m_MaskDatasetId(-1),
    m_MaskDataspaceId(-1),
    m_AnglesFileId(-1),
    m_AnglesDatasetId(-1),
    m_AnglesDataspaceId(-1)
{
  allocateChunks();
  connect(prop_DataFileName(), SIGNAL(valueChanged(QString,int)), this, SLOT(onDataFileNameChanged()));
  connect(prop_DataSetName(), SIGNAL(valueChanged(QString,int)), this, SLOT(onDataFileNameChanged()));
  connect(prop_MaskDataFileName(), SIGNAL(valueChanged(QString,int)), this, SLOT(onMaskFileNameChanged()));
  connect(prop_MaskDataSetName(), SIGNAL(valueChanged(QString,int)), this, SLOT(onMaskFileNameChanged()));
  connect(prop_AnglesDataFileName(), SIGNAL(valueChanged(QString,int)), this, SLOT(onAnglesChanged()));
  connect(prop_AnglesDataSetName(), SIGNAL(valueChanged(QString,int)), this, SLOT(onAnglesChanged()));
  connect(this, SIGNAL(dimensionsChanged(CctwIntVector3D)), prop_Dimensions(), SLOT(setValue(CctwIntVector3D)));
  connect(this, SIGNAL(chunkSizeChanged(CctwIntVector3D)), prop_ChunkSize(), SLOT(setValue(CctwIntVector3D)));
  connect(this, SIGNAL(chunkCountChanged(CctwIntVector3D)), prop_ChunkCount(), SLOT(setValue(CctwIntVector3D)));

  connect(prop_Dimensions(), SIGNAL(valueChanged(CctwIntVector3D,int)), this, SLOT(sizingChanged()));
  connect(prop_ChunkSize(), SIGNAL(valueChanged(CctwIntVector3D,int)), this, SLOT(sizingChanged()));
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

void CctwChunkedData::setDimensions(CctwIntVector3D dim)
{
  if (m_DimensionsCache != dim) {
    m_DimensionsCache = dim;

    m_ChunkCountCache = ((m_DimensionsCache-CctwIntVector3D(1,1,1))/m_ChunkSizeCache+CctwIntVector3D(1,1,1));

    allocateChunks();

    emit dimensionsChanged(m_DimensionsCache);
    emit chunkCountChanged(m_ChunkCountCache);
  }
}

void CctwChunkedData::setChunkSize(CctwIntVector3D cksz)
{
  if (m_ChunkSizeCache != cksz) {
    m_ChunkSizeCache = cksz;

    m_ChunkCountCache = ((m_DimensionsCache-CctwIntVector3D(1,1,1))/m_ChunkSizeCache+CctwIntVector3D(1,1,1));

    allocateChunks();

    emit chunkSizeChanged(m_ChunkSizeCache);
    emit chunkCountChanged(m_ChunkCountCache);
  }
}

void CctwChunkedData::setMaskDimensions(int mx, int my)
{

}

void CctwChunkedData::setAnglesDimensions(int n)
{

}

void CctwChunkedData::sizingChanged()
{
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
  printMessage(tr("%1:setDataSource(\"%2\")").arg(get_Name()).arg(CctwApplication::addSlashes(desc)));

  QUrl url(desc);

  printMessage(tr("scheme:   %1").arg(url.scheme()));
  printMessage(tr("host:     %1").arg(url.host()));
  printMessage(tr("path:     %1").arg(url.path()));
//  printMessage(tr("filename: %1").arg(url.fileName()));
#if QT_VERSION >= 0x050000
  printMessage(tr("query:    %1").arg(url.query()));
#endif
  printMessage(tr("fragment: %1").arg(url.fragment()));

  set_DataFileName(url.path());
  set_DataSetName(url.fragment());

#if QT_VERSION >= 0x050000
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
  printMessage(tr("%1:setMaskSource(\"%2\")").arg(get_Name()).arg(CctwApplication::addSlashes(desc)));

  QUrl url(desc);

  printMessage(tr("scheme:   %1").arg(url.scheme()));
  printMessage(tr("host:     %1").arg(url.host()));
  printMessage(tr("path:     %1").arg(url.path()));
//  printMessage(tr("filename: %1").arg(url.fileName()));
#if QT_VERSION >= 0x050000
  printMessage(tr("query:    %1").arg(url.query()));
#endif
  printMessage(tr("fragment: %1").arg(url.fragment()));

  set_MaskDataFileName(url.path());
  set_MaskDataSetName(url.fragment());

#if QT_VERSION >= 0x050000
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

void CctwChunkedData::setMaskDataset(QString desc)
{
  set_MaskDataSetName(desc);
}

void CctwChunkedData::setAnglesSource(QString desc)
{
  printMessage(tr("%1:setAnglesSource(\"%2\")").arg(get_Name()).arg(CctwApplication::addSlashes(desc)));

  QUrl url(desc);

  printMessage(tr("scheme:   %1").arg(url.scheme()));
  printMessage(tr("host:     %1").arg(url.host()));
  printMessage(tr("path:     %1").arg(url.path()));
//  printMessage(tr("filename: %1").arg(url.fileName()));
#if QT_VERSION >= 0x050000
  printMessage(tr("query:    %1").arg(url.query()));
#endif
  printMessage(tr("fragment: %1").arg(url.fragment()));

  set_AnglesDataFileName(url.path());
  set_AnglesDataSetName(url.fragment());

#if QT_VERSION >= 0x050000
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

void CctwChunkedData::clearDependencies()
{
  foreach(CctwDataChunk* p, m_DataChunks) {
    if (p) {
      p->clearDependencies();
    }
  }
}

void CctwChunkedData::addDependency(int f, int t)
{
  if (f >= 0 && f < m_DataChunks.count()) {
    chunk(f) -> addDependency(t);
  }
}

CctwDataChunk *CctwChunkedData::chunk(int n)
{
  if (n >= 0 && n < m_DataChunks.count()) {
    QcepMutexLocker lock(__FILE__, __LINE__, &m_ChunkLock);

    CctwDataChunk *chunk = m_DataChunks[n];

    if (chunk == NULL) {
      chunk = new CctwDataChunk(this, n, tr("Chunk-%1").arg(n), NULL);

      chunk->moveToThread(parent()->thread());
      chunk->setParent(parent());

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
  closeInputFile(true);

  /* Restore previous error handler */
  H5Eset_auto(H5E_DEFAULT, old_func, old_client_data);

  return res;
}

bool CctwChunkedData::openInputFile(bool quietly)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  if (m_FileId >= 0) {
    return true;
  }

  QString fileName    = get_DataFileName();
  QString dataSetName = get_DataSetName();

  if (!quietly) {
    printMessage(tr("About to open input file: %1 dataset: %2")
                  .arg(fileName).arg(dataSetName));
  }

  QFileInfo f(fileName);

  hid_t fileId = -1;
  hid_t dsetId = -1;
  hid_t dspcId = -1;
  hid_t plist  = -1;

  try
  {
    if (!f.exists())
      throw tr("File %1 does not exist").arg(fileName);
    if (H5Fis_hdf5(qPrintable(fileName)) <= 0)
      throw tr("File %1 exists but is not an hdf file").arg(fileName);

    fileId = H5Fopen(qPrintable(fileName), H5F_ACC_RDWR, H5P_DEFAULT);
    if (fileId < 0)
      throw tr("File %1 could not be opened").arg(fileName);

    dsetId = H5Dopen(fileId, qPrintable(dataSetName), H5P_DEFAULT);
    if (dsetId <= 0)
      throw tr("Could not open dataset!");

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
    } else {
      set_HDFChunkSize(CctwIntVector3D(cksz[2], cksz[1], cksz[0]));
    }

    m_FileId      = fileId;
    m_DatasetId   = dsetId;
    m_DataspaceId = dspcId;

    if (!quietly) {
      printMessage(tr("Opened input file OK"));
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
    if (dsetId >= 0) H5Dclose(dsetId);
    if (fileId >= 0) H5Fclose(fileId);
    return false;
  }
  return true;
}

void CctwChunkedData::closeInputFile(bool quietly)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  if (m_IsNeXus)
    closeInputNeXusFile();

  if (m_DataspaceId >= 0) {
    H5Sclose(m_DataspaceId);
    m_DataspaceId = -1;
  }

  if (m_DatasetId >= 0) {
    H5Dclose(m_DatasetId);
    m_DatasetId = -1;
  }

  if (m_FileId >= 0) {
    H5Fclose(m_FileId);
    m_FileId = -1;
  }

  if (!quietly) {
    printMessage("Closed input file");
  }
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

  printMessage(tr("About to open output file: %1 dataset: %2")
                .arg(fileName).arg(dataSetName));

  int res = true;

  QFileInfo f(fileName);

  hid_t fileId = -1;
  hid_t gplist = -1;
  hid_t dsetId = -1;
  hid_t dspcId = -1;
  hid_t plist  = -1;

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

    dsetId = H5Dopen(fileId, qPrintable(get_DataSetName()), H5P_DEFAULT);

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
        } else if (cmprs) {
          if (H5Pset_deflate(plist, cmprs) < 0) {
            printMessage("Set deflate failed");
            res = false;
          }
        }
      }

      gplist = H5Pcreate(H5P_LINK_CREATE);

      if (H5Pset_create_intermediate_group(gplist, 1) < 0) {
        printMessage(tr("Failed to set create intermediate groups"));
        res = false;
      } else {
        dsetId = H5Dcreate(fileId,
                           qPrintable(get_DataSetName()),
                           H5T_NATIVE_FLOAT,
                           dspcId,
                           gplist,
                           plist,
                           H5P_DEFAULT);
      }
    }

    if (dsetId < 0) {
      printMessage(tr("Could not create or find dataset %1").arg(get_DataSetName()));
      res = false;
    }
  }

  if (plist  >= 0) H5Pclose(plist);
  if (gplist >= 0) H5Pclose(gplist);

  if (res == false) {
    if (dspcId >= 0) H5Sclose(dspcId);
    if (dsetId >= 0) H5Dclose(dsetId);
    if (fileId >= 0) H5Fclose(fileId);
  } else {
    if (fileId < 0 || dsetId < 0 || dspcId < 0) {
      printMessage(tr("Anomaly in CctwChunkedData::openOutputFile fileId=%1, dsetId=%2, dspcId=%3")
                   .arg(fileId).arg(dsetId).arg(dspcId));
    }

    m_FileId      = fileId;
    m_DatasetId   = dsetId;
    m_DataspaceId = dspcId;

    printMessage("Opened output file OK");
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

  if (m_FileId >= 0) {
    H5Fclose(m_FileId);
    m_FileId = -1;
  }

  printMessage("Closed output file");
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
  closeMaskFile(true);

  /* Restore previous error handler */
  H5Eset_auto(H5E_DEFAULT, old_func, old_client_data);

  return res;
}

bool CctwChunkedData::openMaskFile(bool quietly)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  if (m_FileId >= 0) {
    return true;
  }

  QString fileName    = get_MaskDataFileName();
  QString dataSetName = get_MaskDataSetName();

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
    if (!f.exists())
      throw tr("File %1 does not exist").arg(fileName);
    if (H5Fis_hdf5(qPrintable(fileName)) <= 0)
      throw tr("File %1 exists but is not an hdf file").arg(fileName);

    fileId = H5Fopen(qPrintable(fileName), H5F_ACC_RDWR, H5P_DEFAULT);
    if (fileId < 0)
      throw tr("File %1 could not be opened").arg(fileName);

    dsetId = H5Dopen(fileId, qPrintable(dataSetName), H5P_DEFAULT);
    if (dsetId <= 0)
      throw tr("Could not open dataset!");

    dspcId = H5Dget_space(dsetId);
    if (dspcId < 0)
      throw tr("Could not get dataspace of existing dataset");

    hsize_t dims[3];
    int ndims = H5Sget_simple_extent_ndims(dspcId);
    if (ndims != 2)
      throw tr("Dataspace is not 2-dimensional");

    int ndims2 = H5Sget_simple_extent_dims(dspcId, dims, NULL);
    if (ndims2 != 2)
      throw tr("Could not get dataspace dimensions");

    setMaskDimensions(dims[1], dims[0]);

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
      printMessage(tr("Anomaly in CctwChunkedData::openMaskFile fileId=%1, dsetId=%2, dspcId=%3")
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

void CctwChunkedData::closeMaskFile(bool quietly)
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

  if (m_MaskFileId >= 0) {
    H5Fclose(m_MaskFileId);
    m_MaskFileId = -1;
  }

  if (!quietly) {
    printMessage("Closed mask input file");
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
  closeAnglesFile(true);

  /* Restore previous error handler */
  H5Eset_auto(H5E_DEFAULT, old_func, old_client_data);

  return res;
}

bool CctwChunkedData::openAnglesFile(bool quietly)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  if (m_FileId >= 0) {
    return true;
  }

  QString fileName    = get_AnglesDataFileName();
  QString dataSetName = get_AnglesDataSetName();

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
    if (!f.exists())
      throw tr("File %1 does not exist").arg(fileName);
    if (H5Fis_hdf5(qPrintable(fileName)) <= 0)
      throw tr("File %1 exists but is not an hdf file").arg(fileName);

    fileId = H5Fopen(qPrintable(fileName), H5F_ACC_RDWR, H5P_DEFAULT);
    if (fileId < 0)
      throw tr("File %1 could not be opened").arg(fileName);

    dsetId = H5Dopen(fileId, qPrintable(dataSetName), H5P_DEFAULT);
    if (dsetId <= 0)
      throw tr("Could not open dataset!");

    dspcId = H5Dget_space(dsetId);
    if (dspcId < 0)
      throw tr("Could not get dataspace of existing dataset");

    hsize_t dims[3];
    int ndims = H5Sget_simple_extent_ndims(dspcId);
    if (ndims != 1)
      throw tr("Dataspace is not 1-dimensional");

    int ndims2 = H5Sget_simple_extent_dims(dspcId, dims, NULL);
    if (ndims2 != 1)
      throw tr("Could not get dataspace dimensions");

    setAnglesDimensions(dims[0]);

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
      printMessage(tr("Anomaly in CctwChunkedData::openAnglesFile fileId=%1, dsetId=%2, dspcId=%3")
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

void CctwChunkedData::closeAnglesFile(bool quietly)
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

  if (m_AnglesFileId >= 0) {
    H5Fclose(m_AnglesFileId);
    m_AnglesFileId = -1;
  }

  if (!quietly) {
    printMessage("Closed Angles file");
  }
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

          H5Sclose(memspace_id);
        }

        if (weightData && chunkData) {
          int s = sz.volume();

          for (int i=0; i<s; i++) {
            if (chunkData[i] == chunkData[i]) { // Test for NaN
              weightData[i] = 1;
            } else {
              weightData[i] = 0;
            }
          }
        }
      }
    }
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
  }

//  printMessage(tr("CctwChunkedData::readChunk(%1)").arg(n));

  return chk;
}

void CctwChunkedData::writeChunk(int n)
{
  if (openOutputFile()) {
    QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

    printMessage(tr("Writing chunk %1").arg(n));

    CctwDataChunk *chk = chunk(n);

    if (chk) {
//      prop_ChunksWritten()->incValue(1);

      normalizeChunk(n);

      if (m_FileId >= 0) {

        hid_t memspace_id = -1;
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

        if (chunkData == NULL) {
          printMessage(tr("Anomaly writing chunk %1, data == NULL").arg(n));
        } else if ((m_TransformOptions & 8) == 0){
          memspace_id   = H5Screate_simple(3, count, NULL);
          herr_t selerr = H5Sselect_hyperslab(m_DataspaceId, H5S_SELECT_SET, offset, stride, count, block);
          herr_t wrterr = H5Dwrite(m_DatasetId, CCTW_H5T_INTERNAL_TYPE, memspace_id, m_DataspaceId, H5P_DEFAULT, chunkData);

          if (selerr || wrterr) {
            printMessage(tr("Error writing chunk %1, selerr = %2, wrterr = %3")
                         .arg(n)
                         .arg(selerr)
                         .arg(wrterr));
          }

          H5Sclose(memspace_id);
        }
      }
    }

    releaseChunkData(n);
  }
}

void CctwChunkedData::beginTransform(bool isInput, int transformOptions)
{
  m_IsInput = isInput;
  m_TransformOptions = transformOptions;

//  printMessage("CctwChunkedData::beginTransform()");

  if (m_IsInput) {
    openInputFile();
  } else {
    openOutputFile();
  }
}

void CctwChunkedData::endTransform()
{
//  printMessage("CctwChunkedData::endTransform()");

  if (m_IsInput) {
    closeInputFile();
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
