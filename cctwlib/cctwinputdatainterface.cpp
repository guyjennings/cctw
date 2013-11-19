#include "cctwinputdatainterface.h"

CctwInputDataInterface::CctwInputDataInterface(CctwIntVector3D dim,
                                               CctwIntVector3D chunkSize,
                                               CctwqtInputDataFrameManager *manager,
                                               QObject *parent)
  : CctwqtChunkedData(dim, chunkSize, manager, parent)
{
}
