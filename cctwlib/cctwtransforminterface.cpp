#include "cctwtransforminterface.h"

#ifdef USE_QT
CctwTransformInterface::CctwTransformInterface(QObject *parent) :
  CctwqtObject(parent)
{
}
#else
CctwTransformInterface::CctwTransformInterface()
{
}
#endif

CctwDoubleVector3D CctwTransformInterface::forward(double x, double y, double z)
{
  return forward(CctwDoubleVector3D(x,y,z));
}

CctwDoubleVector3D CctwTransformInterface::inverse(double x, double y, double z)
{
  return inverse(CctwDoubleVector3D(x,y,z));
}
