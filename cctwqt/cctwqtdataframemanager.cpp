#include "cctwqtdataframemanager.h"
#include "qcepmutexlocker.h"

CctwqtDataFrameManager::CctwqtDataFrameManager(QObject *parent) :
  CctwObject(parent),
  m_Data(NULL)
{
}

void CctwqtDataFrameManager::setData(CctwqtChunkedData *data)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  m_Data = data;

  int n = m_Data->dimensions().z();

  m_Frames.resize(n);

  for(int i=0; i<n; i++) {
    m_Frames[i] = NULL;
  }
}

