#include "cctwoutputdatainterface.h"

CctwOutputDataInterface::CctwOutputDataInterface(CctwApplication *application, CctwIntVector3D dim,
                                                 CctwIntVector3D chunkSize,
                                                 CctwOutputDataFrameManager *manager, QString name,
                                                 QObject *parent)
  : CctwChunkedData(application, dim, chunkSize, manager, name, parent)
{
}
