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
         0.0, -1.0, 0.0,
         0.0, 0.0, 1.0,
         -1.0, 0.0, 0.0, "o Matrix - rotate detector axes into lab"),
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
  m_ND(m_Application->saver(), this, "nD", CctwDoubleVector3D(), "nD - detector plane normal direction"),
  m_GridDim(m_Application->saver(), this, "gridDim", CctwDoubleVector3D(101,71,71), "Grid Dimensions")
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
  zzz(2,2) = sqrt(clat*clat - zzz(0,2)*zzz(0,2) - zzz(1,2)*zzz(1,2));

  CctwDoubleMatrix3x3 tmat = zzz.inverted();

  return tmat.transposed();
}

CctwDoubleVector3D CctwTransformTest::getDetPos(CctwDoubleVector3D xyz)
{
  return getDetPos(xyz.x(), xyz.y());
}

CctwDoubleVector3D CctwTransformTest::getDetPos(double x, double y)
{
  double pixelSize = get_PixelSize();

  return CctwDoubleVector3D((x+0.5 - get_Det0x())*pixelSize,
                            (y+0.5 - get_Det0y())*pixelSize, 0.0);
}

CctwDoubleVector3D CctwTransformTest::pixel2qlab(double x, double y, double z)
{
  return pixel2qlab(CctwDoubleVector3D(x,y,z));
}

CctwDoubleVector3D CctwTransformTest::pixel2qlab(CctwDoubleVector3D pixel)
{
  CctwDoubleVector3D xyz = pixel;
  double scale = 1.0/get_Wavelength();

  xyz = get_OMatInv()*xyz;
  xyz = get_DMatInv()*xyz;
  xyz = xyz - get_CD();
  xyz = xyz.normalized();
  xyz = xyz*scale;

  xyz.x() -= scale;

  xyz = get_GMatInv()*xyz;

  return xyz;
}

CctwDoubleVector3D CctwTransformTest::qlab2hkl(double qx, double qy, double qz)
{
  return qlab2hkl(CctwDoubleVector3D(qx,qy,qz));
}

CctwDoubleVector3D CctwTransformTest::qlab2hkl(CctwDoubleVector3D qlab)
{
  return get_UBMatInv()*qlab;
}

CctwDoubleVector3D CctwTransformTest::hkl2grid(double h, double k, double l)
{
  return hkl2grid(CctwDoubleVector3D(h,k,l));
}

CctwDoubleVector3D CctwTransformTest::hkl2grid(CctwDoubleVector3D hkl)
{
  CctwDoubleVector3D xyz = hkl;

  xyz = xyz - get_GridOrigin();
  xyz = get_GridBasisInv()*xyz;

  for (int i=0; i<3; i++) {
    xyz(i) *= qMax(get_GridDim()(i)-1,1.0);
  }

  return xyz;
}

CctwDoubleVector3D CctwTransformTest::grid2hkl(double gx, double gy, double gz)
{
  return grid2hkl(CctwDoubleVector3D(gx,gy,gz));
}

CctwDoubleVector3D CctwTransformTest::grid2hkl(CctwDoubleVector3D grid)
{
  CctwDoubleVector3D xyz = grid;

  for (int i=0; i<3; i++) {
    xyz(i) /= qMax(get_GridDim()(i)-1,1.0);
  }

  xyz = get_GridBasis()*xyz;
  xyz = xyz + get_GridOrigin();

  return xyz;
}

CctwDoubleVector3D CctwTransformTest::hkl2qlab(double h, double k, double l)
{
  return hkl2qlab(CctwDoubleVector3D(h,k,l));
}

CctwDoubleVector3D CctwTransformTest::hkl2qlab(CctwDoubleVector3D hkl)
{
  return get_UBMat()*hkl;
}

CctwDoubleVector3D CctwTransformTest::qlab2pixel(double qx, double qy, double qz)
{
  return qlab2pixel(CctwDoubleVector3D(qx,qy,qz));
}

CctwDoubleVector3D CctwTransformTest::qlab2pixel(CctwDoubleVector3D qlab)
{
  CctwDoubleVector3D xyz = qlab;

  xyz = get_GMat()*xyz;
  xyz(0) += 1.0/get_Wavelength();
  xyz = xyz.normalized();

  double dmin   = CctwDoubleVector3D::dotProduct(get_CD(),get_ND());
  double cosang = CctwDoubleVector3D::dotProduct(xyz, get_ND());

  double pdist = -dmin/cosang;

  xyz = xyz*pdist;

  xyz = xyz + get_CD();

  xyz = get_DMat()*xyz;
  xyz = get_OMat()*xyz;

  return xyz;
}

CctwDoubleVector3D CctwTransformTest::getDetPix(double x, double y, double z)
{
  return getDetPix(CctwDoubleVector3D(x,y,z));
}

CctwDoubleVector3D CctwTransformTest::getDetPix(CctwDoubleVector3D xyz)
{
  CctwDoubleVector3D detpix;

  detpix.x() = (int)(xyz.x()/get_PixelSize() + get_Det0x() - 0.5);
  detpix.y() = (int)(xyz.y()/get_PixelSize() + get_Det0y() - 0.5);

  return detpix;
}
