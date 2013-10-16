#ifndef CCTWQTINPUTDATAFRAMEMANAGER_H
#define CCTWQTINPUTDATAFRAMEMANAGER_H

#include "cctwqtdataframemanager.h"

class CctwqtDataFrame;

class CctwqtInputDataFrameManager : public CctwqtDataFrameManager
{
  Q_OBJECT
public:
  explicit CctwqtInputDataFrameManager(QObject *parent);
  
signals:
  
public slots:
  int  loadChunk(int ckx, int cky, int ckz);
  void releaseChunk(int chunkId);
  void writeChunk(CctwqtDataChunk *chunk);

private:
  void garbageCollectFrames();

private:
  int m_NLoadedFrames;
  int m_NLoadedFramesLimit;
};

#endif // CCTWQTINPUTDATAFRAMEMANAGER_H
