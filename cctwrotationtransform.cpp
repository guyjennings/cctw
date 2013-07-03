#include "cctwrotationtransform.h"

CctwRotationTransform::CctwRotationTransform()
{
}

CctwRotationTransform *CctwRotationTransform::createNew(int argc, char *argv[])
{
  return new CctwRotationTransform();
}

CctwVector3D<double> CctwRotationTransform::forward(CctwVector3D<double> a)
{
  return a;
}

CctwVector3D<double> CctwRotationTransform::inverse(CctwVector3D<double> q)
{
  return q;
}
