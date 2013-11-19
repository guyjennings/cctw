#ifndef CCTWCHUNKEDDATA_H
#define CCTWCHUNKEDDATA_H

#include <QVector>
#include "cctwchunkeddatainterface.h"

class CctwDataFrameManager;
class CctwDataChunk;

class CctwChunkedData : public CctwChunkedDataInterface
{
  Q_OBJECT
public:
  explicit CctwChunkedData(CctwIntVector3D dim,        // Data set dimension
                             CctwIntVector3D chunkSize,  // Chunk size
//                             CctwDoubleVector3D origin,
//                             CctwDoubleVector3D scale,
                             CctwDataFrameManager *manager,
                             QObject *parent);
  void allocateChunks();

signals:
  
public slots:
  void clearDependencies();
  void addDependency(CctwIntVector3D f, CctwIntVector3D t);

  void setDimensions(CctwIntVector3D dim);
  void setChunkSize(CctwIntVector3D cksz);

  CctwDataChunk *chunk(CctwIntVector3D idx);

  void mergeChunk(CctwDataChunk *chunk);
  void clearMergeCounters();

protected:
  CctwDataFrameManager      *m_Manager;
  QVector< CctwDataChunk* >  m_DataChunks;
};

#endif // CCTWCHUNKEDDATA_H
