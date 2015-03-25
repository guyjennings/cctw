#ifndef CCTWTRANSFORMINTERFACE_H
#define CCTWTRANSFORMINTERFACE_H

#include "cctwobject.h"
#include "cctwvector3d.h"

class CctwTransformInterface : public CctwObject
{
  Q_OBJECT
public:
  CctwTransformInterface(QString name, QObject *parent);

public slots:

  virtual bool hasInverse() const = 0;

  virtual CctwDoubleVector3D forward(CctwDoubleVector3D d) = 0;
                                          // Transform from input to output space coordinates
  virtual CctwDoubleVector3D inverse(CctwDoubleVector3D q) = 0;
                                          // Back transform from output to input space coordinates

  virtual CctwDoubleVector3D forward(double x, double y, double z);
  virtual CctwDoubleVector3D inverse(double x, double y, double z);
};

#endif // CCTWTRANSFORMINTERFACE_H
