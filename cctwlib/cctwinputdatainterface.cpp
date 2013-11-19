#include "cctwinputdatainterface.h"

CctwInputDataInterface::CctwInputDataInterface(CctwIntVector3D dim,
                                               CctwIntVector3D chunkSize,
                                               CctwInputDataFrameManager *manager,
                                               QObject *parent)
  : CctwqtChunkedData(dim, chunkSize, manager, parent)
{
}
