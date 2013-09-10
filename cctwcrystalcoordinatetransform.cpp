#include "cctwcrystalcoordinatetransform.h"
#include <cmath>

#ifdef USE_QT
CctwCrystalCoordinateTransform::CctwCrystalCoordinateTransform(CctwCrystalCoordinateParameters *parms, QObject *parent) :
  CctwqtObject(parent),
  m_Parms(parms),
  m_CurrentFrame(-1),
  m_CurrentFrameChangeCount(0)
{
  updateFromParameters();
  setCurrentFrame(0.0);
}
#else
CctwCrystalCoordinateTransform::CctwCrystalCoordinateTransform(CctwCrystalCoordinateParameters *parms) :
  m_Parms(parms),
  m_CurrentFrame(-1),
  m_CurrentFrameChangeCount(0)
{
  updateFromParameters();
  setCurrentFrame(0.0);
}
#endif

void CctwCrystalCoordinateTransform::updateFromParameters()
{
  m_TwoThetaAngle = m_Parms->m_TwoThetaNom + m_Parms->m_TwoThetaCorrection;
  m_PhiAngle      = m_Parms->m_PhiNom + m_Parms->m_PhiCorrection;
  m_ChiAngle      = m_Parms->m_ChiNom + m_Parms->m_ChiCorrection;

  m_GridBasisInv  = m_Parms->m_GridBasis.inverted();
  m_OMat          = CctwDoubleMatrix3x3(1.0, 0.0, 0.0,
                                        0.0, 0.0, 1.0,
                                        -1.0, 0.0, 0.0);

  m_OMatInv       = m_OMat.inverted();

  m_UBMatInv      = m_Parms->m_UBMat.inverted();

  m_BMat          = createBMatrix(m_Parms->m_UnitCell);
  m_BMatInv       = m_BMat.inverted();

  m_UMat          = m_Parms->m_UBMat * m_BMatInv;
  m_UMatInv       = m_UMat.inverted();
}

void CctwCrystalCoordinateTransform::setCurrentFrame(double frame)
{
  if (frame != m_CurrentFrame) {
    m_CurrentFrame = frame;

    m_OmegaAngle = frame*m_Parms->m_OmegaStep + m_Parms->m_OmegaCorrection;

    CctwDoubleMatrix3x3 dimat = CctwDoubleMatrix3x3::identity();

    dimat = CctwDoubleMatrix3x3::rotZ(m_Parms->m_OrientErrorDetYaw)*dimat;
    dimat = CctwDoubleMatrix3x3::rotY(m_Parms->m_OrientErrorDetPitch)*dimat;
    dimat = CctwDoubleMatrix3x3::rotX(m_Parms->m_OrientErrorDetRoll)*dimat;
    dimat = CctwDoubleMatrix3x3::rotZ(m_TwoThetaAngle)*dimat;
    dimat = CctwDoubleMatrix3x3::rotY(m_Parms->m_OrientErrorGonPitch)*dimat;

    m_DMatInv = dimat;
    m_DMat    = dimat.inverted();

    CctwDoubleMatrix3x3 gmat = CctwDoubleMatrix3x3::identity();

    gmat = CctwDoubleMatrix3x3::rotZ(m_PhiAngle)*gmat;
    gmat = CctwDoubleMatrix3x3::rotX(m_ChiAngle)*gmat;
    gmat = CctwDoubleMatrix3x3::rotZ(m_OmegaAngle)*gmat;
    gmat = CctwDoubleMatrix3x3::rotY(m_Parms->m_OrientErrorGonPitch)*gmat;

    m_GMat    = gmat;
    m_GMatInv = gmat.inverted();

    CctwDoubleVector3D cD(m_Parms->m_Distance, 0, 0);
    cD = CctwDoubleMatrix3x3::rotZ(m_TwoThetaAngle)*cD;
    cD = CctwDoubleMatrix3x3::rotY(m_Parms->m_OrientErrorGonPitch)*cD;
    CctwDoubleVector3D tVec = m_GMat*m_Parms->m_XTrans;

    cD = tVec - cD;

    CctwDoubleVector3D nD(dimat(0,0), dimat(1,0), dimat(2,0));

    m_CD = cD;
    m_ND = nD;

    m_CurrentFrameChangeCount++;
  }
}

CctwDoubleMatrix3x3 CctwCrystalCoordinateTransform::createBMatrix(const CctwUnitCell &cell) const
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

bool CctwCrystalCoordinateTransform::hasInverse() const
{
  return false;
}

