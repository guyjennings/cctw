#ifndef CCTWQTCRYSTALCOORDINATETRANSFORM_H
#define CCTWQTCRYSTALCOORDINATETRANSFORM_H

#include "cctwqtobject.h"
#include "cctwcrystalcoordinatetransform.h"
#include "cctwqtcrystalcoordinateparameters.h"

class CctwqtCrystalCoordinateTransform : public CctwCrystalCoordinateTransform
{
  Q_OBJECT
public:
  explicit CctwqtCrystalCoordinateTransform(CctwqtCrystalCoordinateParameters *parms, QObject *parent);
  
signals:
  
public slots:
  double changeCount() { return CctwCrystalCoordinateTransform::m_CurrentFrameChangeCount; }
  void report();

private:
  CctwqtCrystalCoordinateParameters *m_QTParms;
};

#endif // CCTWQTCRYSTALCOORDINATETRANSFORM_H
