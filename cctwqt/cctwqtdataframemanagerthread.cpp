#include "cctwqtdataframemanagerthread.h"
#include "cctwqtdataframemanager.h"

CctwqtDataFrameManagerThread::CctwqtDataFrameManagerThread(QObject *parent) :
  QThread(parent)
{
}

void CctwqtDataFrameManagerThread::run()
{
  m_Manager.fetchAndStoreOrdered(new CctwqtDataFrameManager());

  exec();
}

CctwqtDataFrameManager * CctwqtDataFrameManagerThread::manager() const
{
  while (isRunning() && m_Manager == NULL) {
    QThread::msleep(100);
  }

  return m_Manager;
}
