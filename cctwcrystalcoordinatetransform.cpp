#include "cctwcrystalcoordinatetransform.h"

CctwCrystalCoordinateTransform::CctwCrystalCoordinateTransform()
{
}

CctwCrystalCoordinateTransform *CctwCrystalCoordinateTransform::createNew(int argc, char *argv[])
{
  return new CctwCrystalCoordinateTransform();
}

CctwVector3D<double> CctwCrystalCoordinateTransform::forward(CctwVector3D<double> a)
{
  return a;
}

CctwVector3D<double> CctwCrystalCoordinateTransform::inverse(CctwVector3D<double> q)
{
  return q;
}
