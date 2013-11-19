#include "cctwinputdatainterface.h"

CctwInputDataInterface::CctwInputDataInterface(CctwIntVector3D dim,
                                               CctwIntVector3D chunkSize,
                                               CctwInputDataFrameManager *manager,
                                               QObject *parent)
  : CctwChunkedData(dim, chunkSize, manager, parent)
{
}
