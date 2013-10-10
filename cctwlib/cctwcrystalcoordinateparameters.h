#ifndef CCTWCRYSTALCOORDINATEPARAMETERS_H
#define CCTWCRYSTALCOORDINATEPARAMETERS_H

#ifdef USE_QT
#include "cctwqtobject.h"
#endif

#include "cctwvector3d.h"
#include "cctwmatrix3x3.h"
#include "cctwunitcell.h"

#ifdef USE_QT
class CctwCrystalCoordinateParameters : public CctwqtObject
{
  Q_OBJECT
public:
  CctwCrystalCoordinateParameters(QObject *parent);
#else
class CctwCrystalCoordinateParameters
{
public:
  CctwCrystalCoordinateParameters();
#endif

public:
  double pixelSize() const { return m_PixelSize; }
  double wavelength() const { return m_Wavelength; }
  double distance() const { return m_Distance; }

  CctwUnitCell        unitCell() const { return m_UnitCell; }
  CctwDoubleMatrix3x3 ubMat() const { return m_UBMat; }

  double det0x() const { return m_Det0x; }
  double det0y() const { return m_Det0y; }

  CctwDoubleVector3D xTrans() const { return m_XTrans; }

  double orientErrorDetPitch() const { return m_OrientErrorDetPitch; }
  double orientErrorDetRoll() const { return m_OrientErrorDetRoll; }
  double orientErrorDetYaw() const { return m_OrientErrorDetYaw; }
  double orientErrorGonPitch() const { return m_OrientErrorGonPitch; }

  double twoThetaCorrection() const { return m_TwoThetaCorrection; }
  double twoThetaNom() const { return m_TwoThetaNom; }

  double omegaCorrection() const { return m_OmegaCorrection; }
  double omegaStep() const { return m_OmegaStep; }

  double chiCorrection() const { return m_ChiCorrection; }
  double chiNom() const { return m_ChiNom; }

  double phiCorrection() const { return m_PhiCorrection; }
  double phiNom() const { return m_PhiNom; }

  CctwDoubleVector3D  gridOrigin() const { return m_GridOrigin; }
  CctwDoubleMatrix3x3 gridBasis() const { return m_GridBasis; }
  CctwDoubleVector3D  gridDim() const { return m_GridDim; }

  void setPixelSize(double sz) { m_PixelSize = sz; }
  void setWavelength(double wv) { m_Wavelength = wv; }
  void setDistance(double d) { m_Distance = d; }

  void setUnitCell(CctwUnitCell uc) { m_UnitCell = uc; }
  void setUBMat(CctwDoubleMatrix3x3 ub) { m_UBMat = ub; }

  void setDet0x(double d0x) { m_Det0x = d0x; }
  void setDet0y(double d0y) { m_Det0y = d0y; }

  void setXTrans(CctwDoubleVector3D xt) { m_XTrans = xt; }

  void setOrientErrorDetPitch(double x) { m_OrientErrorDetPitch = x; }
  void setOrientErrorDetRoll(double x) { m_OrientErrorDetRoll = x; }
  void setOrientErrorDetYaw(double x) { m_OrientErrorDetYaw = x; }
  void setOrientErrorGonPitch(double x) { m_OrientErrorGonPitch = x; }

  void setTwoThetaCorrection(double tthc) { m_TwoThetaCorrection = tthc; }
  void setTwoThetaNom(double tthn) { m_TwoThetaNom = tthn; }

  void setOmegaCorrection(double omgc) { m_OmegaCorrection = omgc; }
  void setOmegaStep(double omgs) { m_OmegaStep = omgs; }

  void setChiCorrection(double chic) { m_ChiCorrection = chic; }
  void setChiNom(double chin) { m_ChiNom = chin; }

  void setPhiCorrection(double phic) { m_PhiCorrection = phic; }
  void setPhiNom(double phin) { m_PhiNom = phin; }

  void setGridOrigin(CctwDoubleVector3D org) { m_GridOrigin = org; }
  void setGridBasis(CctwDoubleMatrix3x3 bas) { m_GridBasis = bas; }
  void setGridDim(CctwDoubleVector3D dim) { m_GridDim = dim; }

private:
  double m_PixelSize;
  double m_Wavelength;
  double m_Distance;

  CctwUnitCell        m_UnitCell;
  CctwDoubleMatrix3x3 m_UBMat;

  double m_Det0x;
  double m_Det0y;

  CctwDoubleVector3D m_XTrans;

  double m_OrientErrorDetPitch;
  double m_OrientErrorDetRoll;
  double m_OrientErrorDetYaw;
  double m_OrientErrorGonPitch;

  double m_TwoThetaCorrection;
  double m_TwoThetaNom;

  double m_OmegaCorrection;
  double m_OmegaStep;

  double m_ChiCorrection;
  double m_ChiNom;

  double m_PhiCorrection;
  double m_PhiNom;

  CctwDoubleVector3D  m_GridOrigin;
  CctwDoubleMatrix3x3 m_GridBasis;
  CctwDoubleVector3D  m_GridDim;

#ifdef USE_QT
public:
  Q_PROPERTY(double pixelSize                READ pixelSize    WRITE setPixelSize)
  Q_PROPERTY(double wavelength               READ wavelength   WRITE setWavelength)
  Q_PROPERTY(double distance                 READ distance     WRITE setDistance)

  Q_PROPERTY(CctwUnitCell unitCell           READ unitCell     WRITE setUnitCell)
  Q_PROPERTY(CctwDoubleMatrix3x3 ubMat       READ ubMat        WRITE setUBMat)

  Q_PROPERTY(double det0x                    READ det0x        WRITE setDet0x)
  Q_PROPERTY(double det0y                    READ det0y        WRITE setDet0y)

  Q_PROPERTY(CctwDoubleVector3D xTrans       READ xTrans       WRITE setXTrans)

  Q_PROPERTY(double orientErrorDetPitch      READ orientErrorDetPitch  WRITE setOrientErrorDetPitch)
  Q_PROPERTY(double orientErrorDetRoll       READ orientErrorDetRoll   WRITE setOrientErrorDetRoll)
  Q_PROPERTY(double orientErrorDetYaw        READ orientErrorDetYaw    WRITE setOrientErrorDetYaw)
  Q_PROPERTY(double orientErrorGonPitch      READ orientErrorGonPitch  WRITE setOrientErrorGonPitch)

  Q_PROPERTY(double twoThetaCorrection       READ twoThetaCorrection   WRITE setTwoThetaCorrection)
  Q_PROPERTY(double twoThetaNom              READ twoThetaNom          WRITE setTwoThetaNom)

  Q_PROPERTY(double omegaCorrection          READ omegaCorrection      WRITE setOmegaCorrection)
  Q_PROPERTY(double omegaStep                READ omegaStep            WRITE setOmegaStep)

  Q_PROPERTY(double chiCorrection            READ chiCorrection        WRITE setChiCorrection)
  Q_PROPERTY(double chiNom                   READ chiNom               WRITE setChiNom)

  Q_PROPERTY(double phiCorrection            READ phiCorrection        WRITE setPhiCorrection)
  Q_PROPERTY(double phiNom                   READ phiNom               WRITE setPhiNom)

  Q_PROPERTY(CctwDoubleVector3D  gridOrigin  READ gridOrigin           WRITE setGridOrigin)
  Q_PROPERTY(CctwDoubleMatrix3x3 gridBasis   READ gridBasis            WRITE setGridBasis)
  Q_PROPERTY(CctwDoubleVector3D  gridDim     READ gridDim              WRITE setGridDim)
#endif
};

#endif // CCTWCRYSTALCOORDINATEPARAMETERS_H
