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
  CctwCrystalCoordinateParameters(QString name, QObject *parent);

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
  void setTwoThetaCorrection(const double tthc);
  void setTwoThetaNom(const double tthn);
  void setTwoThetaStep(const double tths);
  void setTwoThetaAngles(const QcepDoubleVector angs);

  void setOmega(const QString omg);
  void setOmegaCorrection(const double omgc);
  void setOmegaNom(const double omgn);
  void setOmegaStep(const double omgs);
  void setOmegaAngles(const QcepDoubleVector angs);

  void setChi(const QString chi);
  void setChiCorrection(double chic);
  void setChiNom(double chin);
  void setChiStep(const double omgs);
  void setChiAngles(const QcepDoubleVector angs);

  void setPhi(const QString phi);
  void setPhiCorrection(double phic);
  void setPhiNom(double phin);
  void setPhiStep(const double omgs);
  void setPhiAngles(const QcepDoubleVector angs);

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
  void parseAngleArgument(const QString arg, const QString angleName, double *corr, double *start, double *step, QcepDoubleVector *angs);

  double interpolateAngle(double frame, double corr, double start, double step, QcepDoubleVector angs);

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

  QString m_TwoTheta;
  double m_TwoThetaCorrection;
  double m_TwoThetaNom;
  double m_TwoThetaStep;
  QcepDoubleVector m_TwoThetaAngles;

  QString m_Omega;
  double m_OmegaCorrection;
  double m_OmegaNom;
  double m_OmegaStep;
  QcepDoubleVector m_OmegaAngles;

  QString m_Chi;
  double m_ChiCorrection;
  double m_ChiNom;
  double m_ChiStep;
  QcepDoubleVector m_ChiAngles;

  QString m_Phi;
  double m_PhiCorrection;
  double m_PhiNom;
  double m_PhiStep;
  QcepDoubleVector m_PhiAngles;

  CctwDoubleVector3D  m_GridOrigin;
  CctwDoubleMatrix3x3 m_GridBasis;
  CctwDoubleVector3D  m_GridDim;
  CctwDoubleVector3D  m_GridOffset;

