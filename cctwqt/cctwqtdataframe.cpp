#include "cctwqtdataframe.h"
#include "qcepmutexlocker.h"

CctwqtDataFrame::CctwqtDataFrame(int dimx, int dimy, QObject *parent) :
  CctwObject(parent),
  m_Counter(0),
  m_DimX(dimx),
  m_DimY(dimy),
  m_Data(dimx*dimy),
  m_LastReferenced(QDateTime::currentDateTime())
{
}

CctwqtDataFrame::~CctwqtDataFrame()
{
}

int CctwqtDataFrame::reference()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_LastReferenced = QDateTime::currentDateTime();

  return m_Counter.fetchAndAddOrdered(1) + 1;
}

int CctwqtDataFrame::referenceCount()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Counter.fetchAndAddOrdered(0);
}

int CctwqtDataFrame::dereference()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Counter.fetchAndAddOrdered(-1) - 1;
}

QDateTime CctwqtDataFrame::lastReferenced()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_LastReferenced;
}
