#include "cctwrotationtransform.h"

CctwRotationTransform::CctwRotationTransform(QObject *parent) :
  CctwTransformInterface(parent)
{
}

bool CctwRotationTransform::hasInverse() const
{
  return true;
}

CctwDoubleVector3D CctwRotationTransform::forward(CctwDoubleVector3D a)
{
  return a;
}

CctwDoubleVector3D CctwRotationTransform::inverse(CctwDoubleVector3D q)
{
  return q;
}
