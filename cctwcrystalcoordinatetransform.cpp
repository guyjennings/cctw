#include "cctwcrystalcoordinatetransform.h"

CctwCrystalCoordinateTransform::CctwCrystalCoordinateTransform()
{
  m_U = CctwDoubleMatrix3x3::rotationMatrix(0.3, 0.4, 0.5);
  m_Uinv = m_U.inverted();
}

CctwCrystalCoordinateTransform *CctwCrystalCoordinateTransform::createNew(int argc, char *argv[])
{
  return new CctwCrystalCoordinateTransform();
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::forward(CctwDoubleVector3D a)
{
  return m_U*a;
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::inverse(CctwDoubleVector3D q)
{
  return m_Uinv*q;
}
