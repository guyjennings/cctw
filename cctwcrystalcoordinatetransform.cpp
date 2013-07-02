#include "cctwcrystalcoordinatetransform.h"

CctwCrystalCoordinateTransform::CctwCrystalCoordinateTransform()
{
}

CctwVector3D CctwCrystalCoordinateTransform::forward(CctwVector3D a)
{
  return a;
}

CctwVector3D CctwCrystalCoordinateTransform::inverse(CctwVector3D q)
{
  return q;
}
