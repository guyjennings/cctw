#ifndef CCTWROTATIONTRANSFORM_H
#define CCTWROTATIONTRANSFORM_H

#include "cctwtransforminterface.h"

class CctwRotationTransform : public CctwTransformInterface
{
  Q_OBJECT
public:
  CctwRotationTransform(QObject *parent);

public:

  virtual bool hasInverse() const;

  virtual CctwDoubleVector3D forward(CctwDoubleVector3D a);
                                          // Transform from input to output space coordinates
  virtual CctwDoubleVector3D inverse(CctwDoubleVector3D);
                                          // Back transform from output to input space coordinates
};

#endif // CCTWROTATIONTRANSFORM_H
