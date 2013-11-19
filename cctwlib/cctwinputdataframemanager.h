#ifndef CCTWINPUTDATAFRAMEMANAGER_H
#define CCTWINPUTDATAFRAMEMANAGER_H

#include "cctwdataframemanager.h"

class CctwqtDataFrame;

class CctwInputDataFrameManager : public CctwDataFrameManager
{
  Q_OBJECT
public:
  explicit CctwInputDataFrameManager(QObject *parent);
  
signals:
  
public slots:
  int  loadChunk(int ckx, int cky, int ckz);
  void releaseChunk(int chunkId);
  void writeChunk(CctwqtDataChunk *chunk);

  void beginTransform();
  void endTransform();

private:
  void garbageCollectFrames();

private:
  int m_NLoadedFrames;
  int m_NLoadedFramesLimit;
};

#endif // CCTWINPUTDATAFRAMEMANAGER_H
