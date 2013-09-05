#ifndef CCTWUNITCELL_H
#define CCTWUNITCELL_H

#include "cctwvector3d.h"

#ifdef USE_QT
#include <QSettings>
#include <QString>
#endif

class CctwUnitCell
{
public:
  CctwUnitCell();
  CctwUnitCell(double a, double b, double c, double alpha, double beta, double gamma);
  CctwUnitCell(CctwDoubleVector3D lengths, CctwDoubleVector3D angles);

  double a() const;
  double& a();

  double b() const;
  double& b();

  double c() const;
  double& c();

  double alpha() const;
  double& alpha();

  double beta() const;
  double& beta();

  double gamma() const;
  double& gamma();

  void setValue(double a, double b, double c, double alpha, double beta, double gamma);
  void setValue(CctwDoubleVector3D lengths, CctwDoubleVector3D angles);

  CctwDoubleVector3D lengths() const;
  CctwDoubleVector3D angles() const;

  bool operator == (const CctwUnitCell &cell) const;
  bool operator != (const CctwUnitCell &cell) const;

#ifdef USE_QT
  void setSettingsValue(QSettings *settings, QString name);
  static void customSaver(const QVariant &val, QSettings *settings, QString name);
#endif

private:
  double m_A;
  double m_B;
  double m_C;
  double m_Alpha;
  double m_Beta;
  double m_Gamma;
};

#ifdef USE_QT
Q_DECLARE_METATYPE(CctwUnitCell)
#endif

#endif // CCTWUNITCELL_H
