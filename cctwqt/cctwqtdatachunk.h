#ifndef CCTWQTDATACHUNK_H
#define CCTWQTDATACHUNK_H

#include "cctwqtobject.h"
#include <QVector>
#include <QSharedPointer>
#include "cctwqtdataframe.h"
#include "../cctwchunkeddatainterface.h"
#include "cctwqtdataframemanager.h"
#include <QMutex>

class CctwqtDataChunk : public CctwqtObject
{
  Q_OBJECT
public:
  explicit CctwqtDataChunk(CctwIntVector3D idx, CctwChunkedDataInterface *data, CctwqtDataFrameManager *manager, QObject *parent);
  
  void clearDependencies();
  void addDependency(CctwIntVector3D dep);
  void reportDependencies();

signals:
  
public slots:
  
private:
  CctwIntVector3D                              m_ChunkIndex;
  CctwChunkedDataInterface                    *m_Data;
  CctwqtDataFrameManager                      *m_Manager;
  QVector< QSharedPointer <CctwqtDataFrame> >  m_DataFrames;
  QVector< CctwIntVector3D >                   m_Dependencies;
  QMutex                                       m_DependenciesLock;
};

#endif // CCTWQTDATACHUNK_H
