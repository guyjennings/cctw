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

  int allocateData();
  int allocateWeights();
  int deallocateData();
  int deallocateWeights();
  int detachData();
  int detachWeights();

  bool dataAllocated() const;
  bool weightsAllocated() const;

  CctwChunkedData::MergeDataType data(int lx, int ly, int lz);
  CctwChunkedData::MergeDataType weight(int lx, int ly, int lz);
  void setData(int lx, int ly, int lz, CctwChunkedData::MergeDataType val);
  void setWeight(int lx, int ly, int lz, CctwChunkedData::MergeDataType val);
  int pixelOffset(int lx, int ly, int lz);

  CctwChunkedData::MergeDataType *dataPointer();
  CctwChunkedData::MergeDataType *weightsPointer();

  CctwIntVector3D chunkStart();
  void resetChunkStart();
  CctwIntVector3D chunkSize();
  void setChunkSize(CctwIntVector3D size);

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

  bool popMergeData(CctwChunkedData::MergeDataType **data, CctwChunkedData::MergeDataType **weights);
  void pushMergeData(CctwChunkedData::MergeDataType *data, CctwChunkedData::MergeDataType *weights);

  void normalizeChunk();

  static void resetAllocationLimits(int nmax);
  static int maxAllocated();

  void setBuffer(void *buffer);

  void mergeData(CctwChunkedData::MergeDataType *id,
                 CctwChunkedData::MergeDataType *iw,
                 int n);

private:
  CctwChunkedData::MergeDataType *allocateBuffer();
  void releaseBuffer(CctwChunkedData::MergeDataType *);
  CctwIntVector3D calculateChunkStart();
  CctwIntVector3D calculateChunkSize();

private:
  CctwChunkedData                           *m_Data;
  int                                        m_ChunkIndex;
  CctwIntVector3D                            m_ChunkStart;
  CctwIntVector3D                            m_ChunkSize;
  CctwChunkedData::MergeDataType            *m_ChunkData;
  CctwChunkedData::MergeDataType            *m_ChunkWeights;
  int                                        m_Normalized;
  int                                        m_DataWritten;
  int                                        m_WeightsWritten;
  QVector< int >                             m_Dependencies;
  QMutex                                     m_DependenciesLock;
  QMutex                                     m_MergeLock;
  int                                        m_MergeCounter;
  QList< CctwChunkedData::MergeDataType* >   m_MergeData;
  QList< CctwChunkedData::MergeDataType* >   m_MergeWeights;
  /** True iff this object owns the chunk memory */
  bool                                       m_OwnData;
};

#endif // CCTWDATACHUNK_H
