#ifndef CCTWTRANSFORMTEST_H
#define CCTWTRANSFORMTEST_H

#include "cctwobject.h"
#include "cctwdoublematrix3x3property.h"
#include "cctwdoublevector3dproperty.h"
#include "cctwunitcellproperty.h"

class CctwqtApplication;

class CctwTransformTest : public CctwObject
{
  Q_OBJECT
public:
  explicit CctwTransformTest(CctwqtApplication *app, QObject *parent = 0);
  
signals:
  
public slots:
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

public:
  CctwDoubleMatrix3x3 createBMatrix(const CctwUnitCell &cell) const;

private:
  CctwqtApplication *m_Application;

  Q_PROPERTY(double frame READ get_Frame WRITE set_Frame)
  QCEP_DOUBLE_PROPERTY(Frame)

  Q_PROPERTY(double pixelSize READ get_PixelSize WRITE set_PixelSize)
  QCEP_DOUBLE_PROPERTY(PixelSize)

  Q_PROPERTY(double wavelength READ get_Wavelength WRITE set_Wavelength)
  QCEP_DOUBLE_PROPERTY(Wavelength)

  Q_PROPERTY(double distance READ get_Distance WRITE set_Distance)
  QCEP_DOUBLE_PROPERTY(Distance)

  Q_PROPERTY(CctwUnitCell unitCell READ get_UnitCell WRITE set_UnitCell)
  CCTW_UNITCELL_PROPERTY(UnitCell)

  Q_PROPERTY(CctwDoubleMatrix3x3 ubMat READ get_UBMat WRITE set_UBMat)
  CCTW_DOUBLEMATRIX3X3_PROPERTY(UBMat)

  Q_PROPERTY(CctwDoubleMatrix3x3 ubMatInv READ get_UBMatInv WRITE set_UBMatInv)
  CCTW_DOUBLEMATRIX3X3_PROPERTY(UBMatInv)

  Q_PROPERTY(double det0x READ get_Det0x WRITE set_Det0x)
  QCEP_DOUBLE_PROPERTY(Det0x)

  Q_PROPERTY(double det0y READ get_Det0y WRITE set_Det0y)
  QCEP_DOUBLE_PROPERTY(Det0y)

  Q_PROPERTY(CctwDoubleVector3D xtrans READ get_Xtrans WRITE set_Xtrans)
  CCTW_DOUBLEVECTOR3D_PROPERTY(Xtrans)

  Q_PROPERTY(double orientErrorDetPitch READ get_OrientErrorDetPitch WRITE set_OrientErrorDetPitch)
  QCEP_DOUBLE_PROPERTY(OrientErrorDetPitch)

  Q_PROPERTY(double orientErrorDetRoll READ get_OrientErrorDetRoll WRITE set_OrientErrorDetRoll)
  QCEP_DOUBLE_PROPERTY(OrientErrorDetRoll)

  Q_PROPERTY(double orientErrorDetYaw READ get_OrientErrorDetYaw WRITE set_OrientErrorDetYaw)
  QCEP_DOUBLE_PROPERTY(OrientErrorDetYaw)

  Q_PROPERTY(double orientErrorGonPitch READ get_OrientErrorGonPitch WRITE set_OrientErrorGonPitch)
  QCEP_DOUBLE_PROPERTY(OrientErrorGonPitch)

  Q_PROPERTY(double twoThetaCorrection READ get_TwoThetaCorrection WRITE set_TwoThetaCorrection)
  QCEP_DOUBLE_PROPERTY(TwoThetaCorrection)

  Q_PROPERTY(double twoThetaNom READ get_TwoThetaNom WRITE set_TwoThetaNom)
  QCEP_DOUBLE_PROPERTY(TwoThetaNom)

  Q_PROPERTY(double twoThetaAngle READ get_TwoThetaAngle WRITE set_TwoThetaAngle)
  QCEP_DOUBLE_PROPERTY(TwoThetaAngle)

  Q_PROPERTY(double omegaCorrection READ get_OmegaCorrection WRITE set_OmegaCorrection)
  QCEP_DOUBLE_PROPERTY(OmegaCorrection)

  Q_PROPERTY(double omegaStep READ get_OmegaStep WRITE set_OmegaStep)
  QCEP_DOUBLE_PROPERTY(OmegaStep)

  Q_PROPERTY(double omegaNom READ get_OmegaNom WRITE set_OmegaNom)
  QCEP_DOUBLE_PROPERTY(OmegaNom)

