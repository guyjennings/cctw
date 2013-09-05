#include "cctwunitcell.h"

CctwUnitCell::CctwUnitCell() :
  m_A(0),
  m_B(0),
  m_C(0),
  m_Alpha(0),
  m_Beta(0),
  m_Gamma(0)
{
}

CctwUnitCell::CctwUnitCell(double a, double b, double c, double alpha, double beta, double gamma) :
  m_A(a),
  m_B(b),
  m_C(c),
  m_Alpha(alpha),
  m_Beta(beta),
  m_Gamma(gamma)
{
}

CctwUnitCell::CctwUnitCell(CctwDoubleVector3D lengths, CctwDoubleVector3D angles) :
  m_A(lengths.x()),
  m_B(lengths.y()),
  m_C(lengths.z()),
  m_Alpha(angles.x()),
  m_Beta(angles.y()),
  m_Gamma(angles.z())
{
}

double CctwUnitCell::a() const
{
  return m_A;
}

double& CctwUnitCell::a()
{
  return m_A;
}

double CctwUnitCell::b() const
{
  return m_B;
}

double& CctwUnitCell::b()
{
  return m_B;
}

double CctwUnitCell::c() const
{
  return m_C;
}

double& CctwUnitCell::c()
{
  return m_C;
}

double CctwUnitCell::alpha() const
{
  return m_Alpha;
}

double& CctwUnitCell::alpha()
{
  return m_Alpha;
}

double CctwUnitCell::beta() const
{
  return m_Beta;
}

double& CctwUnitCell::beta()
{
  return m_Beta;
}

double CctwUnitCell::gamma() const
{
  return m_Gamma;
}

double& CctwUnitCell::gamma()
{
  return m_Gamma;
}

void CctwUnitCell::setValue(double a, double b, double c, double alpha, double beta, double gamma)
{
  m_A = a;
  m_B = b;
  m_C = c;
  m_Alpha = alpha;
  m_Beta = beta;
  m_Gamma = gamma;
}

void CctwUnitCell::setValue(CctwDoubleVector3D lengths, CctwDoubleVector3D angles)
{
  m_A = lengths.x();
  m_B = lengths.y();
  m_C = lengths.z();

  m_Alpha = angles.x();
  m_Beta = angles.y();
  m_Gamma = angles.z();
}

CctwDoubleVector3D CctwUnitCell::lengths() const
{
  return CctwDoubleVector3D(m_A, m_B, m_C);
}

CctwDoubleVector3D CctwUnitCell::angles() const
{
  return CctwDoubleVector3D(m_Alpha, m_Beta, m_Gamma);
}

bool CctwUnitCell::operator == (const CctwUnitCell &cell) const
{
  return a()==cell.a() && b()==cell.b() && c()==cell.c() &&
      alpha()==cell.alpha() && beta()==cell.beta() && gamma()==cell.gamma();
}

bool CctwUnitCell::operator != (const CctwUnitCell &cell) const
{
  return a()!=cell.a() || b()!=cell.b() || c()!=cell.c() ||
      alpha()!=cell.alpha() || beta()!=cell.beta() || gamma()!=cell.gamma();
}

#ifdef USE_QT

void CctwUnitCell::setSettingsValue(QSettings *settings, QString name)
{
  settings->beginGroup(name);

  settings->setValue("a", a());
  settings->setValue("b", b());
  settings->setValue("c", c());
  settings->setValue("alpha", alpha());
  settings->setValue("beta",  beta());
  settings->setValue("gamma", gamma());

  settings->endGroup();
}

void CctwUnitCell::customSaver(const QVariant &val, QSettings *settings, QString name)
{
  CctwUnitCell vec = val.value<CctwUnitCell>();

  vec.setSettingsValue(settings, name);
}

#endif

