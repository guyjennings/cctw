#include "cctwqtdataframemanager.h"
#include <QThread>
#include "cctwqtdataframemanagerthread.h"
#include "cctwqtdataframe.h"
#include "cctwchunkindex.h"

CctwqtDataFrameManager::CctwqtDataFrameManager(QObject *parent) :
  CctwqtObject(),
  m_Parent(parent),
  m_Data(NULL)
{
}

void CctwqtDataFrameManager::setData(CctwqtChunkedData *data)
{
  m_Data = data;

  int n = m_Data->dimensions().z();

  m_Frames.resize(n);

  for(int i=0; i<n; i++) {
    m_Frames[i] = NULL;
  }
}

void CctwqtDataFrameManager::printMessage(QString msg)
{
  if (m_Parent) {
    QMetaObject::invokeMethod(m_Parent, "printMessage", Q_ARG(QString, msg));
  }
}

int CctwqtDataFrameManager::loadChunk(int nx, int ny, int nz)
{
  if (QThread::currentThread() != thread()) {
    QMetaObject::invokeMethod(this, "loadChunk", Qt::BlockingQueuedConnection, Q_ARG(int, nx), Q_ARG(int, ny), Q_ARG(int, nz));
  } else {
    if (m_Frames[nz] == NULL) {
      CctwqtDataFrameManagerThread::msleep(1000);

      m_Frames[nz] = new CctwqtDataFrame(m_Data->dimensions().x(),
                                         m_Data->dimensions().y(),
                                         this);
    }

    m_Frames[nz] -> reference();
  }

  return m_Data->chunkNumberFromIndex(CctwIntVector3D(nx, ny, nz));
}

void CctwqtDataFrameManager::releaseChunk(int chunkId)
{
  int nz = m_Data->chunkIndexFromNumber(chunkId).z();

  if (m_Frames[nz]) {
    m_Frames[nz] -> dereference();
  }
}
