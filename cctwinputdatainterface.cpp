#include "cctwinputdatainterface.h"

#ifdef USE_QT

CctwInputDataInterface::CctwInputDataInterface(CctwIntVector3D dim,
                                               CctwIntVector3D chunkSize,
//                                               CctwDoubleVector3D origin,
//                                               CctwDoubleVector3D scale,
                                               CctwqtInputDataFrameManager *manager,
                                               QObject *parent)
  : CctwqtChunkedData(dim, chunkSize, /*origin, scale,*/ manager, parent)
{
}

#else

CctwInputDataInterface::CctwInputDataInterface(CctwIntVector3D dim,
                                               CctwIntVector3D chunkSize/*,
                                               CctwDoubleVector3D origin,
                                               CctwDoubleVector3D scale*/)
  : CctwChunkedDataInterface(dim, chunkSize/*, origin, scale*/)
{
}

#endif
