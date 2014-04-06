#include "cctwimporter.h"
#include "cctwapplication.h"
#include <QDir>
#include <QtConcurrentRun>
#include "qcepmutexlocker.h"
#include "cctwinputdatah5.h"

CctwImporter::CctwImporter(CctwApplication *application, QString name, QObject *parent) :
  CctwObject(name, parent),
  m_Application(application),
  m_BacklogSemaphore(8),
  m_CompletionSemaphore(0),
  m_FileId(-1),
  m_DatasetId(-1),
  m_DataspaceId(-1),
  m_InputBuffer(NULL),
  m_InputBufferCount(0),
  m_DarkImage(new QcepImageData<double>(QcepSettingsSaverWPtr(),0,0)),
  m_DataFormat(m_Application->saver(), this, "dataFormat", 0, "Imported data format (0=auto)"),
  m_DarkImagePath(m_Application->saver(), this, "darkImagePath", "", "Dark image path"),
  m_ImagePaths(m_Application->saver(), this, "imagePaths", QcepStringList(), "Imported image paths"),
  m_ImageDirectory(m_Application->saver(), this, "imageDirectory", "", "Image directory"),
  m_ImagePattern(m_Application->saver(), this, "imagePattern", "", "Image pattern"),
  m_OutputPath(m_Application->saver(), this, "outputPath", "./output.h5", "Destination for imported data"),
  m_OutputDataset(m_Application->saver(), this, "outputDataset", "/data", "Destination Dataset for imported data"),
  m_ChunkSize(m_Application->saver(), this, "chunkSize", CctwIntVector3D(32,32,32), "output chunk size"),
  m_Compression(m_Application->saver(), this, "compression", 0, "output compression type"),
  m_XDimension(QcepSettingsSaverWPtr(), this, "xDimension", 0, "X Dimension of output"),
  m_YDimension(QcepSettingsSaverWPtr(), this, "yDimension", 0, "Y Dimension of output"),
  m_ZDimension(QcepSettingsSaverWPtr(), this, "zDimension", 0, "Z Dimension of output"),
  m_InputDataBuffering(m_Application->saver(), this, "inputDataBuffering", 0, "Input Data Buffering"),
  m_CheckRigorously(m_Application->saver(), this, "checkRigorously", false, "Perform rigorous check (every element) of imported data"),
  m_CheckApproximately(m_Application->saver(), this, "checkApproximately", true, "Perform approximate check (about 30 seconds) of imported data")
{
}

CctwImporter::~CctwImporter()
{
  deleteDataBuffer();
}

void CctwImporter::clearInputFiles()
{
  prop_ImagePaths()->clear();
}

void CctwImporter::changeDirectory(QString path)
{
  set_ImageDirectory(path);
}

void CctwImporter::appendInputFile(QString path)
{
  prop_ImagePaths()->appendValue(path);
}

void CctwImporter::appendMatchingFiles(QString pattern)
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

void CctwImporter::loadDarkImage()
{
  QString dkPath = get_DarkImagePath();

  if (dkPath.length() > 0) {
    if (m_DarkImage->readImage(get_DarkImagePath())) {
      printMessage(tr("Reading dark image from %1").arg(dkPath));
      m_DarkImage->loadMetaData();
    } else {
      printMessage(tr("Failed to read dark image from %1").arg(dkPath));
    }
  } else {
    printMessage(tr("No dark image specified"));
  }
}

