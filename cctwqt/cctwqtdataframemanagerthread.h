#ifndef CCTWQTDATAFRAMEMANAGERTHREAD_H
#define CCTWQTDATAFRAMEMANAGERTHREAD_H

#include <QThread>
#include <QAtomicPointer>

class CctwqtDataFrameManager;

class CctwqtDataFrameManagerThread : public QThread
{
  Q_OBJECT
public:
  explicit CctwqtDataFrameManagerThread(QObject *parent = 0);
  
signals:
  
public slots:
  
public:
  CctwqtDataFrameManager *manager() const;

  static void sleep(unsigned long n) { QThread::sleep(n); }
  static void msleep(unsigned long n) { QThread::msleep(n); }
  static void usleep(unsigned long n) { QThread::usleep(n); }

private:
  void run();

private:
  QAtomicPointer<CctwqtDataFrameManager> m_Manager;
};

#endif // CCTWQTDATAFRAMEMANAGERTHREAD_H
