#ifndef CCTWQTDATACHUNK_H
#define CCTWQTDATACHUNK_H

#include "cctwdatachunk.h"
#include <QVector>
#include <QSharedPointer>
#include "cctwdataframe.h"
#include "cctwqtchunkeddata.h"
#include "cctwdataframemanager.h"
#include <QMutex>

class CctwqtDataChunk : public CctwDataChunk
{
  Q_OBJECT
public:
  explicit CctwqtDataChunk(CctwqtChunkedData *data,
                           CctwIntVector3D idx,
                           CctwDataFrameManager *manager,
                           QObject *parent);
  ~CctwqtDataChunk();

  int  readData();
  int  readWeights();

  int  writeData();
  int  writeWeights();

  void clearDependencies();
  void addDependency(CctwIntVector3D dep);
  void sortDependencies();
  int  dependencyCount() const;
  CctwIntVector3D dependency(int n) const;

  void reportDependencies();

  CctwIntVector3D index() const;
  void mergeChunk(CctwqtDataChunk *c);
  void clearMergeCounters();
  void incMergeCounters();
  int mergeCount();

  bool popMergeData(double **data, double **weights);
  void pushMergeData(double *data, double *weights);

  void waitForData();
  void finishedWithData();

  static void resetAllocationLimits(int nmax);
  static int maxAllocated();

protected:
  double *allocateBuffer();
  void releaseBuffer(double *buffer);

signals:
  
public slots:
  
private:
//  CctwIntVector3D                              m_ChunkIndex;
  CctwqtChunkedData                           *m_Data;
  CctwDataFrameManager                      *m_Manager;
  QVector< QSharedPointer <CctwDataFrame> >  m_DataFrames;
  QVector< CctwIntVector3D >                   m_Dependencies;
  QMutex                                       m_DependenciesLock;
  QMutex                                       m_MergeLock;
  int                                          m_MergeCounter;
  QList< double* >                             m_MergeData;
  QList< double* >                             m_MergeWeights;
};

#endif // CCTWQTDATACHUNK_H
