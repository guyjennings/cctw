#include "cctwoutputdatainterface.h"

CctwOutputDataInterface::CctwOutputDataInterface(CctwIntVector3D dim,
                                                 CctwIntVector3D chunkSize,
                                                 CctwqtOutputDataFrameManager *manager,
                                                 QObject *parent)
  : CctwqtChunkedData(dim, chunkSize, manager, parent)
{
}
