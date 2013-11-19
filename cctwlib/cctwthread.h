#ifndef CCTWTHREAD_H
#define CCTWTHREAD_H

#include <QThread>

class CctwThread : public QThread
{
  Q_OBJECT
public:
  explicit CctwThread(QObject *parent = 0);
  
signals:
  
public slots:
  
public:
  static void sleep(unsigned long sec);
  static void msleep(unsigned long msec);
  static void usleep(unsigned long usec);
};

#endif // CCTWQTTHREAD_H
