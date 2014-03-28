#include "cctwcrystalcoordinateparameters.h"

CctwCrystalCoordinateParameters::CctwCrystalCoordinateParameters(QString name, QObject *parent) :
  CctwObject(name, parent)
{
  setDefaults();
}

void CctwCrystalCoordinateParameters::setDefaults()
{
  setPixelSize(0.080567);
  setWavelength(0.153870);
  setDistance(365.97);

  setUnitCell(CctwUnitCell(4.04, 4.04, 4.04, 1.570796, 1.570796, 1.570796));

  setUBMat(CctwDoubleMatrix3x3(-0.001955, 0.177126, -0.172890,
                               -0.247516, -0.000885, 0.001892,
                               0.000736, 0.172899, 0.177127));

  setDet0x(1041.266200);
  setDet0y(989.624800);

  setXTrans(CctwDoubleVector3D(0.005277, -0.006373, 0.034958));

  setOrientErrorDetPitch(-0.014305);
  setOrientErrorDetRoll(-0.008063);
  setOrientErrorDetYaw(-0.000361);
  setOrientErrorGonPitch(-0.000000);

  setTwoThetaCorrection(0.0);
  setTwoThetaNom(0.0);

  setOmegaCorrection(0.003777);
  setOmegaStep(0.008727);

  setChiCorrection(-0.005531);
  setChiNom(0.0);

  setPhiCorrection(0.0);
  setPhiNom(0.0);

  setGridOrigin(CctwDoubleVector3D(1.5, -0.5, 0.0));
  setGridBasis(CctwDoubleMatrix3x3(1.0, 0.0, 0.0,
                                   0.0, 0.5, 0.5,
                                   0.0, 0.5, -0.5));
  setGridDim(CctwDoubleVector3D(101,71,71));

  setGridOffset(CctwDoubleVector3D(1024,1024,1024));
}

double CctwCrystalCoordinateParameters::pixelSize() const { return m_PixelSize; }
double CctwCrystalCoordinateParameters::wavelength() const { return m_Wavelength; }
double CctwCrystalCoordinateParameters::distance() const { return m_Distance; }

CctwUnitCell        CctwCrystalCoordinateParameters::unitCell() const { return m_UnitCell; }
CctwDoubleMatrix3x3 CctwCrystalCoordinateParameters::ubMat() const { return m_UBMat; }

double CctwCrystalCoordinateParameters::det0x() const { return m_Det0x; }
double CctwCrystalCoordinateParameters::det0y() const { return m_Det0y; }

CctwDoubleVector3D CctwCrystalCoordinateParameters::xTrans() const { return m_XTrans; }

double CctwCrystalCoordinateParameters::orientErrorDetPitch() const { return m_OrientErrorDetPitch; }
double CctwCrystalCoordinateParameters::orientErrorDetRoll() const { return m_OrientErrorDetRoll; }
double CctwCrystalCoordinateParameters::orientErrorDetYaw() const { return m_OrientErrorDetYaw; }
double CctwCrystalCoordinateParameters::orientErrorGonPitch() const { return m_OrientErrorGonPitch; }

double CctwCrystalCoordinateParameters::twoThetaCorrection() const { return m_TwoThetaCorrection; }
double CctwCrystalCoordinateParameters::twoThetaNom() const { return m_TwoThetaNom; }

double CctwCrystalCoordinateParameters::omegaCorrection() const { return m_OmegaCorrection; }
double CctwCrystalCoordinateParameters::omegaStep() const { return m_OmegaStep; }

double CctwCrystalCoordinateParameters::chiCorrection() const { return m_ChiCorrection; }
double CctwCrystalCoordinateParameters::chiNom() const { return m_ChiNom; }

double CctwCrystalCoordinateParameters::phiCorrection() const { return m_PhiCorrection; }
double CctwCrystalCoordinateParameters::phiNom() const { return m_PhiNom; }

CctwDoubleVector3D  CctwCrystalCoordinateParameters::gridOrigin() const { return m_GridOrigin; }
CctwDoubleMatrix3x3 CctwCrystalCoordinateParameters::gridBasis() const { return m_GridBasis; }
CctwDoubleVector3D  CctwCrystalCoordinateParameters::gridDim() const { return m_GridDim; }
CctwDoubleVector3D  CctwCrystalCoordinateParameters::gridOffset() const { return m_GridOffset; }

void CctwCrystalCoordinateParameters::setPixelSize(double sz) { m_PixelSize = sz; }
void CctwCrystalCoordinateParameters::setWavelength(double wv) { m_Wavelength = wv; }
void CctwCrystalCoordinateParameters::setDistance(double d) { m_Distance = d; }

void CctwCrystalCoordinateParameters::setUnitCell(CctwUnitCell uc) { m_UnitCell = uc; }
void CctwCrystalCoordinateParameters::setUBMat(CctwDoubleMatrix3x3 ub) { m_UBMat = ub; }

void CctwCrystalCoordinateParameters::setDet0x(double d0x) { m_Det0x = d0x; }
void CctwCrystalCoordinateParameters::setDet0y(double d0y) { m_Det0y = d0y; }

void CctwCrystalCoordinateParameters::setXTrans(CctwDoubleVector3D xt) { m_XTrans = xt; }

void CctwCrystalCoordinateParameters::setOrientErrorDetPitch(double x) { m_OrientErrorDetPitch = x; }
void CctwCrystalCoordinateParameters::setOrientErrorDetRoll(double x) { m_OrientErrorDetRoll = x; }
void CctwCrystalCoordinateParameters::setOrientErrorDetYaw(double x) { m_OrientErrorDetYaw = x; }
void CctwCrystalCoordinateParameters::setOrientErrorGonPitch(double x) { m_OrientErrorGonPitch = x; }

void CctwCrystalCoordinateParameters::setTwoThetaCorrection(double tthc) { m_TwoThetaCorrection = tthc; }
void CctwCrystalCoordinateParameters::setTwoThetaNom(double tthn) { m_TwoThetaNom = tthn; }

void CctwCrystalCoordinateParameters::setOmegaCorrection(double omgc) { m_OmegaCorrection = omgc; }
void CctwCrystalCoordinateParameters::setOmegaStep(double omgs) { m_OmegaStep = omgs; }

void CctwCrystalCoordinateParameters::setChiCorrection(double chic) { m_ChiCorrection = chic; }
void CctwCrystalCoordinateParameters::setChiNom(double chin) { m_ChiNom = chin; }

void CctwCrystalCoordinateParameters::setPhiCorrection(double phic) { m_PhiCorrection = phic; }
void CctwCrystalCoordinateParameters::setPhiNom(double phin) { m_PhiNom = phin; }

void CctwCrystalCoordinateParameters::setGridOrigin(CctwDoubleVector3D org) { m_GridOrigin = org; }
void CctwCrystalCoordinateParameters::setGridBasis(CctwDoubleMatrix3x3 bas) { m_GridBasis = bas; }
void CctwCrystalCoordinateParameters::setGridDim(CctwDoubleVector3D dim) { m_GridDim = dim; }
void CctwCrystalCoordinateParameters::setGridOffset(CctwDoubleVector3D off) { m_GridOffset = off; }

