#include "cctwdataframemanager.h"
#include "qcepmutexlocker.h"

CctwDataFrameManager::CctwDataFrameManager(QObject *parent) :
  CctwObject(parent),
  m_Data(NULL)
{
}

void CctwDataFrameManager::setData(CctwqtChunkedData *data)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Data = data;

  int n = m_Data->dimensions().z();

  m_Frames.resize(n);

  for(int i=0; i<n; i++) {
    m_Frames[i] = NULL;
  }
}

