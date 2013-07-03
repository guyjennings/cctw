#include "cctwcrystalcoordinatetransform.h"

CctwCrystalCoordinateTransform::CctwCrystalCoordinateTransform()
{
}

CctwCrystalCoordinateTransform *CctwCrystalCoordinateTransform::createNew(int argc, char *argv[])
{
  return new CctwCrystalCoordinateTransform();
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::forward(CctwDoubleVector3D a)
{
  return a;
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::inverse(CctwDoubleVector3D q)
{
  return q;
}
