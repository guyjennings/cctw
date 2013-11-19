#ifndef CCTWQTCHUNKEDDATA_H
#define CCTWQTCHUNKEDDATA_H

#include <QVector>
#include "cctwchunkeddatainterface.h"

class CctwDataFrameManager;
class CctwqtDataChunk;

class CctwqtChunkedData : public CctwChunkedDataInterface
{
  Q_OBJECT
public:
  explicit CctwqtChunkedData(CctwIntVector3D dim,        // Data set dimension
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

  CctwqtDataChunk *chunk(CctwIntVector3D idx);

  void mergeChunk(CctwqtDataChunk *chunk);
  void clearMergeCounters();

protected:
  CctwDataFrameManager      *m_Manager;
  QVector< CctwqtDataChunk* >  m_DataChunks;
};

#endif // CCTWQTCHUNKEDDATA_H
