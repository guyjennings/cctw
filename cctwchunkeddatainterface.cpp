#include "cctwchunkeddatainterface.h"

CctwChunkedDataInterface::CctwChunkedDataInterface(CctwVector3D<int> dim, CctwVector3D<int> chunkSize, CctwVector3D<double> scale)
  : m_Dimensions(dim),
    m_ChunkSize(chunkSize),
    m_Scale(scale)
{
}
