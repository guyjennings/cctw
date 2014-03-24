#include "cctwchunkeddata.h"
#include "cctwdatachunk.h"
#include "cctwapplication.h"
#include <QUrlQuery>
#include "cctwdebug.h"
#include "qcepmutexlocker.h"

CctwChunkedData::CctwChunkedData
  (CctwApplication *application,
   CctwIntVector3D  dim,
   CctwIntVector3D  chunkSize,
   bool             isInput,
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
    m_Compression(application->saver(), this, "compression", 0, "Compression Level"),
    m_HDFChunkSize(application->saver(), this, "hdfChunkSize", CctwIntVector3D(0,0,0), "HDF File Chunk Size"),
    m_IsInput(isInput),
    m_FileId(-1),
    m_DatasetId(-1),
    m_DataspaceId(-1)
{
  allocateChunks();
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
    m_DataChunks[i] = new CctwDataChunk(this, i, tr("chunk-%1").arg(i), parent());
  }
}

void CctwChunkedData::setDimensions(CctwIntVector3D dim)
{
  if (m_Dimensions != dim) {
    m_Dimensions = dim;

    m_ChunkCount = ((m_Dimensions-CctwIntVector3D(1,1,1))/m_ChunkSize+CctwIntVector3D(1,1,1));

    allocateChunks();
  }
}

void CctwChunkedData::setChunkSize(CctwIntVector3D cksz)
{
  if (m_ChunkSize != cksz) {
    m_ChunkSize = cksz;

    m_ChunkCount = ((m_Dimensions-CctwIntVector3D(1,1,1))/m_ChunkSize+CctwIntVector3D(1,1,1));

    allocateChunks();
  }
}

