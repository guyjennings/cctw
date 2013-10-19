#include "cctwqtoutputdataframemanager.h"
#include "cctwqtdataframe.h"
#include "cctwchunkindex.h"
#include "cctwqtdatachunk.h"

CctwqtOutputDataFrameManager::CctwqtOutputDataFrameManager(QcepSettingsSaverPtr saver, QObject *parent) :
  CctwqtDataFrameManager(parent),
  m_ObjectNamer(this, "outputDataManager"),
  m_Saver(saver),
  m_FileId(-1),
  m_DatasetId(-1),
  m_DataspaceId(-1),
  m_FilePath(m_Saver, this, "filePath", "", "Output file path"),
  m_OutputFormat(m_Saver, this, "outputFormat", 0, "Output file format")
{
}

int CctwqtOutputDataFrameManager::loadChunk(int nx, int ny, int nz)
{
//  if (QThread::currentThread() != thread()) {
//    QMetaObject::invokeMethod(this, "loadChunk", Qt::BlockingQueuedConnection, Q_ARG(int, nx), Q_ARG(int, ny), Q_ARG(int, nz));
//  } else {
//    if (m_Frames[nz] == NULL) {
////      CctwqtOutputDataFrameManagerThread::msleep(1000);

//      m_Frames[nz] = new CctwqtDataFrame(m_Data->dimensions().x(),
//                                         m_Data->dimensions().y(),
//                                         this);
//    }

//    m_Frames[nz] -> reference();
//  }

  return m_Data->chunkNumberFromIndex(CctwIntVector3D(nx, ny, nz));
}

void CctwqtOutputDataFrameManager::releaseChunk(int chunkId)
{
  int nz = m_Data->chunkIndexFromNumber(chunkId).z();

  if (m_Frames[nz]) {
    m_Frames[nz] -> dereference();
  }
}

void CctwqtOutputDataFrameManager::openOutputFile()
{
  closeOutputFile();

  printMessage("Opening output file");

  m_FileId = H5Fcreate(qPrintable(get_FilePath()), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  if (m_FileId < 0) {
    printMessage("File creation failed");
  } else {
    hsize_t dims[3];

    dims[0] = m_Data->dimensions().x();
    dims[1] = m_Data->dimensions().y();
    dims[2] = m_Data->dimensions().z();

    m_DataspaceId = H5Screate_simple(3, dims, NULL);

    m_DatasetId   = H5Dcreate(m_FileId, "data", H5T_NATIVE_FLOAT, m_DataspaceId, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  }
}

void CctwqtOutputDataFrameManager::closeOutputFile()
{
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

void CctwqtOutputDataFrameManager::writeChunk(CctwqtDataChunk *chunk)
{
  QMutexLocker lock(&m_WriteLock);

  if (m_FileId < 0) {
    openOutputFile();
  }

  if (m_DatasetId >= 0 && m_DataspaceId >= 0) {
    hid_t memspace_id;
    hsize_t offset[3], count[3], stride[3], block[3];

    count[0] = m_Data->chunkSize().x();
    count[1] = m_Data->chunkSize().y();
    count[2] = m_Data->chunkSize().z();

    stride[0] = 1;
    stride[1] = 1;
    stride[2] = 1;

    block[0] = 1;
    block[1] = 1;
    block[2] = 1;

    CctwIntVector3D chunkOffset = chunk->index()*m_Data->chunkSize();

    offset[0] = chunkOffset.x();
    offset[1] = chunkOffset.y();
    offset[2] = chunkOffset.z();

    memspace_id = H5Screate_simple(3, count, NULL);

    herr_t selerr = H5Sselect_hyperslab(m_DataspaceId, H5S_SELECT_SET, offset, stride, count, block);

    herr_t wrterr = H5Dwrite(m_DatasetId, H5T_NATIVE_DOUBLE, memspace_id, m_DataspaceId, H5P_DEFAULT, chunk->dataPointer());

    if (selerr || wrterr) {
      printMessage(tr("Error writing chunk [%1,%2,%3] -> selerr = %4, wrterr = %5")
                   .arg(chunk->index().x()).arg(chunk->index().y()).arg(chunk->index().z())
                   .arg(selerr).arg(wrterr));
    }

    H5Sclose(memspace_id);
  } else {
    printMessage("Not written");
  }
}

void CctwqtOutputDataFrameManager::beginTransform()
{
  QMutexLocker lock(&m_WriteLock);

  if (m_FileId < 0) {
    openOutputFile();
  }
}

void CctwqtOutputDataFrameManager::endTransform()
{
  QMutexLocker lock(&m_WriteLock);

  if (m_FileId >= 0) {
    closeOutputFile();
  }
}
