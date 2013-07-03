#ifndef CCTWCHUNKEDDATAINTERFACE_H
#define CCTWCHUNKEDDATAINTERFACE_H

#include "cctwvector3d.h"

class CctwChunkedDataInterface
{
public:
  CctwChunkedDataInterface(CctwIntVector3D dim,        // Data set dimension
                           CctwIntVector3D chunkSize,  // Chunk size
                           CctwDoubleVector3D origin,
                           CctwDoubleVector3D scale);

public:
  CctwIntVector3D     dimensions() const   { return m_Dimensions; }
  CctwIntVector3D     chunkSize() const    { return m_ChunkSize; }
  CctwDoubleVector3D  origin() const       { return m_Origin; }
  CctwDoubleVector3D  scale() const        { return m_Scale; }

  CctwDoubleVector3D  toReal(CctwIntVector3D vec);
  CctwIntVector3D     toPixel(CctwDoubleVector3D vec);

private:
  CctwIntVector3D     m_Dimensions;
  CctwIntVector3D     m_ChunkSize;
  CctwDoubleVector3D  m_Origin;
  CctwDoubleVector3D  m_Scale;
};

#endif // CCTWCHUNKEDDATAINTERFACE_H
