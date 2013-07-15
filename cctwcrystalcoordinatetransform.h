#ifndef CCTWCRYSTALCOORDINATETRANSFORM_H
#define CCTWCRYSTALCOORDINATETRANSFORM_H

#include "cctwtransforminterface.h"
#include "cctwmatrix3x3.h"

class CctwCrystalCoordinateTransform : public CctwTransformInterface
{
public:
  CctwCrystalCoordinateTransform();

public:
  static CctwCrystalCoordinateTransform* createNew(int argc, char *argv[]);
                                          // Allocate and return a new crystal coordinate transformation object
                                          // based on the command line parameters passed in argc and argv

  virtual CctwDoubleVector3D forward(CctwDoubleVector3D a);
                                          // Transform from input to output space coordinates
  virtual CctwDoubleVector3D inverse(CctwDoubleVector3D q);
                                          // Back transform from output to input space coordinates

public:
  double m_Lambda;

  CctwDoubleMatrix3x3 m_B;
  CctwDoubleMatrix3x3 m_U;
  CctwDoubleMatrix3x3 m_G;
  CctwDoubleMatrix3x3 m_D;
  CctwDoubleMatrix3x3 m_O;

  CctwDoubleVector3D m_Tds;
  CctwDoubleVector3D m_Te;
  CctwDoubleVector3D m_Tpd;
  CctwDoubleVector3D m_Tgs0;

private:
  CctwDoubleMatrix3x3 m_Binv;
  CctwDoubleMatrix3x3 m_Uinv;
  CctwDoubleMatrix3x3 m_Ginv;
  CctwDoubleMatrix3x3 m_Dinv;
  CctwDoubleMatrix3x3 m_Oinv;

  CctwDoubleVector3D m_Tgs;
};

#endif // CCTWCRYSTALCOORDINATETRANSFORM_H
