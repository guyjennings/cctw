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
  void printMessage(QString msg);
  int  loadChunk(int nx, int ny, int nz);
  void releaseChunk(int chunkId);
  void setData(CctwqtChunkedData *data);

private:
  QObject                               *m_Parent;
  CctwqtChunkedData                     *m_Data;
  QVector<CctwqtDataFrame*>              m_Frames;
};

#endif // CCTWQTDATAFRAMEMANAGER_H
