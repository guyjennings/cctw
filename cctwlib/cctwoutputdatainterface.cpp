#include "cctwoutputdatainterface.h"

CctwOutputDataInterface::CctwOutputDataInterface(CctwIntVector3D dim,
                                                 CctwIntVector3D chunkSize,
                                                 CctwOutputDataFrameManager *manager,
                                                 QObject *parent)
  : CctwChunkedData(dim, chunkSize, manager, parent)
{
}