//CctwCrystalCoordinateTransform *CctwCrystalCoordinateTransform::createNew(int argc, char *argv[])
//{
//  return new CctwCrystalCoordinateTransform();
//}

CctwDoubleVector3D CctwCrystalCoordinateTransform::forward(CctwDoubleVector3D a)
{
  setCurrentFrame(a.z());

  return hkl2grid(qlab2hkl(pixel2qlab(getDetPos(a.x(), a.y()))));
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::inverse(CctwDoubleVector3D q)
{
  return CctwDoubleVector3D();
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::getDetPos(CctwDoubleVector3D xyz)
{
  return getDetPos(xyz.x(), xyz.y());
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::getDetPos(double x, double y)
{
  double pixelSize = m_Parms->m_PixelSize;

  return CctwDoubleVector3D((x+0.5 - m_Parms->m_Det0x)*pixelSize,
                            (y+0.5 - m_Parms->m_Det0y)*pixelSize, 0.0);
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::pixel2qlab(double x, double y, double z)
{
  return pixel2qlab(CctwDoubleVector3D(x,y,z));
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::pixel2qlab(CctwDoubleVector3D pixel)
{
  CctwDoubleVector3D xyz = pixel;
  double scale = 1.0/m_Parms->m_Wavelength;

  xyz = m_OMatInv*xyz;
  xyz = m_DMatInv*xyz;
  xyz = xyz - m_CD;
  xyz = xyz.normalized();
  xyz = xyz*scale;

  xyz.x() -= scale;

  xyz = m_GMatInv*xyz;

  return xyz;
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::qlab2hkl(double qx, double qy, double qz)
{
  return qlab2hkl(CctwDoubleVector3D(qx,qy,qz));
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::qlab2hkl(CctwDoubleVector3D qlab)
{
  return m_UBMatInv*qlab;
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::hkl2grid(double h, double k, double l)
{
  return hkl2grid(CctwDoubleVector3D(h,k,l));
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::hkl2grid(CctwDoubleVector3D hkl)
{
  CctwDoubleVector3D xyz = hkl;

  xyz = xyz - m_Parms->m_GridOrigin;
  xyz = m_GridBasisInv*xyz;

  for (int i=0; i<3; i++) {
    xyz(i) *= qMax(m_Parms->m_GridDim(i)-1,1.0);
  }

  return xyz;
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::grid2hkl(double gx, double gy, double gz)
{
  return grid2hkl(CctwDoubleVector3D(gx,gy,gz));
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::grid2hkl(CctwDoubleVector3D grid)
{
  CctwDoubleVector3D xyz = grid;

  for (int i=0; i<3; i++) {
    xyz(i) /= qMax(m_Parms->m_GridDim(i)-1,1.0);
  }

  xyz = m_Parms->m_GridBasis*xyz;
  xyz = xyz + m_Parms->m_GridOrigin;

  return xyz;
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::hkl2qlab(double h, double k, double l)
{
  return hkl2qlab(CctwDoubleVector3D(h,k,l));
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::hkl2qlab(CctwDoubleVector3D hkl)
{
  return m_Parms->m_UBMat*hkl;
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::qlab2pixel(double qx, double qy, double qz)
{
  return qlab2pixel(CctwDoubleVector3D(qx,qy,qz));
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::qlab2pixel(CctwDoubleVector3D qlab)
{
  CctwDoubleVector3D xyz = qlab;

  xyz = m_GMat*xyz;
  xyz(0) += 1.0/m_Parms->m_Wavelength;
  xyz = xyz.normalized();

  double dmin   = CctwDoubleVector3D::dotProduct(m_CD,m_ND);
  double cosang = CctwDoubleVector3D::dotProduct(xyz, m_ND);

  double pdist = -dmin/cosang;

  xyz = xyz*pdist;

  xyz = xyz + m_CD;

  xyz = m_DMat*xyz;
  xyz = m_OMat*xyz;

  return xyz;
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::getDetPix(double x, double y, double z)
{
  return getDetPix(CctwDoubleVector3D(x,y,z));
}

CctwDoubleVector3D CctwCrystalCoordinateTransform::getDetPix(CctwDoubleVector3D xyz)
{
  CctwDoubleVector3D detpix;

  detpix.x() = (int)(xyz.x()/m_Parms->m_PixelSize + m_Parms->m_Det0x - 0.5);
  detpix.y() = (int)(xyz.y()/m_Parms->m_PixelSize + m_Parms->m_Det0y - 0.5);

  return detpix;
}
