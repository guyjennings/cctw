#ifndef CCTWQTCRYSTALCOORDINATEPARAMETERS_H
#define CCTWQTCRYSTALCOORDINATEPARAMETERS_H

#include "cctwobject.h"
#include "cctwcrystalcoordinateparameters.h"
#include "cctwqtunitcellproperty.h"
#include "cctwqtdoublematrix3x3property.h"
#include "cctwqtdoublevector3dproperty.h"

class CctwqtApplication;

class CctwqtCrystalCoordinateParameters : public CctwCrystalCoordinateParameters
{
  Q_OBJECT
public:
  explicit CctwqtCrystalCoordinateParameters(CctwqtApplication *app, QObject *parent=0);
  typedef CctwCrystalCoordinateParameters inherited;

signals:
  
public slots:
  void setDefaults();

private:
  CctwqtApplication *m_Application;
};

#endif // CCTWQTCRYSTALCOORDINATEPARAMETERS_H
