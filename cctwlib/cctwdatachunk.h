#ifndef CCTWDATACHUNK_H
#define CCTWDATACHUNK_H

#include "cctwobject.h"
#include "cctwvector3d.h"
#include <QMutex>
#include <QVector>
#include <QList>
#include "cctwchunkeddata.h"

class CctwDataChunk : public CctwObject
{
  Q_OBJECT
public:
  CctwDataChunk(CctwChunkedData *data,
                int index,
                QString name,
                QObject *parent);

  virtual ~CctwDataChunk();

public slots:

//  virtual int readData();
//  virtual int readWeights();
//  virtual int normalize();
//  virtual int writeData();
//  virtual int writeWeights();

  int allocateData();
  int allocateWeights();
  int deallocateData();
  int deallocateWeights();

  bool dataAllocated() const;
  bool weightsAllocated() const;

  double data(int lx, int ly, int lz);
  double weight(int lx, int ly, int lz);
  void setData(int lx, int ly, int lz, double val);
  void setWeight(int lx, int ly, int lz, double val);
  int pixelOffset(int lx, int ly, int lz);

  double *dataPointer();
  double *weightsPointer();

  CctwIntVector3D chunkStart();
  CctwIntVector3D chunkSize();

  void clearDependencies();
  void addDependency(int dep);
  void sortDependencies();
  int dependencyCount() const;
  int dependency(int n) const;

  void reportDependencies();

  int index() const;
  void mergeChunk(CctwDataChunk *c);
  void clearMergeCounters();
  void incMergeCounters();
  int mergeCount();

  bool popMergeData(double **data, double **weights);
  void pushMergeData(double *data, double *weights);

  void normalizeChunk();

//  void waitForData();
//  void finishedWithData();

  static void resetAllocationLimits(int nmax);
  static int maxAllocated();

private:
  double *allocateBuffer();
  void releaseBuffer(double *);
  CctwIntVector3D calculateChunkStart();
  CctwIntVector3D calculateChunkSize();

private:
  CctwChunkedData                           *m_Data;
  int                                        m_ChunkIndex;
  CctwIntVector3D                            m_ChunkStart;
  CctwIntVector3D                            m_ChunkSize;
  double                                    *m_ChunkData;
  double                                    *m_ChunkWeights;
  int                                        m_Normalized;
  int                                        m_DataWritten;
  int                                        m_WeightsWritten;
  QVector< int >                             m_Dependencies;
  QMutex                                     m_DependenciesLock;
  QMutex                                     m_MergeLock;
  int                                        m_MergeCounter;
  QList< double* >                           m_MergeData;
  QList< double* >                           m_MergeWeights;
};

#endif // CCTWDATACHUNK_H
