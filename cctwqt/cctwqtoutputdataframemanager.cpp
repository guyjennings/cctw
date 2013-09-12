#include "cctwqtoutputdataframemanager.h"
#include "cctwqtdataframe.h"
#include "cctwchunkindex.h"

CctwqtOutputDataFrameManager::CctwqtOutputDataFrameManager(QObject *parent) :
  CctwqtDataFrameManager(parent)
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