  Q_PROPERTY(double omegaAngle READ get_OmegaAngle WRITE set_OmegaAngle)
  QCEP_DOUBLE_PROPERTY(OmegaAngle)

  Q_PROPERTY(double chiCorrection READ get_ChiCorrection WRITE set_ChiCorrection)
  QCEP_DOUBLE_PROPERTY(ChiCorrection)

  Q_PROPERTY(double chiNom READ get_ChiNom WRITE set_ChiNom)
  QCEP_DOUBLE_PROPERTY(ChiNom)

  Q_PROPERTY(double chiAngle READ get_ChiAngle WRITE set_ChiAngle)
  QCEP_DOUBLE_PROPERTY(ChiAngle)

  Q_PROPERTY(double phiCorrection READ get_PhiCorrection WRITE set_PhiCorrection)
  QCEP_DOUBLE_PROPERTY(PhiCorrection)

  Q_PROPERTY(double phiNom READ get_PhiNom WRITE set_PhiNom)
  QCEP_DOUBLE_PROPERTY(PhiNom)

  Q_PROPERTY(double phiAngle READ get_PhiAngle WRITE set_PhiAngle)
  QCEP_DOUBLE_PROPERTY(PhiAngle)

  Q_PROPERTY(CctwDoubleVector3D  gridOrigin READ get_GridOrigin WRITE set_GridOrigin)
  CCTW_DOUBLEVECTOR3D_PROPERTY(GridOrigin)

  Q_PROPERTY(CctwDoubleMatrix3x3 gridBasis READ get_GridBasis WRITE set_GridBasis)
  CCTW_DOUBLEMATRIX3X3_PROPERTY(GridBasis)

  Q_PROPERTY(CctwDoubleMatrix3x3 gridBasisInv READ get_GridBasisInv WRITE set_GridBasisInv)
  CCTW_DOUBLEMATRIX3X3_PROPERTY(GridBasisInv)

  Q_PROPERTY(CctwDoubleMatrix3x3 oMat READ get_OMat WRITE set_OMat)
  CCTW_DOUBLEMATRIX3X3_PROPERTY(OMat)

  Q_PROPERTY(CctwDoubleMatrix3x3 oMatInv READ get_OMatInv WRITE set_OMatInv)
  CCTW_DOUBLEMATRIX3X3_PROPERTY(OMatInv)

  Q_PROPERTY(CctwDoubleMatrix3x3 dMat READ get_DMat WRITE set_DMat)
  CCTW_DOUBLEMATRIX3X3_PROPERTY(DMat)

  Q_PROPERTY(CctwDoubleMatrix3x3 dMatInv READ get_DMatInv WRITE set_DMatInv)
  CCTW_DOUBLEMATRIX3X3_PROPERTY(DMatInv)

  Q_PROPERTY(CctwDoubleMatrix3x3 uMat READ get_UMat WRITE set_UMat)
  CCTW_DOUBLEMATRIX3X3_PROPERTY(UMat)

  Q_PROPERTY(CctwDoubleMatrix3x3 uMatInv READ get_UMatInv WRITE set_UMatInv)
  CCTW_DOUBLEMATRIX3X3_PROPERTY(UMatInv)

  Q_PROPERTY(CctwDoubleMatrix3x3 bMat READ get_BMat WRITE set_BMat)
  CCTW_DOUBLEMATRIX3X3_PROPERTY(BMat)

  Q_PROPERTY(CctwDoubleMatrix3x3 bMatInv READ get_BMatInv WRITE set_BMatInv)
  CCTW_DOUBLEMATRIX3X3_PROPERTY(BMatInv)

  Q_PROPERTY(CctwDoubleMatrix3x3 gMat READ get_GMat WRITE set_GMat)
  CCTW_DOUBLEMATRIX3X3_PROPERTY(GMat)

  Q_PROPERTY(CctwDoubleMatrix3x3 gMatInv READ get_GMatInv WRITE set_GMatInv)
  CCTW_DOUBLEMATRIX3X3_PROPERTY(GMatInv)

  Q_PROPERTY(CctwDoubleVector3D cD READ get_CD WRITE set_CD)
  CCTW_DOUBLEVECTOR3D_PROPERTY(CD)

  Q_PROPERTY(CctwDoubleVector3D nD READ get_ND WRITE set_ND)
  CCTW_DOUBLEVECTOR3D_PROPERTY(ND)

  Q_PROPERTY(CctwDoubleVector3D gridDim READ get_GridDim WRITE set_GridDim)
  CCTW_DOUBLEVECTOR3D_PROPERTY(GridDim)
};

#endif // CCTWTRANSFORMTEST_H
