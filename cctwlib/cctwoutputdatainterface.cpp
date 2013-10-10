#include "cctwoutputdatainterface.h"

#ifdef USE_QT

CctwOutputDataInterface::CctwOutputDataInterface(CctwIntVector3D dim,
                                                 CctwIntVector3D chunkSize,
                                                 /*CctwDoubleVector3D origin,
                                                 CctwDoubleVector3D scale, */CctwqtOutputDataFrameManager *manager,
                                                 QObject *parent)
  : CctwqtChunkedData(dim, chunkSize, /*origin, scale,*/ manager, parent)
{
}

#else

CctwOutputDataInterface::CctwOutputDataInterface(CctwIntVector3D dim,
                                                 CctwIntVector3D chunkSize/*,
                                                 CctwDoubleVector3D origin,
                                                 CctwDoubleVector3D scale*/)
  : CctwChunkedDataInterface(dim, chunkSize/*, origin, scale*/)
{
}

#endif
