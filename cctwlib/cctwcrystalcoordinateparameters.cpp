#include "cctwcrystalcoordinateparameters.h"

CctwCrystalCoordinateParameters::CctwCrystalCoordinateParameters(QObject *parent) :
  CctwObject(parent)
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
}
