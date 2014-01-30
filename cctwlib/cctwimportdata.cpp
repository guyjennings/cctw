#include "cctwimportdata.h"
#include "cctwapplication.h"
#include <QDir>
#include <QtConcurrentRun>
#include "qcepmutexlocker.h"

CctwImportData::CctwImportData(CctwApplication *application, QObject *parent) :
  CctwObject(parent),
  m_Application(application),
  m_BacklogSemaphore(8),
  m_CompletionSemaphore(0),
  m_FileId(-1),
  m_DatasetId(-1),
  m_DataspaceId(-1),
  m_InputBuffer(NULL),
  m_InputBufferCount(0),
  m_DataFormat(m_Application->saver(), this, "dataFormat", 0, "Imported data format (0=auto)"),
  m_DarkImagePath(m_Application->saver(), this, "darkImagePath", "", "Dark image path"),
  m_ImagePaths(m_Application->saver(), this, "imagePaths", QcepStringList(), "Imported image paths"),
  m_ImageDirectory(m_Application->saver(), this, "imageDirectory", "", "Image directory"),
  m_ImagePattern(m_Application->saver(), this, "imagePattern", "", "Image pattern"),
  m_OutputPath(m_Application->saver(), this, "outputPath", "", "Destination for imported data"),
  m_ChunkSize(m_Application->saver(), this, "chunkSize", CctwIntVector3D(128,128,128), "output chunk size"),
  m_Compression(m_Application->saver(), this, "compression", 0, "output compression type"),
  m_XDimension(QcepSettingsSaverWPtr(), this, "xDimension", 0, "X Dimension of output"),
  m_YDimension(QcepSettingsSaverWPtr(), this, "yDimension", 0, "Y Dimension of output"),
  m_ZDimension(QcepSettingsSaverWPtr(), this, "zDimension", 0, "Z Dimension of output"),
  m_InputDataBuffering(m_Application->saver(), this, "inputDataBuffering", 0, "Input Data Buffering")
{
}

CctwImportData::~CctwImportData()
{
  deleteDataBuffer();
}

void CctwImportData::clearInputFiles()
{
  prop_ImagePaths()->clear();
}

void CctwImportData::changeDirectory(QString path)
{
  set_ImageDirectory(path);
}

void CctwImportData::appendInputFile(QString path)
{
  prop_ImagePaths()->appendValue(path);
}

void CctwImportData::appendMatchingFiles(QString pattern)
{
  set_ImagePattern(pattern);

  QDir d(get_ImageDirectory());
  d.setFilter(QDir::Files);

  QStringList patts(pattern);
  d.setNameFilters(patts);

  QStringList newFiles = d.entryList();

  QStringList l = get_ImagePaths();

  l.append(newFiles);

  set_ImagePaths(l);
}

void CctwImportData::importData()
{
  QTime startAt;
  startAt.start();

  QStringList paths = get_ImagePaths();
  int n = paths.count();
  QVector< QFuture<void> > results(n);
  QDir inp(get_ImageDirectory());

  if (m_Application) {
    m_Application->waitCompleted();
    m_Application->set_Progress(0);
    m_Application->set_Halting(false);
    m_Application->set_ProgressLimit(n);
  }

  printMessage(tr("Importing %1 frames of data").arg(n));

  set_ZDimension(n);

  if (get_InputDataBuffering() <= 1) {
    for (int i=0; i<n; i++) {
      if (m_Application) {
        m_Application->addWorkOutstanding(1);
      }

      m_BacklogSemaphore.acquire(1);
      //      printMessage("acquired backlog semaphore");
      QtConcurrent::run(this, &CctwImportData::importDataFrame,
                        i, inp.filePath(paths[i]));
    }
  } else {
    int nb = get_InputDataBuffering();

    initializeDataBuffer();

    for (int i=0; i<n; i++) {
      if (m_Application) {
        m_Application->addWorkOutstanding(1);
      }

      readDataFrameToBuffer(i, nb, inp.filePath(paths[i]));
    }

    outputDataFromBuffer(0);
  }

  // waitTillFinished();

  m_CompletionSemaphore.acquire(n);
  closeOutputFile();
  deleteDataBuffer();
  m_CompletionSemaphore.release(n);

  if (m_Application->get_Halting()) {
    printMessage(tr("Import Cancelled after %1 sec").arg(startAt.elapsed()/1000.0));
  } else {
    printMessage(tr("Import Completed after %1 sec").arg(startAt.elapsed()/1000.0));
  }
}

