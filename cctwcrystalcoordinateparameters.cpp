#include "cctwcrystalcoordinateparameters.h"

#ifdef USE_QT

CctwCrystalCoordinateParameters::CctwCrystalCoordinateParameters(QObject *parent) :
  CctwqtObject(parent)
{
}

#else

CctwCrystalCoordinateParameters::CctwCrystalCoordinateParameters()
{
}

#endif
