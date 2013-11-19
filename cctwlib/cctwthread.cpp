#include "cctwthread.h"

CctwThread::CctwThread(QObject *parent) :
  QThread(parent)
{
}

void CctwThread::sleep(unsigned long sec)
{
  QThread::sleep(sec);
}

void CctwThread::msleep(unsigned long msec)
{
  QThread::msleep(msec);
}

void CctwThread::usleep(unsigned long usec)
{
  QThread::usleep(usec);
}
