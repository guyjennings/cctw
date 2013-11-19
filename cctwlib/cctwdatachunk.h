#ifndef CCTWDATACHUNK_H
#define CCTWDATACHUNK_H

#include "cctwobject.h"
#include "cctwvector3d.h"
#include <QMutex>
#include <QVector>
#include <QList>
#include "cctwchunkeddata.h"
#include "cctwdataframemanager.h"

class CctwDataChunk : public CctwObject
{
  Q_OBJECT
public:
  CctwDataChunk(CctwChunkedData *data,
                CctwIntVector3D index,
                CctwDataFrameManager *manager,
                QObject *parent);

  virtual ~CctwDataChunk();

public slots:

  virtual int readData();
  virtual int readWeights();
  virtual int normalize();
  virtual int writeData();
  virtual int writeWeights();

  int allocateData();
  int allocateWeights();
  int deallocateData();
  int deallocateWeights();

  bool dataAllocated() const;
  bool weightsAllocated() const;

  double data(CctwIntVector3D localcoords);
  double weight(CctwIntVector3D localcoords);
  void setData(CctwIntVector3D localcoords, double val);
  void setWeight(CctwIntVector3D localcoords, double val);
  int pixelOffset(CctwIntVector3D localcoords);

  double *dataPointer();
  double *weightsPointer();

  CctwIntVector3D chunkSize();

  void clearDependencies();
  void addDependency(CctwIntVector3D dep);
  void sortDependencies();
  int  dependencyCount() const;
  CctwIntVector3D dependency(int n) const;

  void reportDependencies();

  CctwIntVector3D index() const;
  void mergeChunk(CctwDataChunk *c);
  void clearMergeCounters();
  void incMergeCounters();
  int mergeCount();

  bool popMergeData(double **data, double **weights);
  void pushMergeData(double *data, double *weights);

  void waitForData();
  void finishedWithData();

  static void resetAllocationLimits(int nmax);
  static int maxAllocated();

private:
  double *allocateBuffer();
  void releaseBuffer(double *);

private:
  CctwChunkedData                           *m_Data;
  CctwDataFrameManager                      *m_Manager;
  CctwIntVector3D                            m_ChunkIndex;
  double                                    *m_ChunkData;
  double                                    *m_ChunkWeights;
  int                                        m_Normalized;
  int                                        m_DataWritten;
  int                                        m_WeightsWritten;
  QVector< QSharedPointer <CctwDataFrame> >  m_DataFrames;
  QVector< CctwIntVector3D >                 m_Dependencies;
  QMutex                                     m_DependenciesLock;
  QMutex                                     m_MergeLock;
  int                                        m_MergeCounter;
  QList< double* >                           m_MergeData;
  QList< double* >                           m_MergeWeights;
};

#endif // CCTWDATACHUNK_H
