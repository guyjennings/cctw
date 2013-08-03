#ifndef CCTWQTDATAFRAMEMANAGERTHREAD_H
#define CCTWQTDATAFRAMEMANAGERTHREAD_H

#include <QThread>
#include <QAtomicPointer>
#include "cctwqtdataframemanager.h"

class CctwqtDataFrameManagerThread : public QThread
{
  Q_OBJECT
public:
  explicit CctwqtDataFrameManagerThread(QObject *parent = 0);
  
signals:
  
public slots:
  
public:
  CctwqtDataFrameManager *manager() const;

private:
  void run();

private:
  QAtomicPointer<CctwqtDataFrameManager> m_Manager;
};

#endif // CCTWQTDATAFRAMEMANAGERTHREAD_H
