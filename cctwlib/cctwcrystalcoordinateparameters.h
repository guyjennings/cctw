#ifndef CCTWCRYSTALCOORDINATEPARAMETERS_H
#define CCTWCRYSTALCOORDINATEPARAMETERS_H

#include "qcepproperty.h"
#include "cctwobject.h"
#include "cctwvector3d.h"
#include "cctwmatrix3x3.h"
#include "cctwunitcell.h"

class CctwCrystalCoordinateParameters : public CctwObject
{
  Q_OBJECT
public:
  CctwCrystalCoordinateParameters(QString name, QcepObjectWPtr parent);

signals:
  void parametersChanged();

public slots:
  void setDefaults();

public:
  double pixelSize() const;
  double wavelength() const;
  double distance() const;

  CctwUnitCell        unitCell() const;
  CctwDoubleMatrix3x3 ubMat() const;
  CctwDoubleMatrix3x3 oMat() const;
  CctwDoubleVector3D  oVec() const;

  double det0x() const;
  double det0y() const;

  CctwDoubleVector3D xTrans() const;

  double orientErrorDetPitch() const;
  double orientErrorDetRoll() const;
  double orientErrorDetYaw() const;
  double orientErrorGonPitch() const;

  QString twoTheta() const;
  double twoThetaCorrection() const;
  double twoThetaNom() const;
  double twoThetaStep() const;
  QcepDoubleVector twoThetaAngles() const;

  QString omega() const;
  double omegaCorrection() const;
  double omegaNom() const;
  double omegaStep() const;
  QcepDoubleVector omegaAngles() const;

  QString chi() const;
  double chiCorrection() const;
  double chiNom() const;
  double chiStep() const;
  QcepDoubleVector chiAngles() const;

  QString phi() const;
  double phiCorrection() const;
  double phiNom() const;
  double phiStep() const;
  QcepDoubleVector phiAngles() const;

  CctwDoubleVector3D  gridOrigin() const;
  CctwDoubleMatrix3x3 gridBasis() const;
  CctwDoubleVector3D  gridDim() const;
  CctwDoubleVector3D  gridOffset() const;

//  int extraFlip() const;

  void setPixelSize(double sz);
  void setWavelength(double wv);
  void setDistance(double d);

  void setUnitCell(CctwUnitCell uc);
  void setUBMat(CctwDoubleMatrix3x3 ub);
  void setOMat(CctwDoubleMatrix3x3 om);
  void setOVec(CctwDoubleVector3D ov);

  void setDet0x(double d0x);
  void setDet0y(double d0y);

  void setXTrans(CctwDoubleVector3D xt);

  void setOrientErrorDetPitch(double x);
  void setOrientErrorDetRoll(double x);
  void setOrientErrorDetYaw(double x);
  void setOrientErrorGonPitch(double x);

  void setTwoTheta(const QString tth);
  void setOmega(const QString omg);
  void setChi(const QString chi);
  void setPhi(const QString phi);

  void setGridOrigin(CctwDoubleVector3D org);
  void setGridBasis(CctwDoubleMatrix3x3 bas);
  void setGridDim(CctwDoubleVector3D dim);
  void setGridOffset(CctwDoubleVector3D off);

  double twoThetaAngle(double frame);
  double omegaAngle(double frame);
  double chiAngle(double frame);
  double phiAngle(double frame);

//  void setExtraFlip(int extra);

private:
  void parseAngleArgument(const QString arg, const QString angleName, QcepDoubleProperty *corr, QcepDoubleProperty *start, QcepDoubleProperty *step, QcepDoubleVectorProperty *angs);

  double interpolateAngle(double frame, QcepDoubleProperty *corr, QcepDoubleProperty *start, QcepDoubleProperty *step, QcepDoubleVectorProperty *angs);

private:
  double m_PixelSize;
  double m_Wavelength;
  double m_Distance;

  CctwUnitCell        m_UnitCell;
  CctwDoubleMatrix3x3 m_UBMat;
  CctwDoubleMatrix3x3 m_OMat;
  CctwDoubleVector3D  m_OVec;

