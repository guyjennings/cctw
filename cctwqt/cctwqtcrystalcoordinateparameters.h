#ifndef CCTWQTCRYSTALCOORDINATEPARAMETERS_H
#define CCTWQTCRYSTALCOORDINATEPARAMETERS_H

#include "cctwqtobject.h"
#include "../cctwcrystalcoordinateparameters.h"
#include "cctwqtunitcellproperty.h"
#include "cctwqtdoublematrix3x3property.h"
#include "cctwqtdoublevector3dproperty.h"

class CctwqtApplication;

class CctwqtCrystalCoordinateParameters : public CctwCrystalCoordinateParameters
{
  Q_OBJECT
public:
  explicit CctwqtCrystalCoordinateParameters(CctwqtApplication *app, QObject *parent=0);
  typedef CctwCrystalCoordinateParameters inherited;

signals:
  
public slots:
  void updateParameters();

private:
  CctwqtApplication *m_Application;

  Q_PROPERTY(double pixelSize READ get_PixelSize WRITE set_PixelSize)
  QCEP_DOUBLE_PROPERTY(PixelSize)

  Q_PROPERTY(double wavelength READ get_Wavelength WRITE set_Wavelength)
  QCEP_DOUBLE_PROPERTY(Wavelength)

  Q_PROPERTY(double distance READ get_Distance WRITE set_Distance)
  QCEP_DOUBLE_PROPERTY(Distance)

  Q_PROPERTY(CctwUnitCell unitCell READ get_UnitCell WRITE set_UnitCell)
  CCTWQT_UNITCELL_PROPERTY(UnitCell)

  Q_PROPERTY(CctwDoubleMatrix3x3 ubMat READ get_UBMat WRITE set_UBMat)
  CCTWQT_DOUBLEMATRIX3X3_PROPERTY(UBMat)

  Q_PROPERTY(double det0x READ get_Det0x WRITE set_Det0x)
  QCEP_DOUBLE_PROPERTY(Det0x)

  Q_PROPERTY(double det0y READ get_Det0y WRITE set_Det0y)
  QCEP_DOUBLE_PROPERTY(Det0y)

  Q_PROPERTY(CctwDoubleVector3D xTrans READ get_XTrans WRITE set_XTrans)
  CCTWQT_DOUBLEVECTOR3D_PROPERTY(XTrans)

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

  Q_PROPERTY(double omegaCorrection READ get_OmegaCorrection WRITE set_OmegaCorrection)
  QCEP_DOUBLE_PROPERTY(OmegaCorrection)

  Q_PROPERTY(double omegaStep READ get_OmegaStep WRITE set_OmegaStep)
  QCEP_DOUBLE_PROPERTY(OmegaStep)

  Q_PROPERTY(double chiCorrection READ get_ChiCorrection WRITE set_ChiCorrection)
  QCEP_DOUBLE_PROPERTY(ChiCorrection)

  Q_PROPERTY(double chiNom READ get_ChiNom WRITE set_ChiNom)
  QCEP_DOUBLE_PROPERTY(ChiNom)

  Q_PROPERTY(double phiCorrection READ get_PhiCorrection WRITE set_PhiCorrection)
  QCEP_DOUBLE_PROPERTY(PhiCorrection)

  Q_PROPERTY(double phiNom READ get_PhiNom WRITE set_PhiNom)
  QCEP_DOUBLE_PROPERTY(PhiNom)

  Q_PROPERTY(CctwDoubleVector3D  gridOrigin READ get_GridOrigin WRITE set_GridOrigin)
  CCTWQT_DOUBLEVECTOR3D_PROPERTY(GridOrigin)

  Q_PROPERTY(CctwDoubleMatrix3x3 gridBasis READ get_GridBasis WRITE set_GridBasis)
  CCTWQT_DOUBLEMATRIX3X3_PROPERTY(GridBasis)

  Q_PROPERTY(CctwDoubleVector3D gridDim READ get_GridDim WRITE set_GridDim)
  CCTWQT_DOUBLEVECTOR3D_PROPERTY(GridDim)
};

#endif // CCTWQTCRYSTALCOORDINATEPARAMETERS_H
