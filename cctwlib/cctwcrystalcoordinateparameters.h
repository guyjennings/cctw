#ifndef CCTWCRYSTALCOORDINATEPARAMETERS_H
#define CCTWCRYSTALCOORDINATEPARAMETERS_H

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

  double det0x() const;
  double det0y() const;

  CctwDoubleVector3D xTrans() const;

  double orientErrorDetPitch() const;
  double orientErrorDetRoll() const;
  double orientErrorDetYaw() const;
  double orientErrorGonPitch() const;

  double twoThetaCorrection() const;
  double twoThetaNom() const;

  double omegaCorrection() const;
  double omegaStep() const;

  double chiCorrection() const;
  double chiNom() const;

  double phiCorrection() const;
  double phiNom() const;

  CctwDoubleVector3D  gridOrigin() const;
  CctwDoubleMatrix3x3 gridBasis() const;
  CctwDoubleVector3D  gridDim() const;

  void setPixelSize(double sz);
  void setWavelength(double wv);
  void setDistance(double d);

  void setUnitCell(CctwUnitCell uc);
  void setUBMat(CctwDoubleMatrix3x3 ub);

  void setDet0x(double d0x);
  void setDet0y(double d0y);

  void setXTrans(CctwDoubleVector3D xt);

  void setOrientErrorDetPitch(double x);
  void setOrientErrorDetRoll(double x);
  void setOrientErrorDetYaw(double x);
  void setOrientErrorGonPitch(double x);

  void setTwoThetaCorrection(double tthc);
  void setTwoThetaNom(double tthn);

  void setOmegaCorrection(double omgc);
  void setOmegaStep(double omgs);

  void setChiCorrection(double chic);
  void setChiNom(double chin);

  void setPhiCorrection(double phic);
  void setPhiNom(double phin);

  void setGridOrigin(CctwDoubleVector3D org);
  void setGridBasis(CctwDoubleMatrix3x3 bas);
  void setGridDim(CctwDoubleVector3D dim);

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

public:
  Q_PROPERTY(double pixelSize                READ pixelSize            WRITE setPixelSize            NOTIFY parametersChanged)
  Q_PROPERTY(double wavelength               READ wavelength           WRITE setWavelength           NOTIFY parametersChanged)
  Q_PROPERTY(double distance                 READ distance             WRITE setDistance             NOTIFY parametersChanged)

  Q_PROPERTY(CctwUnitCell unitCell           READ unitCell             WRITE setUnitCell             NOTIFY parametersChanged)
  Q_PROPERTY(CctwDoubleMatrix3x3 ubMat       READ ubMat                WRITE setUBMat                NOTIFY parametersChanged)

  Q_PROPERTY(double det0x                    READ det0x                WRITE setDet0x                NOTIFY parametersChanged)
  Q_PROPERTY(double det0y                    READ det0y                WRITE setDet0y                NOTIFY parametersChanged)

  Q_PROPERTY(CctwDoubleVector3D xTrans       READ xTrans               WRITE setXTrans               NOTIFY parametersChanged)

  Q_PROPERTY(double orientErrorDetPitch      READ orientErrorDetPitch  WRITE setOrientErrorDetPitch  NOTIFY parametersChanged)
  Q_PROPERTY(double orientErrorDetRoll       READ orientErrorDetRoll   WRITE setOrientErrorDetRoll   NOTIFY parametersChanged)
  Q_PROPERTY(double orientErrorDetYaw        READ orientErrorDetYaw    WRITE setOrientErrorDetYaw    NOTIFY parametersChanged)
  Q_PROPERTY(double orientErrorGonPitch      READ orientErrorGonPitch  WRITE setOrientErrorGonPitch  NOTIFY parametersChanged)

  Q_PROPERTY(double twoThetaCorrection       READ twoThetaCorrection   WRITE setTwoThetaCorrection   NOTIFY parametersChanged)
  Q_PROPERTY(double twoThetaNom              READ twoThetaNom          WRITE setTwoThetaNom          NOTIFY parametersChanged)

  Q_PROPERTY(double omegaCorrection          READ omegaCorrection      WRITE setOmegaCorrection      NOTIFY parametersChanged)
  Q_PROPERTY(double omegaStep                READ omegaStep            WRITE setOmegaStep            NOTIFY parametersChanged)

  Q_PROPERTY(double chiCorrection            READ chiCorrection        WRITE setChiCorrection        NOTIFY parametersChanged)
  Q_PROPERTY(double chiNom                   READ chiNom               WRITE setChiNom               NOTIFY parametersChanged)

  Q_PROPERTY(double phiCorrection            READ phiCorrection        WRITE setPhiCorrection        NOTIFY parametersChanged)
  Q_PROPERTY(double phiNom                   READ phiNom               WRITE setPhiNom               NOTIFY parametersChanged)

  Q_PROPERTY(CctwDoubleVector3D  gridOrigin  READ gridOrigin           WRITE setGridOrigin           NOTIFY parametersChanged)
  Q_PROPERTY(CctwDoubleMatrix3x3 gridBasis   READ gridBasis            WRITE setGridBasis            NOTIFY parametersChanged)
  Q_PROPERTY(CctwDoubleVector3D  gridDim     READ gridDim              WRITE setGridDim              NOTIFY parametersChanged)
};

#endif // CCTWCRYSTALCOORDINATEPARAMETERS_H
