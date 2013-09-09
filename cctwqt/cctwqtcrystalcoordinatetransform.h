#ifndef CCTWQTCRYSTALCOORDINATETRANSFORM_H
#define CCTWQTCRYSTALCOORDINATETRANSFORM_H

#include "cctwqtobject.h"
#include "../cctwcrystalcoordinatetransform.h"

class CctwqtCrystalCoordinateTransform : public CctwCrystalCoordinateTransform
{
  Q_OBJECT
public:
  explicit CctwqtCrystalCoordinateTransform(CctwCrystalCoordinateParameters *parms, QObject *parent);
  
signals:
  
public slots:
  double changeCount() { return CctwCrystalCoordinateTransform::m_CurrentFrameChangeCount; }
};

#endif // CCTWQTCRYSTALCOORDINATETRANSFORM_H