  double m_Det0x;
  double m_Det0y;

  CctwDoubleVector3D m_XTrans;

  double m_OrientErrorDetPitch;
  double m_OrientErrorDetRoll;
  double m_OrientErrorDetYaw;
  double m_OrientErrorGonPitch;

  CctwDoubleVector3D  m_GridOrigin;
  CctwDoubleMatrix3x3 m_GridBasis;
  CctwDoubleVector3D  m_GridDim;
  CctwDoubleVector3D  m_GridOffset;

public:
  Q_PROPERTY(double pixelSize                READ pixelSize              WRITE setPixelSize            NOTIFY parametersChanged)
  Q_PROPERTY(double wavelength               READ wavelength             WRITE setWavelength           NOTIFY parametersChanged)
  Q_PROPERTY(double distance                 READ distance               WRITE setDistance             NOTIFY parametersChanged)

  Q_PROPERTY(CctwUnitCell unitCell           READ unitCell               WRITE setUnitCell             NOTIFY parametersChanged)
  Q_PROPERTY(CctwDoubleMatrix3x3 ubMat       READ ubMat                  WRITE setUBMat                NOTIFY parametersChanged)
  Q_PROPERTY(CctwDoubleMatrix3x3 oMat        READ oMat                   WRITE setOMat                 NOTIFY parametersChanged)
  Q_PROPERTY(CctwDoubleVector3D  oVec        READ oVec                   WRITE setOVec                 NOTIFY parametersChanged)

  Q_PROPERTY(double det0x                    READ det0x                  WRITE setDet0x                NOTIFY parametersChanged)
  Q_PROPERTY(double det0y                    READ det0y                  WRITE setDet0y                NOTIFY parametersChanged)

  Q_PROPERTY(CctwDoubleVector3D xTrans       READ xTrans                 WRITE setXTrans               NOTIFY parametersChanged)

  Q_PROPERTY(double orientErrorDetPitch      READ orientErrorDetPitch    WRITE setOrientErrorDetPitch  NOTIFY parametersChanged)
  Q_PROPERTY(double orientErrorDetRoll       READ orientErrorDetRoll     WRITE setOrientErrorDetRoll   NOTIFY parametersChanged)
  Q_PROPERTY(double orientErrorDetYaw        READ orientErrorDetYaw      WRITE setOrientErrorDetYaw    NOTIFY parametersChanged)
  Q_PROPERTY(double orientErrorGonPitch      READ orientErrorGonPitch    WRITE setOrientErrorGonPitch  NOTIFY parametersChanged)

  Q_PROPERTY(QString twoTheta                READ get_TwoTheta           WRITE setTwoTheta             NOTIFY parametersChanged)
  QCEP_STRING_PROPERTY(TwoTheta)

  Q_PROPERTY(double twoThetaCorrection       READ get_TwoThetaCorrection WRITE set_TwoThetaCorrection  NOTIFY parametersChanged)
  QCEP_DOUBLE_PROPERTY(TwoThetaCorrection)

  Q_PROPERTY(double twoThetaNom              READ get_TwoThetaNom        WRITE set_TwoThetaNom         NOTIFY parametersChanged)
  QCEP_DOUBLE_PROPERTY(TwoThetaNom)

  Q_PROPERTY(double twoThetaStep             READ get_TwoThetaStep       WRITE set_TwoThetaStep        NOTIFY parametersChanged)
  QCEP_DOUBLE_PROPERTY(TwoThetaStep)

  Q_PROPERTY(QcepDoubleVector twoThetaAngles READ get_TwoThetaAngles     WRITE set_TwoThetaAngles      NOTIFY parametersChanged)
  QCEP_DOUBLE_VECTOR_PROPERTY(TwoThetaAngles)


  Q_PROPERTY(QString omega                   READ get_Omega              WRITE setOmega                NOTIFY parametersChanged)
  QCEP_STRING_PROPERTY(Omega)

