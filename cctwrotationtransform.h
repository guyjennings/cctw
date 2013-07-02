#ifndef CCTWROTATIONTRANSFORM_H
#define CCTWROTATIONTRANSFORM_H

#include "cctwtransforminterface.h"

class CctwRotationTransform : public CctwTransformInterface
{
public:
  CctwRotationTransform();

public:
  virtual CctwVector3D forward(CctwVector3D a);
                                          // Transform from input to output space coordinates
  virtual CctwVector3D inverse(CctwVector3D q);
                                          // Back transform from output to input space coordinates
};

#endif // CCTWROTATIONTRANSFORM_H