void CctwChunkedData::setDataSource(QString desc)
{
  printMessage(tr("%1:setDataSource(\"%2\")").arg(get_Name()).arg(CctwApplication::addSlashes(desc)));

  QUrl url(desc);

  printMessage(tr("scheme:   %1").arg(url.scheme()));
  printMessage(tr("host:     %1").arg(url.host()));
  printMessage(tr("path:     %1").arg(url.path()));
  printMessage(tr("filename: %1").arg(url.fileName()));
  printMessage(tr("query:    %1").arg(url.query()));
  printMessage(tr("fragment: %1").arg(url.fragment()));

  set_DataFileName(url.fileName());
  set_DataSetName(url.fragment());

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

bool                CctwChunkedData::containsPixel(CctwIntVector3D pixelCoord)
{
  return pixelCoord.x() >= 0 &&
      pixelCoord.y() >= 0 &&
      pixelCoord.z() >= 0 &&
      pixelCoord.x() < m_Dimensions.x() &&
      pixelCoord.y() < m_Dimensions.y() &&
      pixelCoord.z() < m_Dimensions.z();
}

CctwIntVector3D     CctwChunkedData::chunkStart(int n)
{
  // Return pixel coords of start of chunk chunkIdx

  CctwIntVector3D chunkIdx = chunkIndexFromNumber(n);

  return m_ChunkSize*chunkIdx;
}

int     CctwChunkedData::chunkContaining(CctwIntVector3D pixelCoord)
{
  // Return index of chunk containing given pixel

  CctwIntVector3D res = pixelCoord / m_ChunkSize;

  return chunkNumberFromIndex(res);
}

int     CctwChunkedData::chunkContaining(CctwDoubleVector3D fracPixelCoord)
{
  CctwDoubleVector3D fchk = fracPixelCoord / CctwDoubleVector3D(m_ChunkSize.x(), m_ChunkSize.y(), m_ChunkSize.z());

  return chunkNumberFromIndex(CctwIntVector3D(fchk.x(), fchk.y(), fchk.z()));
}

CctwIntVector3D     CctwChunkedData::chunkCount() const
{
  return m_ChunkCount;
}

bool CctwChunkedData::containsChunk(int ix, int iy, int iz)
{
  return ix>=0 && ix<m_ChunkCount.x() &&
         iy>=0 && iy<m_ChunkCount.y() &&
         iz>=0 && iz<m_ChunkCount.z();
}

int CctwChunkedData::chunkNumberFromIndex(CctwIntVector3D chunkIdx)
{
  if (containsChunk(chunkIdx.x(), chunkIdx.y(), chunkIdx.z())) {
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

void CctwChunkedData::addDependency(int f, int t)
{
  if (f >= 0 && f < m_DataChunks.count()) {
    m_DataChunks[f] ->addDependency(t);
  }
}

CctwDataChunk *CctwChunkedData::chunk(int n)
{
  if (n >= 0 && n < m_DataChunks.count()) {
    CctwDataChunk *chunk = m_DataChunks[n];

    if (chunk && chunk->index() != n) {
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
}

void CctwChunkedData::normalizeChunk(int n)
{
  CctwDataChunk *chunk = this->chunk(n);

  if (chunk) {
    chunk->normalizeChunk();
  }
}

//int CctwDataChunk::normalize()
//{
//  if (m_Normalized) {
//    printf("Already normalized\n");
//  } else if (m_ChunkData && m_ChunkWeights) {
//    int cksz = m_Data->chunkSize().volume();

//    for (int i=0; i<cksz; i++) {
//      if (m_ChunkWeights[i] != 0) {
//        m_ChunkData[i] /= m_ChunkWeights[i];
//      }
//    }
//  }

//  m_Normalized = true;

//  return 0;
//}

//int CctwDataChunk::writeData()
//{
//  normalize();

////  if (m_Manager) {
////    m_Manager->writeChunk(this);
////  }

//  return chunkSize().volume()*sizeof(double);
//}

//int CctwDataChunk::writeWeights()
//{
//  return chunkSize().volume()*sizeof(double);
//}


//void CctwDataChunk::waitForData()
//{
//  int nbuff = dependencyCount();

//  if (nbuff > g_AllocationLimit.fetchAndAddOrdered(0)) {
//    printMessage(tr("Trying to allocate too many blocks - will sleep 5 secs then proceed anyway"));

//    CctwThread::sleep(5);
//  } else {
////    printMessage(tr("Trying to acquire %1 blocks, %2 available").arg(nbuff).arg(g_Available.available()));

//    if (!g_Available.tryAcquire(nbuff)) {
//      printMessage(tr("Failed to acquire %1 blocks").arg(nbuff));

//      g_Available.acquire(nbuff);
//    }
//  }
//}

//void CctwDataChunk::finishedWithData()
//{
//  int nbuff = dependencyCount();

//  if (nbuff > g_AllocationLimit.fetchAndAddOrdered(0)) {
//    printMessage(tr("Skipped release"));
//  } else {
//    g_Available.release(nbuff);

////    printMessage(tr("Releasing %1 blocks, %2 available").arg(nbuff).arg(g_Available.available()));
//  }
//}

bool CctwChunkedData::openInputFile()
{
  if (m_FileId >= 0) {
    return true;
  }

  printMessage(tr("About to open input file"));

  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  int res = true;

  QString fileName = get_DataFileName();
  QFileInfo f(fileName);

  hid_t fileId = -1;
  hid_t dsetId = -1;
  hid_t dspcId = -1;

  if (!f.exists()) {
    printMessage(tr("File %1 does not exist").arg(fileName));
    res = false;
  } else if (H5Fis_hdf5(qPrintable(fileName)) <= 0) {
    printMessage(tr("File %1 exists but is not an hdf file").arg(fileName));
    res = false;
  } else {
    fileId = H5Fopen(qPrintable(fileName), H5F_ACC_RDWR, H5P_DEFAULT);

    if (fileId < 0) {
      printMessage(tr("File %1 could not be opened").arg(fileName));
      res = false;
    } else {
      dsetId = H5Dopen(fileId, qPrintable(get_DataSetName()), H5P_DEFAULT);

      if (dsetId >= 0) {
        dspcId = H5Dget_space(dsetId);

        if (dspcId < 0) {
          printMessage("Could not get dataspace of existing dataset");
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
              printMessage("Could not get dataspace dimensions");
              res = false;
            } else {
              setDimensions(CctwIntVector3D(dims[2], dims[1], dims[0]));
            }
          }
        }
      }
    }
  }

  if (res == false) {
    if (dspcId >= 0) H5Sclose(dspcId);
    if (dsetId >= 0) H5Dclose(dsetId);
    if (fileId >= 0) H5Fclose(fileId);
  } else {
    if (fileId < 0 || dsetId < 0 || dspcId < 0) {
      printMessage(tr("Anomaly in CctwChunkedData::openInputFile fileId=%1, dsetId=%2, dspcId=%3")
                   .arg(fileId).arg(dsetId).arg(dspcId));
    }

    m_FileId      = fileId;
    m_DatasetId   = dsetId;
    m_DataspaceId = dspcId;
  }

  return res;
}

void CctwChunkedData::closeInputFile()
{
  printMessage("About to close input file");

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
}

bool CctwChunkedData::openOutputFile()
{
  if (m_FileId >= 0) {
    return true;
  }

  printMessage("About to open output file");

  QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

  int res = true;

  QString fileName = get_DataFileName();
  QFileInfo f(fileName);

  hid_t fileId = -1;
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
    dsetId = H5Dopen(fileId, qPrintable(get_DataSetName()), H5P_DEFAULT);

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
              printMessage("Dataspace dimensions do not match imported data");
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

        if (H5Pset_chunk(plist, 3, c) < 0) {
          printMessage("Failed to set chunk size");
          res = false;
        } else if (cmprs) {
          if (H5Pset_deflate(plist, cmprs) < 0) {
            printMessage("Set deflate failed");
            res = false;
          }
        }
      }

      dsetId = H5Dcreate(fileId,
                         qPrintable(get_DataSetName()),
                         H5T_NATIVE_FLOAT,
                         dspcId,
                         H5P_DEFAULT,
                         plist,
                         H5P_DEFAULT);
    }

    if (dsetId < 0) {
      printMessage(tr("Could not create or find dataset %1").arg(get_DataSetName()));
      res = false;
    }
  }

  if (plist  >= 0) H5Pclose(plist);

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
  }

  return res;
}

