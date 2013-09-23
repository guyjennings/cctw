#ifndef CCTWQTDATACHUNK_H
#define CCTWQTDATACHUNK_H

#include "cctwdatachunk.h"
#include <QVector>
#include <QSharedPointer>
#include "cctwqtdataframe.h"
#include "cctwqtchunkeddata.h"
#include "cctwqtinputdataframemanager.h"
#include <QMutex>

class CctwqtDataChunk : public CctwDataChunk
{
  Q_OBJECT
public:
  explicit CctwqtDataChunk(CctwqtChunkedData *data,
                           CctwIntVector3D idx,
                           CctwqtDataFrameManager *manager,
                           QObject *parent);
  
  void clearDependencies();
  void addDependency(CctwIntVector3D dep);
  void sortDependencies();
  int  dependencyCount() const;
  CctwIntVector3D dependency(int n) const;

  void reportDependencies();
signals:
  
public slots:
  
private:
  CctwIntVector3D                              m_ChunkIndex;
  CctwqtChunkedData                           *m_Data;
  CctwqtDataFrameManager                      *m_Manager;
  QVector< QSharedPointer <CctwqtDataFrame> >  m_DataFrames;
  QVector< CctwIntVector3D >                   m_Dependencies;
  QMutex                                       m_DependenciesLock;
};

#endif // CCTWQTDATACHUNK_H
