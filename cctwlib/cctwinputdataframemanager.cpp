#include "cctwinputdataframemanager.h"
#include <QThread>
#include "cctwqtdataframe.h"
#include "cctwchunkindex.h"
#include "qcepmutexlocker.h"

CctwInputDataFrameManager::CctwInputDataFrameManager(QObject *parent) :
  CctwDataFrameManager(parent),
  m_NLoadedFrames(0),
  m_NLoadedFramesLimit(256)
{
}

int CctwInputDataFrameManager::loadChunk(int ckx, int cky, int ckz)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  int cksize = m_Data->chunkSize().z();
  int fstart = ckz*cksize;

  for (int nz = fstart; nz < (fstart + cksize); nz++) {
    if (m_Frames[nz] == NULL) {
      printMessage(tr("Frame %1 Loaded").arg(nz));

//      CctwqtInputDataFrameManagerThread::msleep(100);

      m_Frames[nz] = new CctwqtDataFrame(m_Data->dimensions().x(),
                                       m_Data->dimensions().y(),
                                       NULL);

      m_NLoadedFrames++;

      if (m_NLoadedFrames > m_NLoadedFramesLimit) {
        garbageCollectFrames();
      }
    }

    m_Frames[nz] -> reference();
  }

  return m_Data->chunkNumberFromIndex(CctwIntVector3D(ckx, cky, ckz));
}

void CctwInputDataFrameManager::releaseChunk(int chunkId)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  int cksize = m_Data->chunkSize().z();
  int fstart = m_Data->chunkIndexFromNumber(chunkId).z()*cksize;

  for (int nz = fstart; nz < (fstart + cksize); nz++) {
    if (m_Frames[nz]) {
      if (m_Frames[nz] -> dereference() == 0) {
        printMessage(tr("Frame %1 Unloaded").arg(nz));
      }
    }
  }
}

void CctwInputDataFrameManager::garbageCollectFrames()
{
  int n = -1;
  QDateTime oldest;

  for (int i=0; i<m_Frames.count(); i++) {
    if (m_Frames[i]) {
      if (m_Frames[i]->referenceCount() == 0) {
        if (n == -1) {
          oldest = m_Frames[i]->lastReferenced();
          n = i;
        } else if (m_Frames[i]->lastReferenced() < oldest) {
          oldest = m_Frames[i]->lastReferenced();
          n = i;
        }
      }
    }
  }

  if (n != -1) {
    printMessage(tr("Deleting frame %1").arg(n));

    delete m_Frames[n];

    m_Frames[n] = NULL;
  }
}

void CctwInputDataFrameManager::writeChunk(CctwqtDataChunk *chunk)
{
  printMessage("Shouldn't write data on input dataset");
}

void CctwInputDataFrameManager::beginTransform()
{
}

void CctwInputDataFrameManager::endTransform()
{
}
