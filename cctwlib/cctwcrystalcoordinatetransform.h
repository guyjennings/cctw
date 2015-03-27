#ifndef CCTWCRYSTALCOORDINATETRANSFORM_H
#define CCTWCRYSTALCOORDINATETRANSFORM_H

#include "cctwtransforminterface.h"
#include "cctwmatrix3x3.h"
#include "cctwcrystalcoordinateparameters.h"

class CctwCrystalCoordinateTransform : public CctwTransformInterface
{
  Q_OBJECT
public:
  CctwCrystalCoordinateTransform(CctwCrystalCoordinateParameters *parms, QString name, const double *angles, QObject *parent);

public slots:

  virtual bool hasInverse() const;

  virtual CctwDoubleVector3D forward(CctwDoubleVector3D d);
                                          // Transform from input to output space coordinates
  virtual CctwDoubleVector3D inverse(CctwDoubleVector3D q);
                                          // Back transform from output to input space coordinates

  void updateFromParameters();
  void setCurrentFrame(double frame);

  double changeCount();
  void report();

  CctwDoubleMatrix3x3 createBMatrix(const CctwUnitCell &cell) const;

  CctwDoubleVector3D getDetPos(double x, double y);
  CctwDoubleVector3D getDetPos(CctwDoubleVector3D xyz);
  CctwDoubleVector3D pixel2qlab(double x, double y, double z);
  CctwDoubleVector3D pixel2qlab(CctwDoubleVector3D pixel);
  CctwDoubleVector3D qlab2hkl(double qx, double qy, double qz);
  CctwDoubleVector3D qlab2hkl(CctwDoubleVector3D qlab);
  CctwDoubleVector3D hkl2grid(double h, double k, double l);
  CctwDoubleVector3D hkl2grid(CctwDoubleVector3D hkl);
  CctwDoubleVector3D grid2hkl(double gx, double gy, double gz);
  CctwDoubleVector3D grid2hkl(CctwDoubleVector3D grid);
  CctwDoubleVector3D hkl2qlab(double h, double k, double l);
  CctwDoubleVector3D hkl2qlab(CctwDoubleVector3D hkl);
  CctwDoubleVector3D qlab2pixel(double qx, double qy, double qz);
  CctwDoubleVector3D qlab2pixel(CctwDoubleVector3D qlab);
  CctwDoubleVector3D getDetPix(double x, double y, double z);
  CctwDoubleVector3D getDetPix(CctwDoubleVector3D xyz);

  double twoThetaAngle() { return m_TwoThetaAngle; }
  double omegaAngle() { return m_OmegaAngle; }
  double chiAngle() { return m_ChiAngle; }
  double phiAngle() { return m_PhiAngle; }

  CctwDoubleMatrix3x3 bMat() { return m_BMat; }
  CctwDoubleMatrix3x3 uMat() { return m_UMat; }
  CctwDoubleMatrix3x3 gMat() { return m_GMat; }
  CctwDoubleMatrix3x3 dMat() { return m_DMat; }
  CctwDoubleMatrix3x3 oMat() { return m_OMat; }
  CctwDoubleVector3D  oVec() { return m_OVec; }

  CctwDoubleMatrix3x3 ubMatInv() { return m_UBMatInv; }
  CctwDoubleMatrix3x3 gridBasisInv() { return m_GridBasisInv; }
  CctwDoubleMatrix3x3 bMatInv() { return m_BMatInv; }
  CctwDoubleMatrix3x3 uMatInv() { return m_UMatInv; }
  CctwDoubleMatrix3x3 gMatInv() { return m_GMatInv; }
  CctwDoubleMatrix3x3 dMatInv() { return m_DMatInv; }
  CctwDoubleMatrix3x3 oMatInv() { return m_OMatInv; }

  CctwDoubleVector3D cdVec() { return m_CD; }
  CctwDoubleVector3D ndVec() { return m_ND; }

private:
  CctwCrystalCoordinateParameters *m_Parms;

  double m_CurrentFrame;
  int    m_CurrentFrameChangeCount;

  const double *m_Angles;

public:
  double m_TwoThetaAngle;
  double m_OmegaAngle;
  double m_ChiAngle;
  double m_PhiAngle;

  CctwDoubleMatrix3x3 m_BMat;
  CctwDoubleMatrix3x3 m_UMat;
  CctwDoubleMatrix3x3 m_GMat;
  CctwDoubleMatrix3x3 m_DMat;
  CctwDoubleMatrix3x3 m_OMat;
  CctwDoubleVector3D  m_OVec;

private:
  CctwDoubleMatrix3x3 m_UBMatInv;
  CctwDoubleMatrix3x3 m_GridBasisInv;
  CctwDoubleMatrix3x3 m_BMatInv;
  CctwDoubleMatrix3x3 m_UMatInv;
  CctwDoubleMatrix3x3 m_GMatInv;
  CctwDoubleMatrix3x3 m_DMatInv;
  CctwDoubleMatrix3x3 m_OMatInv;

  CctwDoubleVector3D m_CD;
  CctwDoubleVector3D m_ND;
};

#endif // CCTWCRYSTALCOORDINATETRANSFORM_H
