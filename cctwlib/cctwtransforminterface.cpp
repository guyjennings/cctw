#include "cctwtransforminterface.h"

CctwTransformInterface::CctwTransformInterface(QString name, QObject *parent) :
  CctwObject(name, parent)
{
}

CctwDoubleVector3D CctwTransformInterface::forward(double x, double y, double z)
{
  return forward(CctwDoubleVector3D(x,y,z));
}

CctwDoubleVector3D CctwTransformInterface::inverse(double x, double y, double z)
{
  return inverse(CctwDoubleVector3D(x,y,z));
}