bool CctwImportData::createOutputFile()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_OutputMutex);

  if (m_Application && (m_Application->get_Halting()==false)) {
    if (m_FileId < 0) {
      QString fileName = get_OutputPath();
      QFileInfo f(fileName);

      if (!f.exists()) {
        // file does not exist...
        m_FileId = H5Fcreate(qPrintable(fileName), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

        if (m_FileId < 0) {
          printMessage(tr("File %1 could not be created").arg(fileName));
          return false;
        }
      } else if (H5Fis_hdf5(qPrintable(get_OutputPath())) <= 0) {
        printMessage(tr("File %1 exists but is not an hdf file").arg(fileName));
        return false;
      } else {
        m_FileId = H5Fopen(qPrintable(fileName), H5F_ACC_RDWR, H5P_DEFAULT);

        if (m_FileId < 0) {
          printMessage(tr("File %1 could not be opened").arg(fileName));
          return false;
        }
      }

      // File exists and is hdf5 - does the dataset exist

      m_DatasetId = H5Dopen1(m_FileId, "data");

      if (m_DatasetId >= 0) {
        m_DataspaceId = H5Dget_space(m_DatasetId);

        if (m_DataspaceId < 0) {
          printMessage("Couldnt get dataspace of existing dataset");
          return false;
        }

        hsize_t dims[3];
        int ndims = H5Sget_simple_extent_ndims(m_DataspaceId);

        if (ndims != 3) {
          printMessage("Dataspace is not 3-dimensional");
          return false;
        }

        int ndims2 = H5Sget_simple_extent_dims(m_DataspaceId, dims, NULL);

        if (ndims2 != 3) {
          printMessage("Couldnt get dataspace dimensions");
          return false;
        }

        if (dims[0] != get_ZDimension() ||
            dims[1] != get_YDimension() ||
            dims[2] != get_XDimension()) {
          printMessage("Dataspace dimensions do not match imported data");
          return false;
        }
      } else {
        hsize_t dims[3];
        dims[0] = get_ZDimension();
        dims[1] = get_YDimension();
        dims[2] = get_XDimension();

        m_DataspaceId = H5Screate_simple(3, dims, NULL);

        CctwIntVector3D cksz = get_ChunkSize();
        int cmprs = get_Compression();

        hid_t plist = H5Pcreate(H5P_DATASET_CREATE);

        if (cksz.volume() > 0) {
          hsize_t c[3];

          c[0] = cksz.z();
          c[1] = cksz.y();
          c[2] = cksz.x();

          if (H5Pset_chunk(plist, 3, c) < 0) {
            printMessage("Failed to set chunk size");
            return false;
          }

          if (cmprs) {
            if (H5Pset_deflate(plist, cmprs) < 0) {
              printMessage("Set deflate failed");
              return false;
            }
          }
        }

        m_DatasetId = H5Dcreate(m_FileId, "data", H5T_NATIVE_FLOAT, m_DataspaceId, H5P_DEFAULT, plist, H5P_DEFAULT);

        H5Pclose(plist);
      }

      if (m_DatasetId < 0) {
        printMessage(tr("Could not create or find dataset"));
        return false;
      }

      printMessage("Opened output file");
    }

    return true;
  } else {
    return false;
  }
}

void CctwImportData::closeOutputFile()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_OutputMutex);

  if (m_FileId >= 0) {
    if (m_DataspaceId >= 0) {
      H5Sclose(m_DataspaceId);
      m_DataspaceId = -1;
    }

    if (m_DatasetId >= 0) {
      H5Dclose(m_DatasetId);
      m_DatasetId = -1;
    }

    H5Fclose(m_FileId);

    printMessage("Closed output file");

    m_FileId = -1;
  }
}

void CctwImportData::writeOutputFrame(int num, QcepImageData<double> *img)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_OutputMutex);

  printMessage(tr("Writing output frame %1").arg(num));

  if (m_FileId >= 0) {
    hid_t memspace_id;
    hsize_t offset[3], count[3], stride[3], block[3];

    count[0] = 1;
    count[1] = get_YDimension();
    count[2] = get_XDimension();

    stride[0] = 1;
    stride[1] = 1;
    stride[2] = 1;

    block[0] = 1;
    block[1] = 1;
    block[2] = 1;

    offset[0] = num;
    offset[1] = 0;
    offset[2] = 0;

    memspace_id = H5Screate_simple(3, count, NULL);

    herr_t selerr = H5Sselect_hyperslab(m_DataspaceId, H5S_SELECT_SET, offset, stride, count, block);
    herr_t wrterr = H5Dwrite(m_DatasetId, H5T_NATIVE_DOUBLE, memspace_id, m_DataspaceId, H5P_DEFAULT, img->data());

    if (selerr || wrterr) {
      printMessage(tr("Error writing frame %1, selerr = %2, wrterr = %3").arg(num).arg(selerr).arg(wrterr));
    }
  }
}