void CctwChunkedData::closeOutputFile()
{
  printMessage("About to close output file");

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
  CctwDataChunk *chk = chunk(n);

  if (chk) {
    chk->allocateData();
    chk->allocateWeights();

    if (openInputFile()) {
      printMessage(tr("About to read chunk %1").arg(n));

      QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

      if (m_FileId >= 0) {
        hid_t memspace_id = -1;
        hsize_t offset[3], count[3], stride[3], block[3];
        CctwIntVector3D st = chk -> chunkStart();
        CctwIntVector3D sz = chk -> chunkSize();

        printMessage(tr("Reading chunk %1 [%2..%3, %4..%5, %6..%7]")
                     .arg(n)
                     .arg(st.x())
                     .arg(st.x()+sz.x()-1)
                     .arg(st.y())
                     .arg(st.y()+sz.y()-1)
                     .arg(st.z())
                     .arg(st.z()+sz.z()-1)
                     );

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

        double *chunkData = chk->dataPointer();
        double *weightData = chk->weightsPointer();

        if (chunkData == NULL) {
          printMessage(tr("Anomaly reading chunk %1, data == NULL").arg(n));
        } else {
          memspace_id   = H5Screate_simple(3, count, NULL);
          herr_t selerr = H5Sselect_hyperslab(m_DataspaceId, H5S_SELECT_SET, offset, stride, count, block);
          herr_t rderr  = H5Dread(m_DatasetId, H5T_NATIVE_DOUBLE, memspace_id, m_DataspaceId, H5P_DEFAULT, chunkData);

          if (selerr || rderr) {
            printMessage(tr("Error reading chunk %1, selerr = %2, rderr = %3")
                         .arg(n)
                         .arg(selerr)
                         .arg(rderr));
          }

          H5Sclose(memspace_id);
        }

        if (weightData) {
          int s = sz.volume();

          for (int i=0; i<s; i++) {
            weightData[i] = 1;
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

  printMessage(tr("CctwChunkedData::readChunk(%1)").arg(n));

  return chk;
}

void CctwChunkedData::writeChunk(int n)
{
  CctwDataChunk *chk = chunk(n);

  if (chk) {
    normalizeChunk(n);

    if (openOutputFile()) {
      printMessage(tr("About to write chunk %1").arg(n));

      QcepMutexLocker lock(__FILE__, __LINE__, &m_FileAccessMutex);

      if (m_FileId >= 0) {

        hid_t memspace_id = -1;
        hsize_t offset[3], count[3], stride[3], block[3];

        CctwIntVector3D st = chk -> chunkStart();
        CctwIntVector3D sz = chk -> chunkSize();

        printMessage(tr("Writing chunk %1 [%2..%3, %4..%5, %6..%7]")
                     .arg(n)
                     .arg(st.x())
                     .arg(st.x()+sz.x()-1)
                     .arg(st.y())
                     .arg(st.y()+sz.y()-1)
                     .arg(st.z())
                     .arg(st.z()+sz.z()-1)
                     );

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

        double *chunkData = chk->dataPointer();

        if (chunkData == NULL) {
          printMessage(tr("Anomaly writing chunk %1, data == NULL").arg(n));
        } else {
          memspace_id   = H5Screate_simple(3, count, NULL);
          herr_t selerr = H5Sselect_hyperslab(m_DataspaceId, H5S_SELECT_SET, offset, stride, count, block);
          herr_t wrterr = H5Dwrite(m_DatasetId, H5T_NATIVE_DOUBLE, memspace_id, m_DataspaceId, H5P_DEFAULT, chunkData);

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

    releaseChunk(n);
  }
}

void CctwChunkedData::releaseChunk(int n)
{
  printMessage(tr("CctwChunkedData::releaseChunk(%1)").arg(n));

  CctwDataChunk *chk = chunk(n);

  if (chk) {
    chk->deallocateData();
    chk->deallocateWeights();
  }
}

void CctwChunkedData::beginTransform(bool isInput)
{
  m_IsInput = isInput;

  printMessage("CctwChunkedData::beginTransform()");

  if (m_IsInput) {
    openInputFile();
  } else {
    openOutputFile();
  }
}

void CctwChunkedData::endTransform()
{
  printMessage("CctwChunkedData::endTransform()");

  if (m_IsInput) {
    closeInputFile();
  } else {
    closeOutputFile();
  }
}