  Q_PROPERTY(double omegaCorrection          READ get_OmegaCorrection    WRITE set_OmegaCorrection     NOTIFY parametersChanged)
  QCEP_DOUBLE_PROPERTY(OmegaCorrection)

  Q_PROPERTY(double omegaNom                 READ get_OmegaNom           WRITE set_OmegaNom            NOTIFY parametersChanged)
  QCEP_DOUBLE_PROPERTY(OmegaNom)

  Q_PROPERTY(double omegaStep                READ get_OmegaStep          WRITE set_OmegaStep           NOTIFY parametersChanged)
  QCEP_DOUBLE_PROPERTY(OmegaStep)

  Q_PROPERTY(QcepDoubleVector omegaAngles    READ get_OmegaAngles        WRITE set_OmegaAngles         NOTIFY parametersChanged)
  QCEP_DOUBLE_VECTOR_PROPERTY(OmegaAngles)


  Q_PROPERTY(QString chi                     READ get_Chi                WRITE setChi                  NOTIFY parametersChanged)
  QCEP_STRING_PROPERTY(Chi)

  Q_PROPERTY(double chiCorrection            READ get_ChiCorrection      WRITE set_ChiCorrection       NOTIFY parametersChanged)
  QCEP_DOUBLE_PROPERTY(ChiCorrection)

  Q_PROPERTY(double chiNom                   READ get_ChiNom             WRITE set_ChiNom              NOTIFY parametersChanged)
  QCEP_DOUBLE_PROPERTY(ChiNom)

  Q_PROPERTY(double chiStep                  READ get_ChiStep            WRITE set_ChiStep             NOTIFY parametersChanged)
  QCEP_DOUBLE_PROPERTY(ChiStep)

  Q_PROPERTY(QcepDoubleVector chiAngles      READ get_ChiAngles          WRITE set_ChiAngles           NOTIFY parametersChanged)
  QCEP_DOUBLE_VECTOR_PROPERTY(ChiAngles)


  Q_PROPERTY(QString phi                     READ get_Phi                WRITE setPhi                  NOTIFY parametersChanged)
  QCEP_STRING_PROPERTY(Phi)

  Q_PROPERTY(double phiCorrection            READ get_PhiCorrection      WRITE set_PhiCorrection       NOTIFY parametersChanged)
  QCEP_DOUBLE_PROPERTY(PhiCorrection)

  Q_PROPERTY(double phiNom                   READ get_PhiNom             WRITE set_PhiNom              NOTIFY parametersChanged)
  QCEP_DOUBLE_PROPERTY(PhiNom)

  Q_PROPERTY(double phiStep                  READ get_PhiStep            WRITE set_PhiStep             NOTIFY parametersChanged)
  QCEP_DOUBLE_PROPERTY(PhiStep)

  Q_PROPERTY(QcepDoubleVector phiAngles      READ get_PhiAngles          WRITE set_PhiAngles           NOTIFY parametersChanged)
  QCEP_DOUBLE_VECTOR_PROPERTY(PhiAngles)

  Q_PROPERTY(CctwDoubleVector3D  gridOrigin  READ gridOrigin             WRITE setGridOrigin           NOTIFY parametersChanged)
  Q_PROPERTY(CctwDoubleMatrix3x3 gridBasis   READ gridBasis              WRITE setGridBasis            NOTIFY parametersChanged)
  Q_PROPERTY(CctwDoubleVector3D  gridDim     READ gridDim                WRITE setGridDim              NOTIFY parametersChanged)
  Q_PROPERTY(CctwDoubleVector3D  gridOffset  READ gridOffset             WRITE setGridOffset           NOTIFY parametersChanged)

  Q_PROPERTY(bool extraFlip                  READ get_ExtraFlip          WRITE set_ExtraFlip           NOTIFY parametersChanged)
  QCEP_BOOLEAN_PROPERTY(ExtraFlip)
};

#endif // CCTWCRYSTALCOORDINATEPARAMETERS_H
