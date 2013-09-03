#ifndef CCTWQTUNITCELL_H
#define CCTWQTUNITCELL_H

#include "cctwunitcell.h"
#include "qceppropertyvalue.h"

class CctwqtUnitCell : public CctwUnitCell, public QcepPropertyValue
{
public:
  CctwqtUnitCell();
  CctwqtUnitCell(double a, double b, double c, double alpha, double beta, double gamma);
  CctwqtUnitCell(CctwDoubleVector3D lengths, CctwDoubleVector3D angles);

  void setSettingsValue(QSettings *settings, QString name);
};

#endif // CCTWQTUNITCELL_H
