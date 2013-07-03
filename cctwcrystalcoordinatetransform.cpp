#include "cctwcrystalcoordinatetransform.h"

CctwCrystalCoordinateTransform::CctwCrystalCoordinateTransform()
{
}

CctwCrystalCoordinateTransform *CctwCrystalCoordinateTransform::createNew(int argc, char *argv[])
{
  return new CctwCrystalCoordinateTransform();
}

CctwVector3D CctwCrystalCoordinateTransform::forward(CctwVector3D a)
{
  return a;
}

CctwVector3D CctwCrystalCoordinateTransform::inverse(CctwVector3D q)
{
  return q;
}
