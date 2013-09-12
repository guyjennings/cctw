#ifndef CCTWQTOUTPUTDATAFRAMEMANAGER_H
#define CCTWQTOUTPUTDATAFRAMEMANAGER_H

#include "cctwqtdataframemanager.h"

class CctwqtDataFrame;

class CctwqtOutputDataFrameManager : public CctwqtDataFrameManager
{
  Q_OBJECT
public:
  explicit CctwqtOutputDataFrameManager(QObject *parent);

signals:

public slots:
  virtual int  loadChunk(int nx, int ny, int nz);
  virtual void releaseChunk(int chunkId);
};

#endif // CCTWQTOUTPUTDATAFRAMEMANAGER_H
