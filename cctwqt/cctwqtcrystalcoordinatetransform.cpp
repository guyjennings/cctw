#include "cctwqtcrystalcoordinatetransform.h"

CctwqtCrystalCoordinateTransform::CctwqtCrystalCoordinateTransform(CctwqtCrystalCoordinateParameters *parms, QObject *parent) :
  CctwCrystalCoordinateTransform(parms, parent),
  m_QTParms(parms)
{
}

void CctwqtCrystalCoordinateTransform::report()
{
  printMessage(tr("CctwqtCrystalCoordinateParameters - get_Wavelength() = %1")
               .arg(m_QTParms->get_Wavelength()));

  printMessage(tr("CctwCrystalCoordinateParameters::m_Wavelength = %1")
               .arg(((CctwCrystalCoordinateParameters*) m_QTParms)->m_Wavelength));
}
