#ifndef CCTWQTCRYSTALCOORDINATETRANSFORM_H
#define CCTWQTCRYSTALCOORDINATETRANSFORM_H

#include <QObject>
#include "cctwcrystalcoordinatetransform.h"

class CctwqtCrystalCoordinateTransform : public QObject, public CctwCrystalCoordinateTransform
{
  Q_OBJECT
public:
  explicit CctwqtCrystalCoordinateTransform(QObject *parent = 0);
  
signals:
  
public slots:
  
};

#endif // CCTWQTCRYSTALCOORDINATETRANSFORM_H
