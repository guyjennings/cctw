#include "cctwrotationtransform.h"

CctwRotationTransform::CctwRotationTransform()
{
}

CctwRotationTransform *CctwRotationTransform::createNew(int argc, char *argv[])
{
  return new CctwRotationTransform();
}

CctwDoubleVector3D CctwRotationTransform::forward(CctwDoubleVector3D a)
{
  return a;
}

CctwDoubleVector3D CctwRotationTransform::inverse(CctwDoubleVector3D q)
{
  return q;
}
