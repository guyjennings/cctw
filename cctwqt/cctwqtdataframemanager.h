#ifndef CCTWQTDATAFRAMEMANAGER_H
#define CCTWQTDATAFRAMEMANAGER_H

#include "cctwqtobject.h"
#include "cctwqtchunkeddata.h"
#include <QVector>

class CctwqtDataFrame;

class CctwqtDataFrameManager : public CctwqtObject
{
  Q_OBJECT
public:
  explicit CctwqtDataFrameManager(QObject *parent);

signals:

public slots:
  void setData(CctwqtChunkedData *data);
  virtual int  loadChunk(int nx, int ny, int nz) = 0;
  virtual void releaseChunk(int chunkId) = 0;

protected:
  QMutex                                 m_Mutex;
  CctwqtChunkedData                     *m_Data;
  QVector<CctwqtDataFrame*>              m_Frames;
};

#endif // CCTWDATAFRAMEMANAGER_H
