#include "cctwdataframe.h"
#include "qcepmutexlocker.h"

CctwDataFrame::CctwDataFrame(int dimx, int dimy, QString name, QObject *parent) :
  CctwObject(name, parent),
  m_Counter(0),
  m_DimX(dimx),
  m_DimY(dimy),
  m_Data(dimx*dimy),
  m_LastReferenced(QDateTime::currentDateTime())
{
}

CctwDataFrame::~CctwDataFrame()
{
}

int CctwDataFrame::reference()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_LastReferenced = QDateTime::currentDateTime();

  return m_Counter.fetchAndAddOrdered(1) + 1;
}

int CctwDataFrame::referenceCount()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Counter.fetchAndAddOrdered(0);
}

int CctwDataFrame::dereference()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_Counter.fetchAndAddOrdered(-1) - 1;
}

QDateTime CctwDataFrame::lastReferenced()
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  return m_LastReferenced;
}
