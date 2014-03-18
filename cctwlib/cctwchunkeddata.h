#ifndef CCTWCHUNKEDDATA_H
#define CCTWCHUNKEDDATA_H

#include <QVector>
#include "cctwobject.h"
#include "cctwvector3d.h"
#include "cctwintvector3dproperty.h"
#include "qcepproperty.h"

class CctwDataChunk;
class CctwApplication;

class CctwChunkedData : public CctwObject
{
  Q_OBJECT
public:
  explicit CctwChunkedData(CctwApplication *application,
                           CctwIntVector3D dim,        // Data set dimension
                           CctwIntVector3D chunkSize,  // Chunk size
                           QString name,
                           QObject *parent);
  void allocateChunks();

signals:
  
public:
  CctwIntVector3D     dimensions() const   { return m_Dimensions; }
  CctwIntVector3D     chunkSize() const    { return m_ChunkSize; }

  void                setDimensions(CctwIntVector3D dim);
  void                setChunkSize(CctwIntVector3D cksz);

public slots:
  virtual void        setDataSource(QString desc);
  bool                containsPixel(CctwIntVector3D pixelCoord);
  bool                containsChunk(int ix, int iy, int iz);

  CctwIntVector3D     chunkStart(int n);    // Return pixel coords of start of chunk chunkIdx
  int                 chunkContaining(CctwIntVector3D pixelCoord);  // Return index of chunk containing given pixel
  int                 chunkContaining(CctwDoubleVector3D fracPixelCoord);  // Return index of chunk containing fractional pixel coords
  CctwIntVector3D     chunkCount() const;

  CctwIntVector3D     chunkIndexFromNumber(int n);
  int                 chunkNumberFromIndex(CctwIntVector3D chunkIdx);

  void                clearDependencies();
  void                addDependency(int f, int t);

  CctwDataChunk      *chunk(int n);
  CctwDataChunk      *chunk(CctwIntVector3D idx);

  CctwDataChunk      *readChunk(int n);
  void                writeChunk(int n);
  void                releaseChunk(int n);

  void                mergeChunk(CctwDataChunk *chunk);
  void                clearMergeCounters();

protected:
  QVector< CctwDataChunk* >  m_DataChunks;

private:
  CctwIntVector3D     m_Dimensions;
  CctwIntVector3D     m_ChunkSize;
  CctwIntVector3D     m_ChunkCount;

  Q_PROPERTY(QString dataFileName READ get_DataFileName WRITE set_DataFileName)
  QCEP_STRING_PROPERTY(DataFileName)

  Q_PROPERTY(QString dataSetName READ get_DataSetName WRITE set_DataSetName)
  QCEP_STRING_PROPERTY(DataSetName)

  Q_PROPERTY(CctwIntVector3D dimensions READ dimensions WRITE setDimensions)
//  CCTW_INTVECTOR3D_PROPERTY(Dimensions)

  Q_PROPERTY(CctwIntVector3D chunkSize READ chunkSize WRITE setChunkSize)
//  CCTW_INTVECTOR3D_PROPERTY(ChunkSize)

  Q_PROPERTY(CctwIntVector3D chunkCount READ chunkCount STORED false)
//  CCTW_INTVECTOR3D_PROPERTY(ChunkCount)

  Q_PROPERTY(int compression READ get_Compression WRITE set_Compression)
  QCEP_INTEGER_PROPERTY(Compression)
};

#endif // CCTWCHUNKEDDATA_H
