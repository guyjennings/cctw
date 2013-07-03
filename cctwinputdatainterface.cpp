#include "cctwinputdatainterface.h"

CctwInputDataInterface::CctwInputDataInterface(CctwVector3D<int> dim, CctwVector3D<int> chunkSize, CctwVector3D<double> scale)
  : CctwChunkedDataInterface(dim, chunkSize, scale)
{
}
