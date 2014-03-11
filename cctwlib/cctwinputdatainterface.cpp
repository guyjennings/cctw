#include "cctwinputdatainterface.h"

CctwInputDataInterface::CctwInputDataInterface(CctwApplication *application,
                                               CctwIntVector3D dim,
                                               CctwIntVector3D chunkSize,
                                               CctwInputDataFrameManager *manager, QString name,
                                               QObject *parent)
  : CctwChunkedData(application, dim, chunkSize, manager, name, parent)
{
}
