#ifndef CCTWQTCRYSTALCOORDINATETRANSFORM_H
#define CCTWQTCRYSTALCOORDINATETRANSFORM_H

#include "cctwqtobject.h"
#include "cctwcrystalcoordinatetransform.h"

class CctwqtCrystalCoordinateTransform : public CctwqtObject, public CctwCrystalCoordinateTransform
{
  Q_OBJECT
public:
  explicit CctwqtCrystalCoordinateTransform(QObject *parent);
  
signals:
  
public slots:
  
};

#endif // CCTWQTCRYSTALCOORDINATETRANSFORM_H
