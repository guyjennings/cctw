#ifndef CCTWQTCRYSTALCOORDINATETRANSFORM_H
#define CCTWQTCRYSTALCOORDINATETRANSFORM_H

#include "cctwobject.h"
#include "cctwcrystalcoordinatetransform.h"
#include "cctwcrystalcoordinateparameters.h"

class CctwqtCrystalCoordinateTransform : public CctwCrystalCoordinateTransform
{
  Q_OBJECT
public:
  explicit CctwqtCrystalCoordinateTransform(CctwCrystalCoordinateParameters *parms, QObject *parent);
  
signals:
  
public slots:
  double changeCount() { return CctwCrystalCoordinateTransform::m_CurrentFrameChangeCount; }
  void report();

private:
  CctwCrystalCoordinateParameters *m_QTParms;
};

#endif // CCTWQTCRYSTALCOORDINATETRANSFORM_H
