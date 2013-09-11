#ifndef CCTWQTCHUNKEDDATA_H
#define CCTWQTCHUNKEDDATA_H

#include <QVector>
#include "../cctwchunkeddatainterface.h"

class CctwqtDataFrameManager;
class CctwqtDataChunk;

class CctwqtChunkedData : public CctwChunkedDataInterface
{
  Q_OBJECT
public:
  explicit CctwqtChunkedData(CctwIntVector3D dim,        // Data set dimension
                             CctwIntVector3D chunkSize,  // Chunk size
                             CctwDoubleVector3D origin,
                             CctwDoubleVector3D scale,
                             CctwqtDataFrameManager *manager,
                             QObject *parent);
  void allocateChunks();

signals:
  
public slots:
  void clearDependencies();
  void addDependency(CctwIntVector3D f, CctwIntVector3D t);

  CctwqtDataChunk *chunk(CctwIntVector3D idx);

private:
  CctwqtDataFrameManager      *m_Manager;
  QVector< CctwqtDataChunk* >  m_DataChunks;
};

#endif // CCTWQTCHUNKEDDATA_H
