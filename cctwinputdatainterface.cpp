#include "cctwinputdatainterface.h"

CctwInputDataInterface::CctwInputDataInterface(CctwIntVector3D dim, CctwIntVector3D chunkSize, CctwDoubleVector3D origin, CctwDoubleVector3D scale)
  : CctwChunkedDataInterface(dim, chunkSize, origin, scale)
{
}
