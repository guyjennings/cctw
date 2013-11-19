#ifndef CCTWDATAFRAMEMANAGER_H
#define CCTWDATAFRAMEMANAGER_H

#include "cctwobject.h"
#include "cctwchunkeddata.h"
#include <QVector>

class CctwDataFrame;

class CctwDataFrameManager : public CctwObject
{
  Q_OBJECT
public:
  explicit CctwDataFrameManager(QObject *parent);

signals:

public slots:
  void setData(CctwChunkedData *data);
  virtual int  loadChunk(int nx, int ny, int nz) = 0;
  virtual void releaseChunk(int chunkId) = 0;
  virtual void writeChunk(CctwDataChunk *chunk) = 0;

  virtual void beginTransform() = 0;
  virtual void endTransform() = 0;

protected:
  QMutex                               m_Mutex;
  CctwChunkedData                     *m_Data;
  QVector<CctwDataFrame*>              m_Frames;
};

#endif // CCTWDATAFRAMEMANAGER_H
