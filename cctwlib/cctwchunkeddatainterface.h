#ifndef CCTWCHUNKEDDATAINTERFACE_H
#define CCTWCHUNKEDDATAINTERFACE_H

#include "cctwobject.h"
#include "cctwvector3d.h"
#include "cctwintvector3dproperty.h"

class CctwApplication;

class CctwChunkedDataInterface : public CctwObject
{
  Q_OBJECT
public:
  CctwChunkedDataInterface(CctwApplication *application,
                           CctwIntVector3D dim,        // Data set dimension
                           CctwIntVector3D chunkSize,  // Chunk size
//                           CctwDoubleVector3D origin,
//                           CctwDoubleVector3D scale,
                           QString name,
                           QObject *parent);

public:

  void                initialize(void *buffer); // Initialize from byte buffer

  CctwIntVector3D     dimensions() const   { return m_Dimensions; }
  CctwIntVector3D     chunkSize() const    { return m_ChunkSize; }

  virtual void setDimensions(CctwIntVector3D dim);
  virtual void setChunkSize(CctwIntVector3D cksz);

public slots:

  bool                containsPixel(CctwIntVector3D pixelCoord);
  bool                containsChunk(CctwIntVector3D chunkIdx);

  CctwIntVector3D     chunkStart(CctwIntVector3D chunkIdx);    // Return pixel coords of start of chunk chunkIdx
  CctwIntVector3D     chunkIndex(CctwIntVector3D pixelCoord);  // Return index of chunk containing given pixel
  CctwIntVector3D     chunkIndex(CctwDoubleVector3D fracPixelCoord);  // Return index of chunk containing fractional pixel coords
  CctwIntVector3D     chunkCount() const;

  CctwIntVector3D     chunkIndexFromNumber(int n);
  int                 chunkNumberFromIndex(CctwIntVector3D chunkIdx);

  virtual void        clearMergeCounters() = 0;

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

#endif // CCTWCHUNKEDDATAINTERFACE_H
