#ifndef CCTWCHUNKEDDATAINTERFACE_H
#define CCTWCHUNKEDDATAINTERFACE_H

#ifdef USE_QT
#include "cctwqtobject.h"
#endif

#include "cctwvector3d.h"

#ifdef USE_QT
class CctwChunkedDataInterface : public CctwqtObject
{
  Q_OBJECT
#else
class CctwChunkedDataInterface
{
#endif
public:
  CctwChunkedDataInterface(CctwIntVector3D dim,        // Data set dimension
                           CctwIntVector3D chunkSize,  // Chunk size
                           CctwDoubleVector3D origin,
                           CctwDoubleVector3D scale);

  CctwChunkedDataInterface();

#ifdef USE_QT
public slots:
#else
public:
#endif

  void                initialize(void *buffer); // Initialize from byte buffer

  CctwIntVector3D     dimensions() const   { return m_Dimensions; }
  CctwIntVector3D     chunkSize() const    { return m_ChunkSize; }
  CctwDoubleVector3D  origin() const       { return m_Origin; }
  CctwDoubleVector3D  scale() const        { return m_Scale; }

  CctwDoubleVector3D  toReal(CctwIntVector3D vec);
  CctwIntVector3D     toPixel(CctwDoubleVector3D vec);

  bool                containsPixel(CctwIntVector3D pixelCoord);
  bool                containsChunk(CctwIntVector3D chunkIdx);

  CctwIntVector3D     chunkStart(CctwIntVector3D chunkIdx);    // Return pixel coords of start of chunk chunkIdx
  CctwIntVector3D     chunkIndex(CctwIntVector3D pixelCoord);  // Return index of chunk containing given pixel
  CctwIntVector3D     chunkIndex(CctwDoubleVector3D fracPixelCoord);  // Return index of chunk containing fractional pixel coords
  CctwIntVector3D     chunkCount() const;

  CctwIntVector3D     chunkIndexFromNumber(int n);
  int                 chunkNumberFromIndex(CctwIntVector3D chunkIdx);

  CctwIntVector3D     findChunkIndexContaining(CctwDoubleVector3D coords);

private:
  CctwIntVector3D     m_Dimensions;
  CctwIntVector3D     m_ChunkSize;
  CctwDoubleVector3D  m_Origin;
  CctwDoubleVector3D  m_Scale;
  CctwIntVector3D     m_ChunkCount;
};

#endif // CCTWCHUNKEDDATAINTERFACE_H
