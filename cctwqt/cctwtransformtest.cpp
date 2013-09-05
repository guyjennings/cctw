#include "cctwtransformtest.h"
#include "cctwqtapplication.h"
#include <math.h>

CctwTransformTest::CctwTransformTest(CctwqtApplication *app, QObject *parent) :
  CctwqtObject(parent),
  m_Application(app),
  m_Frame(m_Application->saver(), this, "frame", 5, "Frame Number"),
  m_PixelSize(m_Application->saver(), this, "pixelSize", 0.080567, "Detector Pixel Size (mm)"),
  m_Wavelength(m_Application->saver(), this, "wavelength", 0.153870, "Wavelength (Angstrom)"),
  m_Distance(m_Application->saver(), this, "distance", 365.97, "Sample->Detector Distance (mm)"),

  m_UnitCell(m_Application->saver(), this, "unitCell",
             4.04, 4.04, 4.04, 1.570796, 1.570796, 1.570796,
             "Sample Unit Cell (Angstrom,Rad)"),

  m_UBMat(m_Application->saver(), this, "ubMat",
          -0.001955, 0.177126, -0.172890,
          -0.247516, -0.000885, 0.001892,
          0.000736, 0.172899, 0.177127,
          "UB Matrix"),
  m_UBMatInv(m_Application->saver(), this, "ubMat", get_UBMat().inverted(), "UB Matrix Inverse"),

  m_Det0x(m_Application->saver(), this, "det0x", 1041.266200, "X beam center (mm)"),
  m_Det0y(m_Application->saver(), this, "det0y", 989.624800, "Y beam center (mm)"),

  m_Xtrans(m_Application->saver(), this, "xtrans",
           0.005277, -0.006373, 0.034958, "XTrans (mm) goniometer head frame"),

  m_OrientErrorDetPitch(m_Application->saver(), this, "orientErrorDetPitch",
                        -0.014305, "Det Pitch Orientation Error (rad)"),
  m_OrientErrorDetRoll(m_Application->saver(), this, "orientErrorDetRoll",
                        -0.008063, "Det Roll Orientation Error (rad)"),
  m_OrientErrorDetYaw(m_Application->saver(), this, "orientErrorDetYaw",
                        -0.000361, "Det Yaw Orientation Error (rad)"),
  m_OrientErrorGonPitch(m_Application->saver(), this, "orientErrorGonPitch",
                        -0.000000, "Gon Pitch Orientation Error (rad)"),

  m_TwoThetaCorrection(m_Application->saver(), this, "twoThetaCorrection", 0.0, "Two Theta Correction (rad)"),
  m_TwoThetaNom(m_Application->saver(), this, "twoThetaNom", 0.0, "Two Theta Nominal (rad)"),
  m_TwoThetaAngle(m_Application->saver(), this, "twoThetaAngle", get_TwoThetaNom()+get_TwoThetaCorrection(), "Two Theta Angle (rad)"),

  m_OmegaCorrection(m_Application->saver(), this, "omegaCorrection", 0.003777, "Omega Correction (rad)"),
  m_OmegaStep(m_Application->saver(), this, "omegaStep", 0.008727, "Omega Step (rad)"),
  m_OmegaNom(m_Application->saver(), this, "omegaNom", (get_Frame()-0.5)*get_OmegaStep(), "Omega Nominal (rad)"),
  m_OmegaAngle(m_Application->saver(), this, "omegaAngle", get_OmegaNom()+get_OmegaCorrection(), "Omega Angle (rad)"),

  m_ChiCorrection(m_Application->saver(), this, "chiCorrection", -0.005531, "Chi Correction (rad)"),
  m_ChiNom(m_Application->saver(), this, "chiNom", 0.0, "Chi Nominal (rad)"),
  m_ChiAngle(m_Application->saver(), this, "chiAngle", get_ChiNom()+get_ChiCorrection(), "Chi Angle (rad)"),

  m_PhiCorrection(m_Application->saver(), this, "phiCorrection", 0.0, "Phi Correction (rad)"),
  m_PhiNom(m_Application->saver(), this, "phiNom", 0.0, "Phi Nominal (rad)"),
  m_PhiAngle(m_Application->saver(), this, "phiAngle", get_PhiNom()+get_PhiCorrection(), "Phi Angle (rad)"),

  m_GridOrigin(m_Application->saver(), this, "gridOrigin",
               1.5, -0.5, 0.0, "Grid Origin (recip lattice units)"),
  m_GridBasis(m_Application->saver(), this, "gridBasis",
              1.0, 0.0, 0.0,
              0.0, 0.5, 0.5,
              0.0, 0.5, -0.5, "Grid Basis (recip latt units)"),
  m_GridBasisInv(m_Application->saver(), this, "gridBasisInv", get_GridBasis().inverted(), "Grid Basis Inverse"),
  m_OMat(m_Application->saver(), this, "oMat",
         0.0, 0.0, -1.0,
         -1.0, 0.0, 0.0,
         0.0, 1.0, 0.0, "o Matrix - rotate detector axes into lab"),
  m_OMatInv(m_Application->saver(), this, "oMatInv", get_OMat().inverted(), "o Matrix Inverse"),
  m_DMat(m_Application->saver(), this, "dMat", CctwDoubleMatrix3x3(), "d Matrix - lab -> detector coords"),
  m_DMatInv(m_Application->saver(), this, "dMatInv", CctwDoubleMatrix3x3(), "d Matrix Inverse"),
  m_UMat(m_Application->saver(), this, "uMat", CctwDoubleMatrix3x3(), "u Matrix"),
  m_UMatInv(m_Application->saver(), this, "uMatInv", CctwDoubleMatrix3x3(), "u Matrix Inverse"),
  m_BMat(m_Application->saver(), this, "bMat", CctwDoubleMatrix3x3(), "b Matrix"),
  m_BMatInv(m_Application->saver(), this, "bMatInv", CctwDoubleMatrix3x3(), "b Matrix Inverse"),
  m_GMat(m_Application->saver(), this, "gMat", CctwDoubleMatrix3x3(), "g Matrix"),
  m_GMatInv(m_Application->saver(), this, "gMatInv", CctwDoubleMatrix3x3(), "g Matrix Inverse"),
  m_CD(m_Application->saver(), this, "cD", CctwDoubleVector3D(), "cD - crystal position from detector in lab frame"),
  m_ND(m_Application->saver(), this, "nD", CctwDoubleVector3D(), "nD - detector plane normal direction")
{
  CctwDoubleMatrix3x3 dimat = CctwDoubleMatrix3x3::identity();

  dimat = CctwDoubleMatrix3x3::rotZ(get_OrientErrorDetYaw())*dimat;
  dimat = CctwDoubleMatrix3x3::rotY(get_OrientErrorDetPitch())*dimat;
  dimat = CctwDoubleMatrix3x3::rotX(get_OrientErrorDetRoll())*dimat;
  dimat = CctwDoubleMatrix3x3::rotZ(get_TwoThetaAngle())*dimat;
  dimat = CctwDoubleMatrix3x3::rotY(get_OrientErrorGonPitch())*dimat;

  set_DMatInv(dimat);
  set_DMat(dimat.inverted());

  CctwDoubleMatrix3x3 gmat = CctwDoubleMatrix3x3::identity();

  gmat = CctwDoubleMatrix3x3::rotZ(get_PhiAngle())*gmat;
  gmat = CctwDoubleMatrix3x3::rotX(get_ChiAngle())*gmat;
  gmat = CctwDoubleMatrix3x3::rotZ(get_OmegaAngle())*gmat;
  gmat = CctwDoubleMatrix3x3::rotY(get_OrientErrorGonPitch())*gmat;

  set_GMat(gmat);
  set_GMatInv(gmat.inverted());

  set_UBMatInv(get_UBMat().inverted());

  set_BMat(createBMatrix(get_UnitCell()));
  set_BMatInv(get_BMat().inverted());

  set_UMat(get_UBMat()*get_BMatInv());
  set_UMatInv(get_UMat().inverted());

  CctwDoubleVector3D cD(get_Distance(), 0, 0);
  cD = CctwDoubleMatrix3x3::rotZ(get_TwoThetaAngle())*cD;
  cD = CctwDoubleMatrix3x3::rotY(get_OrientErrorGonPitch())*cD;
  CctwDoubleVector3D tVec = get_GMat()*get_Xtrans();

  cD = tVec - cD;

  CctwDoubleVector3D nD(dimat(0,0), dimat(1,0), dimat(2,0));

  set_CD(cD);
  set_ND(nD);
}

CctwDoubleMatrix3x3 CctwTransformTest::createBMatrix(const CctwUnitCell &cell) const
{
  double alat = cell.a();
  double blat = cell.b();
  double clat = cell.c();
  double alpha = cell.alpha();
  double beta = cell.beta();
  double gamma = cell.gamma();

  CctwDoubleMatrix3x3 zzz;

  zzz(0,0) = alat;
  zzz(1,0) = 0;
  zzz(2,0) = 0;

  zzz(0,1) = blat*cos(gamma);
  zzz(1,1) = blat*sin(gamma);
  zzz(2,1) = 0;

  zzz(0,2) = clat*cos(beta);
  zzz(1,2) = clat*(cos(alpha) - cos(beta)*cos(gamma))/sin(gamma);
  zzz(1,3) = sqrt(clat*clat - zzz(0,2)*zzz(0,2) - zzz(1,2)*zzz(1,2));

  CctwDoubleMatrix3x3 tmat = zzz.inverted();

  return tmat.transposed();
}
