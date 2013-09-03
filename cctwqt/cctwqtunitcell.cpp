#include "cctwqtunitcell.h"

CctwqtUnitCell::CctwqtUnitCell()
{
}

CctwqtUnitCell::CctwqtUnitCell(CctwDoubleVector3D lengths, CctwDoubleVector3D angles) :
  CctwUnitCell(lengths, angles)
{
}

CctwqtUnitCell::CctwqtUnitCell(double a, double b, double c, double alpha, double beta, double gamma) :
  CctwUnitCell(a,b,c,alpha,beta,gamma)
{
}

void CctwqtUnitCell::setSettingsValue(QSettings *settings, QString name)
{
}