void CctwImporter::importData()
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

  loadDarkImage();

  printMessage(tr("Importing %1 frames of data").arg(n));

  set_ZDimension(n);

  if (get_InputDataBuffering() <= 1) {
    for (int i=0; i<n; i++) {
      if (m_Application) {
        m_Application->addWorkOutstanding(1);
      }

      m_BacklogSemaphore.acquire(1);
      //      printMessage("acquired backlog semaphore");
      QtConcurrent::run(this, &CctwImporter::importDataFrame,
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

    deleteDataBuffer();
  }

  // waitTillFinished();

  m_CompletionSemaphore.acquire(n);
  closeOutputFile();

  if (m_Application->get_Halting()) {
    printMessage(tr("Import Cancelled after %1 sec").arg(startAt.elapsed()/1000.0));
  } else {
    printMessage(tr("Import Completed after %1 sec").arg(startAt.elapsed()/1000.0));
  }
}

bool CctwImporter::createOutputFile()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_OutputMutex);

  if (m_FileId >= 0) {
    return true;
  }

  printMessage("About to open output file");

  int res = true;

  QString fileName = get_OutputPath();
  QFileInfo f(fileName);

  hid_t fileId = -1;
  hid_t gplist = -1;
  hid_t dsetId = -1;
  hid_t dspcId = -1;
  hid_t plist  = -1;

  if (!f.exists()) {
    // file does not exist...
    fileId = H5Fcreate(qPrintable(fileName), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    if (fileId < 0) {
      printMessage(tr("File %1 could not be created").arg(fileName));
      res = false;
    }
  } else if (H5Fis_hdf5(qPrintable(get_OutputPath())) <= 0) {
    printMessage(tr("File %1 exists but is not an hdf file").arg(fileName));
    res = false;
  } else {
    fileId = H5Fopen(qPrintable(fileName), H5F_ACC_RDWR, H5P_DEFAULT);

    if (fileId < 0) {
      printMessage(tr("File %1 could not be opened").arg(fileName));
      res = false;
    }
  }

  if (fileId > 0) {
    // File exists and is hdf5 - does the dataset exist

    dsetId = H5Dopen(fileId, qPrintable(get_OutputDataset()), H5P_DEFAULT);

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
            if (dims[0] != (hsize_t) get_ZDimension() ||
                dims[1] != (hsize_t) get_YDimension() ||
                dims[2] != (hsize_t) get_XDimension()) {
              printMessage("Dataspace dimensions do not match imported data");
              res = false;
            }
          }
        }
      }
    } else {
      hsize_t dims[3];
      dims[0] = get_ZDimension();
      dims[1] = get_YDimension();
      dims[2] = get_XDimension();

      dspcId = H5Screate_simple(3, dims, NULL);

      CctwIntVector3D cksz = get_ChunkSize();
      int cmprs = get_Compression();

      plist = H5Pcreate(H5P_DATASET_CREATE);

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

      gplist = H5Pcreate(H5P_LINK_CREATE);

      if (H5Pset_create_intermediate_group(gplist, 1) < 0) {
        printMessage(tr("Failed to set create intermediate groups"));
        res = false;
      } else {
        dsetId = H5Dcreate(fileId,
                           qPrintable(get_OutputDataset()),
                           H5T_NATIVE_FLOAT,
                           dspcId,
                           gplist,
                           plist,
                           H5P_DEFAULT);
      }
    }

    if (dsetId < 0) {
      printMessage(tr("Could not create or find dataset"));
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
      printMessage(tr("Anomaly in CctwImporter::createOutputFile fileId=%1, dsetId=%2, dspcId=%3")
                   .arg(fileId).arg(dsetId).arg(dspcId));
    }

    m_FileId      = fileId;
    m_DatasetId   = dsetId;
    m_DataspaceId = dspcId;
  }

  return res;
}

void CctwImporter::closeOutputFile()
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

void CctwImporter::writeOutputFrame(int num, QcepImageData<double> *img)
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

