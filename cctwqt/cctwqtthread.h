#ifndef CCTWQTTHREAD_H
#define CCTWQTTHREAD_H

#include <QThread>

class CctwqtThread : public QThread
{
  Q_OBJECT
public:
  explicit CctwqtThread(QObject *parent = 0);
  
signals:
  
public slots:
  
public:
  static void sleep(unsigned long sec);
  static void msleep(unsigned long msec);
  static void usleep(unsigned long usec);
};

#endif // CCTWQTTHREAD_H
