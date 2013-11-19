#ifndef CCTWCRYSTALCOORDINATETRANSFORM_H
#define CCTWCRYSTALCOORDINATETRANSFORM_H

#include "cctwtransforminterface.h"
#include "cctwmatrix3x3.h"
#include "cctwcrystalcoordinateparameters.h"

#ifdef USE_QT
#include "cctwobject.h"
#endif

class CctwCrystalCoordinateTransform : public CctwTransformInterface
{
#ifdef USE_QT
  Q_OBJECT
public:
  CctwCrystalCoordinateTransform(CctwCrystalCoordinateParameters *parms, QObject *parent);
#else
public:
  CctwCrystalCoordinateTransform(CctwCrystalCoordinateParameters *parms);
#endif

#ifdef USE_QT
public slots:
#else
public:
#endif

//  static CctwCrystalCoordinateTransform* createNew(int argc, char *argv[]);
//                                          // Allocate and return a new crystal coordinate transformation object
//                                          // based on the command line parameters passed in argc and argv

  virtual bool hasInverse() const;

  virtual CctwDoubleVector3D forward(CctwDoubleVector3D a);
                                          // Transform from input to output space coordinates
  virtual CctwDoubleVector3D inverse(CctwDoubleVector3D q);
                                          // Back transform from output to input space coordinates

  void updateFromParameters();
  void setCurrentFrame(double frame);

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

private:
  CctwCrystalCoordinateParameters *m_Parms;

  double m_CurrentFrame;

protected:
  int    m_CurrentFrameChangeCount;

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