public:
  Q_PROPERTY(double pixelSize                READ pixelSize            WRITE setPixelSize            NOTIFY parametersChanged)
  Q_PROPERTY(double wavelength               READ wavelength           WRITE setWavelength           NOTIFY parametersChanged)
  Q_PROPERTY(double distance                 READ distance             WRITE setDistance             NOTIFY parametersChanged)

  Q_PROPERTY(CctwUnitCell unitCell           READ unitCell             WRITE setUnitCell             NOTIFY parametersChanged)
  Q_PROPERTY(CctwDoubleMatrix3x3 ubMat       READ ubMat                WRITE setUBMat                NOTIFY parametersChanged)
  Q_PROPERTY(CctwDoubleMatrix3x3 oMat        READ oMat                 WRITE setOMat                 NOTIFY parametersChanged)
  Q_PROPERTY(CctwDoubleVector3D  oVec        READ oVec                 WRITE setOVec                 NOTIFY parametersChanged)

  Q_PROPERTY(double det0x                    READ det0x                WRITE setDet0x                NOTIFY parametersChanged)
  Q_PROPERTY(double det0y                    READ det0y                WRITE setDet0y                NOTIFY parametersChanged)

  Q_PROPERTY(CctwDoubleVector3D xTrans       READ xTrans               WRITE setXTrans               NOTIFY parametersChanged)

  Q_PROPERTY(double orientErrorDetPitch      READ orientErrorDetPitch  WRITE setOrientErrorDetPitch  NOTIFY parametersChanged)
  Q_PROPERTY(double orientErrorDetRoll       READ orientErrorDetRoll   WRITE setOrientErrorDetRoll   NOTIFY parametersChanged)
  Q_PROPERTY(double orientErrorDetYaw        READ orientErrorDetYaw    WRITE setOrientErrorDetYaw    NOTIFY parametersChanged)
  Q_PROPERTY(double orientErrorGonPitch      READ orientErrorGonPitch  WRITE setOrientErrorGonPitch  NOTIFY parametersChanged)

  Q_PROPERTY(QString twoTheta                READ twoTheta             WRITE setTwoTheta             NOTIFY parametersChanged)
  Q_PROPERTY(double twoThetaCorrection       READ twoThetaCorrection   WRITE setTwoThetaCorrection   NOTIFY parametersChanged)
  Q_PROPERTY(double twoThetaNom              READ twoThetaNom          WRITE setTwoThetaNom          NOTIFY parametersChanged)
  Q_PROPERTY(double twoThetaStep             READ twoThetaStep         WRITE setTwoThetaStep         NOTIFY parametersChanged)
  Q_PROPERTY(QcepDoubleVector twoThetaAngles READ twoThetaAngles       WRITE setTwoThetaAngles       NOTIFY parametersChanged)

  Q_PROPERTY(QString omega                   READ omega                WRITE setOmega                NOTIFY parametersChanged)
  Q_PROPERTY(double omegaCorrection          READ omegaCorrection      WRITE setOmegaCorrection      NOTIFY parametersChanged)
  Q_PROPERTY(double omegaNom                 READ omegaNom             WRITE setOmegaNom             NOTIFY parametersChanged)
  Q_PROPERTY(double omegaStep                READ omegaStep            WRITE setOmegaStep            NOTIFY parametersChanged)
  Q_PROPERTY(QcepDoubleVector omegaAngles    READ omegaAngles          WRITE setOmegaAngles          NOTIFY parametersChanged)

  Q_PROPERTY(QString chi                     READ chi                  WRITE setChi                  NOTIFY parametersChanged)
  Q_PROPERTY(double chiCorrection            READ chiCorrection        WRITE setChiCorrection        NOTIFY parametersChanged)
  Q_PROPERTY(double chiNom                   READ chiNom               WRITE setChiNom               NOTIFY parametersChanged)
  Q_PROPERTY(double chiStep                  READ chiStep              WRITE setChiStep              NOTIFY parametersChanged)
  Q_PROPERTY(QcepDoubleVector chiAngles      READ chiAngles            WRITE setChiAngles            NOTIFY parametersChanged)

  Q_PROPERTY(QString phi                     READ phi                  WRITE setPhi                  NOTIFY parametersChanged)
  Q_PROPERTY(double phiCorrection            READ phiCorrection        WRITE setPhiCorrection        NOTIFY parametersChanged)
  Q_PROPERTY(double phiNom                   READ phiNom               WRITE setPhiNom               NOTIFY parametersChanged)
  Q_PROPERTY(double phiStep                  READ phiStep              WRITE setPhiStep              NOTIFY parametersChanged)
  Q_PROPERTY(QcepDoubleVector phiAngles      READ phiAngles            WRITE setPhiAngles            NOTIFY parametersChanged)

  Q_PROPERTY(CctwDoubleVector3D  gridOrigin  READ gridOrigin           WRITE setGridOrigin           NOTIFY parametersChanged)
  Q_PROPERTY(CctwDoubleMatrix3x3 gridBasis   READ gridBasis            WRITE setGridBasis            NOTIFY parametersChanged)
  Q_PROPERTY(CctwDoubleVector3D  gridDim     READ gridDim              WRITE setGridDim              NOTIFY parametersChanged)
  Q_PROPERTY(CctwDoubleVector3D  gridOffset  READ gridOffset           WRITE setGridOffset           NOTIFY parametersChanged)

  Q_PROPERTY(bool extraFlip                  READ get_ExtraFlip        WRITE set_ExtraFlip           NOTIFY parametersChanged)
  QCEP_BOOLEAN_PROPERTY(ExtraFlip)
};

#endif // CCTWCRYSTALCOORDINATEPARAMETERS_H