void CctwImporter::importDataFrame(int num, QString path)
{
  m_BacklogSemaphore.release(1);

  if (m_Application && !m_Application->get_Halting()) {
    if (path.length() > 0) {

      QcepImageData<double> m(QcepSettingsSaverWPtr(), 0, 0);

      if (m.readImage(path)) {
        m.loadMetaData();

        printMessage(tr("Imported frame %1 from %2").arg(num).arg(path));

        m.subtractDark(m_DarkImage);

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

void CctwImporter::initializeDataBuffer()
{
  deleteDataBuffer();
}

void CctwImporter::allocateDataBuffer(hsize_t dimx, hsize_t dimy, hsize_t dimz)
{
  m_InputBufferStride = dimx*dimy;
  m_InputBufferSize   = dimx*dimy*dimz;

  m_InputBuffer       = new float[m_InputBufferSize];
  m_InputBufferStart  = 0;
  m_InputBufferCount  = 0;
}

void CctwImporter::deleteDataBuffer()
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

void CctwImporter::readDataFrameToBuffer(int i, int nb, QString path)
{
  if (m_Application && !m_Application->get_Halting()) {
    if (path.length() > 0) {

      QcepImageData<double> m(QcepSettingsSaverWPtr(), 0, 0);

      if (m.readImage(path)) {
        m.loadMetaData();

        printMessage(tr("Imported frame %1 from %2").arg(i).arg(path));

        m.subtractDark(m_DarkImage);

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

void CctwImporter::outputDataFromBuffer(int i)
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

void CctwImporter::checkImportedData()
{
  printMessage("Checking imported data...");

  loadDarkImage();

  if (get_CheckRigorously()) {
    checkImportedDataRigorously();
  }

  if (get_CheckApproximately()) {
    checkImportedDataApproximately();
  }
}

void CctwImporter::checkImportedDataRigorously()
{
  CctwInputDataH5 data(get_OutputPath(), get_OutputDataset(), "h5import", this);

  printMessage("Checking imported data rigorously...");
}

static int randomIndex(int n)
{
  return qrand()%n;
}

void CctwImporter::checkImportedDataApproximately()
{
  CctwInputDataH5 data(get_OutputPath(), get_OutputDataset(), "h5import", this);

  printMessage("Checking imported data approximately...");

  QTime startAt;
  startAt.start();

  QStringList paths = get_ImagePaths();
  QDir        inp(get_ImageDirectory());
  int n=0;

  for (int nz = 0; nz<5; nz++) {
    QcepImageData<double> m(QcepSettingsSaverWPtr(), 0, 0);

    if (m.readImage(inp.filePath(paths[nz]))) {
      m.loadMetaData();
      m.subtractDark(m_DarkImage);

      for  (int nx = 0; nx<5; nx++) {
        for (int ny = 0; ny<5; ny++) {
          double fromTiff = m.getImageData(nx, ny);
          double fromHDF  = data.readData(nx, ny, nz);

          printLine(tr("[%1,%2,%3] TF: %4, H5 %5")
                    .arg(nx).arg(ny).arg(nz).arg(fromTiff).arg(fromHDF));
        }
      }
    }
  }

  while (startAt.elapsed() < 60*1000 && !m_Application->get_Halting() && n < 100) {
    int nz = randomIndex(data.dimensions().z());

    QcepImageData<double> m(QcepSettingsSaverWPtr(), 0, 0);

    if (m.readImage(inp.filePath(paths[nz]))) {
      m.loadMetaData();

      m.subtractDark(m_DarkImage);

      for (int i=0; i<100 && !m_Application->get_Halting() && n < 100; i++) {
        int nx = randomIndex(data.dimensions().x());
        int ny = randomIndex(data.dimensions().y());

        double fromTiff = m.getImageData(nx, ny);
        double fromHDF  = data.readData(nx, ny, nz);

        if (fromTiff != fromHDF) {
          n++;
          printMessage(tr("Mismatch at [%1,%2,%3]: %4 <=> %5")
                       .arg(nx).arg(ny).arg(nz)
                       .arg(fromTiff).arg(fromHDF));
        }
      }
    } else {
      printMessage(tr("Failed to load image %1 [%2]")
                   .arg(nz).arg(paths[nz]));
    }
  }

  if (m_Application->get_Halting()) {
    printMessage("Testing interrupted");
  }
}
