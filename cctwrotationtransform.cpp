#include "cctwrotationtransform.h"

CctwRotationTransform::CctwRotationTransform()
{
}

CctwVector3D CctwRotationTransform::forward(CctwVector3D a)
{
  return a;
}

CctwVector3D CctwRotationTransform::inverse(CctwVector3D q)
{
  return q;
}
