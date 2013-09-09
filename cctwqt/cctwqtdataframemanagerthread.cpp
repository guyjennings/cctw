#include "cctwqtdataframemanagerthread.h"
#include "cctwqtdataframemanager.h"

CctwqtDataFrameManagerThread::CctwqtDataFrameManagerThread(QObject *parent) :
  QThread(parent)
{
}

void CctwqtDataFrameManagerThread::run()
{
  m_Manager.fetchAndStoreOrdered(new CctwqtDataFrameManager(parent()));

  exec();
}

CctwqtDataFrameManager * CctwqtDataFrameManagerThread::manager() const
{
#if QT_VERSION >= 0x050000
  while (isRunning() && m_Manager.load() == NULL) {
    QThread::msleep(100);
  }

  return m_Manager.load();
#else
  while (isRunning() && m_Manager == NULL) {
    QThread::msleep(100);
  }

  return m_Manager;
#endif
}
