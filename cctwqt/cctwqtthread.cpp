#include "cctwqtthread.h"

CctwqtThread::CctwqtThread(QObject *parent) :
  QThread(parent)
{
}

void CctwqtThread::sleep(unsigned long sec)
{
  QThread::sleep(sec);
}

void CctwqtThread::msleep(unsigned long msec)
{
  QThread::msleep(msec);
}

void CctwqtThread::usleep(unsigned long usec)
{
  QThread::usleep(usec);
}
