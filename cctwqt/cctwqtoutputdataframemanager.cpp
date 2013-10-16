#include "cctwqtoutputdataframemanager.h"
#include "cctwqtdataframe.h"
#include "cctwchunkindex.h"

CctwqtOutputDataFrameManager::CctwqtOutputDataFrameManager(QcepSettingsSaverPtr saver, QObject *parent) :
  CctwqtDataFrameManager(parent),
  m_ObjectNamer(this, "outputDataManager"),
  m_Saver(saver),
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

void CctwqtOutputDataFrameManager::writeChunk(CctwqtDataChunk *chunk)
{
  printMessage("Want to write chunk");
}