void CctwImportData::importDataFrame(int num, QString path)
{
  m_BacklogSemaphore.release(1);

  if (m_Application && !m_Application->get_Halting()) {
    if (path.length() > 0) {

      QcepImageData<double> m(QcepSettingsSaverWPtr(), 0, 0);

      if (m.readImage(path)) {
        m.loadMetaData();

        printMessage(tr("Imported frame %1 from %2").arg(num).arg(path));

        set_XDimension(m.get_Width());
        set_YDimension(m.get_Height());

        if (createOutputFile()) {
          writeOutputFrame(num, &m);
        } else {
          m_Application->set_Halting(true);
        }
      }
    }
  }

  m_CompletionSemaphore.release(1);

  if (m_Application) {
    m_Application->prop_Progress()->incValue(1);
    m_Application->workCompleted(1);
  }
}

void CctwImportData::initializeDataBuffer()
{
  deleteDataBuffer();
}

void CctwImportData::allocateDataBuffer(hsize_t dimx, hsize_t dimy, hsize_t dimz)
{
  m_InputBufferStride = dimx*dimy;
  m_InputBufferSize   = dimx*dimy*dimz;

  m_InputBuffer       = new float[m_InputBufferSize];
  m_InputBufferStart  = 0;
  m_InputBufferCount  = 0;
}

void CctwImportData::deleteDataBuffer()
{
  if (m_InputBuffer) {
    delete [] m_InputBuffer;

    m_InputBuffer       = 0;
    m_InputBufferStart  = 0;
    m_InputBufferCount  = 0;
    m_InputBufferSize   = 0;
    m_InputBufferStride = 0;
  }
}

void CctwImportData::readDataFrameToBuffer(int i, int nb, QString path)
{
  if (m_Application && !m_Application->get_Halting()) {
    if (path.length() > 0) {

      QcepImageData<double> m(QcepSettingsSaverWPtr(), 0, 0);

      if (m.readImage(path)) {
        m.loadMetaData();

        printMessage(tr("Imported frame %1 from %2").arg(i).arg(path));

        if (m_InputBuffer == NULL) {
          set_XDimension(m.get_Width());
          set_YDimension(m.get_Height());

          allocateDataBuffer(get_XDimension(), get_YDimension(), nb);

          if (!createOutputFile()) {
            m_Application->set_Halting(true);
          }
        }

        hsize_t ibuf = i % nb;

        if (ibuf == 0) {
          outputDataFromBuffer(i);
        }

        double *frame = m.data();
        float *dest  = m_InputBuffer + ibuf*m_InputBufferStride;

        for (hsize_t i=0; i<m_InputBufferStride; i++) {
          *dest++ = *frame++;
        }

        m_InputBufferCount = ibuf+1;
      }
    }
  }

  m_CompletionSemaphore.release(1);

  if (m_Application) {
    m_Application->prop_Progress()->incValue(1);
    m_Application->workCompleted(1);
  }
}

void CctwImportData::outputDataFromBuffer(int i)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_OutputMutex);

  printMessage(tr("Writing output from frames %1 to %2")
               .arg(m_InputBufferStart)
               .arg(m_InputBufferStart+m_InputBufferCount-1));

  if (m_FileId >= 0) {
    hid_t memspace_id;
    hsize_t offset[3], count[3], stride[3], block[3];

    count[0] = m_InputBufferCount;
    count[1] = get_YDimension();
    count[2] = get_XDimension();

    stride[0] = 1;
    stride[1] = 1;
    stride[2] = 1;

    block[0] = 1;
    block[1] = 1;
    block[2] = 1;

    offset[0] = m_InputBufferStart;
    offset[1] = 0;
    offset[2] = 0;

    memspace_id = H5Screate_simple(3, count, NULL);

    herr_t selerr = H5Sselect_hyperslab(m_DataspaceId, H5S_SELECT_SET, offset, stride, count, block);
    herr_t wrterr = H5Dwrite(m_DatasetId, H5T_NATIVE_FLOAT, memspace_id, m_DataspaceId, H5P_DEFAULT, m_InputBuffer);

    if (selerr || wrterr) {
      printMessage(tr("Error writing frames %1 to %2, selerr = %3, wrterr = %4")
                   .arg(m_InputBufferStart)
                   .arg(m_InputBufferStart+m_InputBufferCount-1)
                   .arg(selerr)
                   .arg(wrterr));
    }
  }

  m_InputBufferStart = i;
  m_InputBufferCount = 0;
}
