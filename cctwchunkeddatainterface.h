#ifndef CCTWCHUNKEDDATAINTERFACE_H
#define CCTWCHUNKEDDATAINTERFACE_H

#include "cctwvector3d.h"

class CctwChunkedDataInterface
{
public:
  CctwChunkedDataInterface(CctwVector3D<int> dim,        // Data set dimension
                           CctwVector3D<int> chunkSize,  // Chunk size
                           CctwVector3D<double> scale);

private:
  CctwVector3D<int>     m_Dimensions;
  CctwVector3D<int>     m_ChunkSize;
  CctwVector3D<double>  m_Scale;
};

#endif // CCTWCHUNKEDDATAINTERFACE_H
